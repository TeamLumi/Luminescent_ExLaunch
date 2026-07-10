#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_trainer_card.h"
#include "features/overworld_multiplayer.h"

#include "externals/PlayerWork.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UICard.h"
#include "externals/System/String.h"
#include "romdata/data/ColorSet.h"

#include <cstring>

// ---------------------------------------------------------------------------
// Game RVAs (accessor set mirrors UnionStateTransitionController.
// TransitionShowTrainerCard @0x19F3CD0 — vanilla's own card-blob collection)
// ---------------------------------------------------------------------------
static constexpr uintptr_t RVA_PW_GET_TRAINER_RANK   = 0x2CF5770; // PlayerWork.GetTrainerRank
static constexpr uintptr_t RVA_PW_GET_COLOR_ID       = 0x2CEF820; // PlayerWork.get_colorID
static constexpr uintptr_t RVA_ZUKAN_ZENKOKU_FLAG    = 0x17DB490; // ZukanWork.GetZenkokuFlag
static constexpr uintptr_t RVA_ZUKAN_SEE_COUNT       = 0x17DB8F0; // ZukanWork.SeeCount
static constexpr uintptr_t RVA_ZUKAN_SEE_SINOU       = 0x17DBA80; // ZukanWork.SeeSinouCount
static constexpr uintptr_t RVA_RECORDWORK_GET        = 0x19A4660; // RecordWork.Get(index)
static constexpr uintptr_t RVA_SYSTIME_GET_START     = 0x2CADD30; // SystemTimeWork.GetStartTime
static constexpr uintptr_t RVA_UGITEM_STATUE_KINDS   = 0x18D20F0; // UgItemWork.GetHaveStatueKindNum
static constexpr uintptr_t RVA_PW_GET_UG_COUNT_REC   = 0x2CF4680; // PlayerWork.get_UgCountRecord
static constexpr uintptr_t RVA_PLAYERNAME_CTOR       = 0x1FA31F0; // Dpr.Message.PlayerNameData..ctor
static constexpr uintptr_t RVA_UICARD_PARAM_CTOR     = 0x17A9F10; // Dpr.UI.UICard.Param..ctor

// TypeInfo pointer globals (RAM offsets; populated after system_load_typeinfo)
static constexpr uintptr_t RAM_PLAYERNAME_TYPEINFO   = 0x4C5D9A8; // Dpr.Message.PlayerNameData_TypeInfo
static constexpr uintptr_t RAM_UICARD_PARAM_TYPEINFO = 0x4C65908; // Dpr.UI.UICard.Param_TypeInfo
// Metadata usage token from vanilla OpenTargetTranerCard — loads the exact
// TypeInfo/method set that path needs (UICard.Param, PlayerNameData,
// CreateUIWindow<UICard>, ...).
static constexpr int64_t   TYPEINFO_TOKEN_CARD_PATH  = 0xA20F;

// MYSTATUS raw offsets (per vanilla pseudocode reads)
static constexpr uintptr_t MYSTATUS_TRAINER_ID_OFFSET = 0x8;
static constexpr uintptr_t MYSTATUS_SEX_OFFSET        = 0x10;

// RecordWork indices (from the vanilla collection site)
static constexpr uint32_t REC_CLEAR_TIME       = 0x00;
static constexpr uint32_t REC_RENSHOU_SINGLE   = 0x06;
static constexpr uint32_t REC_RENSHOU_DOUBLE   = 0x08;
static constexpr uint32_t REC_RENSHOU_MSINGLE  = 0x0A;
static constexpr uint32_t REC_RENSHOU_MDOUBLE  = 0x0C;
static constexpr uint32_t REC_CONTEST_FIRST    = 0x0F; // 0x0F..0x13 = style..strong

// UICard.Param field offsets (dump.cs TypeDefIndex 8330)
static constexpr uintptr_t PARAM_IS_SHOW_BADGE  = 0x10;
static constexpr uintptr_t PARAM_CARD_RANK      = 0x14;
static constexpr uintptr_t PARAM_FASHION        = 0x18;
static constexpr uintptr_t PARAM_BODY_TYPE      = 0x19;
static constexpr uintptr_t PARAM_SEX            = 0x1A;
static constexpr uintptr_t PARAM_PLAYER_ID      = 0x1C;
static constexpr uintptr_t PARAM_NAME_DATA      = 0x20;
static constexpr uintptr_t PARAM_MONEY          = 0x28;
static constexpr uintptr_t PARAM_IS_GET_ZUKAN   = 0x2C;
static constexpr uintptr_t PARAM_ZUKAN_SEE      = 0x30;
static constexpr uintptr_t PARAM_PLAY_HOUR      = 0x34;
static constexpr uintptr_t PARAM_PLAY_MIN       = 0x36;
static constexpr uintptr_t PARAM_START_TIME     = 0x38;
static constexpr uintptr_t PARAM_CLEAR_TIME     = 0x40;
static constexpr uintptr_t PARAM_CONTEST_FIRST  = 0x44; // 5 × uint
static constexpr uintptr_t PARAM_RENSHOU_FIRST  = 0x58; // 4 × uint
static constexpr uintptr_t PARAM_POFFIN_COUNT   = 0x68;
static constexpr uintptr_t PARAM_FOSSIL_COUNT   = 0x6C;
static constexpr uintptr_t PARAM_STATUE_KINDS   = 0x70;

// PlayerNameData field offsets
static constexpr uintptr_t NAMEDATA_NICKNAME = 0x10;
static constexpr uintptr_t NAMEDATA_GENDER   = 0x18;
static constexpr uintptr_t NAMEDATA_LANG     = 0x1C;

// ---------------------------------------------------------------------------
// Wire blob — field set mirrors vanilla's TranerData/TranerCardData
// ---------------------------------------------------------------------------

struct __attribute__((packed)) CardBlob {
    uint8_t  fashion;
    uint8_t  colorId;      // model color variant (LoadModels hook)
    uint8_t  bodyType;     // Param.PlayerBodyType (2D layout variant)
    uint8_t  genderid;     // MYSTATUS sex byte ^ 1, as vanilla sends it
    uint8_t  langId;
    uint8_t  cardRank;
    uint32_t trainerId;
    int32_t  money;
    uint8_t  hasZenkoku;
    int32_t  zukanSee;
    uint16_t playHour;
    uint8_t  playMin;
    int64_t  startTime;
    uint32_t clearTime;
    uint32_t contest[5];
    uint32_t renshou[4];
    uint8_t  poffinCount;
    int16_t  fossilCount;
    uint8_t  statueKinds;
};
static_assert(sizeof(CardBlob) == 74, "CardBlob wire size");

// Pending received card (opened from the tick, not the packet callback)
static bool     s_cardPending = false;
static int32_t  s_cardFromStation = -1;
static CardBlob s_pendingBlob = {};
// One-shot: next CardModelViewController.LoadModels shows the peer's appearance
static bool     s_modelOverrideArmed = false;

// ---------------------------------------------------------------------------
// Local collection (vanilla accessor set)
// ---------------------------------------------------------------------------

// SystemTimeWork.GetStartTime returns a packed 12-byte value; the card's
// startTime is bytes 4..11 (per the vanilla site: auVar8._4_8_).
struct __attribute__((packed)) StartTime12 {
    int32_t head;
    int64_t time;
};
static_assert(sizeof(StartTime12) == 12, "GetStartTime return size");

static void collectLocalCardBlob(CardBlob* b) {
    memset(b, 0, sizeof(*b));

    auto* status = PlayerWork::get_playerStatus();

    b->fashion   = (uint8_t)PlayerWork::get_playerFashion();
    b->colorId   = (uint8_t)_ILExternal::external<int32_t>(RVA_PW_GET_COLOR_ID);
    b->bodyType  = status ? status->fields.body_type : 0;
    b->genderid  = status ? (uint8_t)(*(uint8_t*)((uintptr_t)status + MYSTATUS_SEX_OFFSET) ^ 1) : 0;
    b->langId    = (uint8_t)PlayerWork::get_msgLangID();
    b->cardRank  = (uint8_t)_ILExternal::external<int32_t>(RVA_PW_GET_TRAINER_RANK);
    b->trainerId = status ? *(uint32_t*)((uintptr_t)status + MYSTATUS_TRAINER_ID_OFFSET) : 0;
    b->money     = PlayerWork::GetMoney();

    bool zenkoku  = _ILExternal::external<bool>(RVA_ZUKAN_ZENKOKU_FLAG);
    b->hasZenkoku = zenkoku ? 1 : 0;
    b->zukanSee   = zenkoku ? _ILExternal::external<int32_t>(RVA_ZUKAN_SEE_COUNT, 0)
                            : _ILExternal::external<int32_t>(RVA_ZUKAN_SEE_SINOU, 0);

    auto* playerData = PlayerWork::get_playerData();
    if (playerData != nullptr) {
        b->playHour = *(uint16_t*)((uintptr_t)playerData + 0x70);
        b->playMin  = *(uint8_t*)((uintptr_t)playerData + 0x72);
    }

    b->startTime = _ILExternal::external<StartTime12>(RVA_SYSTIME_GET_START).time;
    b->clearTime = _ILExternal::external<uint32_t>(RVA_RECORDWORK_GET, REC_CLEAR_TIME);

    for (int i = 0; i < 5; i++)
        b->contest[i] = _ILExternal::external<uint32_t>(RVA_RECORDWORK_GET, REC_CONTEST_FIRST + i);
    b->renshou[0] = _ILExternal::external<uint32_t>(RVA_RECORDWORK_GET, REC_RENSHOU_SINGLE);
    b->renshou[1] = _ILExternal::external<uint32_t>(RVA_RECORDWORK_GET, REC_RENSHOU_DOUBLE);
    b->renshou[2] = _ILExternal::external<uint32_t>(RVA_RECORDWORK_GET, REC_RENSHOU_MSINGLE);
    b->renshou[3] = _ILExternal::external<uint32_t>(RVA_RECORDWORK_GET, REC_RENSHOU_MDOUBLE);

    // PoffinSaveData is returned by value: { Poffins array, CookingCount } —
    // CookingCount is the card's "Poffins cooked" stat.
    b->poffinCount = (uint8_t)PlayerWork::get_poffinSaveData().fields.CookingCount;

    // Dig fossil play count = first u16 of UgCountRecord
    void* ugRec = _ILExternal::external<void*>(RVA_PW_GET_UG_COUNT_REC);
    if (ugRec != nullptr) {
        b->fossilCount = *(int16_t*)ugRec;
    }
    b->statueKinds = (uint8_t)_ILExternal::external<int32_t>(RVA_UGITEM_STATUE_KINDS);
}

// ---------------------------------------------------------------------------
// Packets
// ---------------------------------------------------------------------------

void mpTrainerCardRequest(int32_t targetStation) {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_CARD_REQUEST);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[TrainerCard] Requested card from station %d\n", targetStation);
}

static void sendCardData(int32_t targetStation) {
    CardBlob blob;
    collectLocalCardBlob(&blob);

    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_CARD_DATA);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    const uint8_t* raw = (const uint8_t*)&blob;
    for (size_t i = 0; i < sizeof(CardBlob); i++)
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, raw[i]);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[TrainerCard] Sent card data to station %d (%zu bytes)\n",
                targetStation, sizeof(CardBlob));
}

void mpTrainerCardOnRequestReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    if (target != mpThisStationIndex()) return;
    if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

    // Cards are public info in vanilla comm modes — respond without a prompt.
    sendCardData(fromStation);
}

void mpTrainerCardOnDataReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    if (target != mpThisStationIndex()) return;
    if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

    uint8_t* raw = (uint8_t*)&s_pendingBlob;
    for (size_t i = 0; i < sizeof(CardBlob); i++)
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &raw[i]);

    // Range sanity before display (distrust posture)
    if (s_pendingBlob.money < 0 || s_pendingBlob.money > 9999999 ||
        s_pendingBlob.zukanSee < 0 || s_pendingBlob.zukanSee > 1000) {
        MP_LOG("[TrainerCard] Rejected implausible card blob from station %d\n", fromStation);
        return;
    }

    s_cardFromStation = fromStation;
    s_cardPending = true;
    MP_LOG("[TrainerCard] Card data from station %d — opening on next tick\n", fromStation);
}

// ---------------------------------------------------------------------------
// Card window (vanilla UnionRoomManager.OpenTargetTranerCard recipe)
// ---------------------------------------------------------------------------

static Il2CppClass* loadTypeInfo(uintptr_t ramOffset) {
    return *(Il2CppClass**)exl::util::modules::GetTargetOffset(ramOffset);
}

static void openReceivedCard() {
    auto& ctx = getOverworldMPContext();
    int32_t station = s_cardFromStation;
    const CardBlob& b = s_pendingBlob;

    // Load the metadata set vanilla's card path uses (idempotent).
    system_load_typeinfo(TYPEINFO_TOKEN_CARD_PATH);

    Il2CppClass* nameKlass  = loadTypeInfo(RAM_PLAYERNAME_TYPEINFO);
    Il2CppClass* paramKlass = loadTypeInfo(RAM_UICARD_PARAM_TYPEINFO);
    if (nameKlass == nullptr || paramKlass == nullptr) {
        MP_LOG("[TrainerCard] TypeInfo not loaded (name=%p param=%p) — aborting\n",
                    nameKlass, paramKlass);
        return;
    }

    // PlayerNameData { nickName, genderid, languageId }
    void* nameData = il2cpp_object_new(nameKlass);
    _ILExternal::external<void>(RVA_PLAYERNAME_CTOR, nameData);
    const char* peerName = (station >= 0 && ctx.remotePlayers[station].playerNameSet)
                               ? ctx.remotePlayers[station].playerNameBuf
                               : "Player";
    *(void**)((uintptr_t)nameData + NAMEDATA_NICKNAME) = System::String::Create(peerName);
    *(int32_t*)((uintptr_t)nameData + NAMEDATA_GENDER) = b.genderid;
    *(int32_t*)((uintptr_t)nameData + NAMEDATA_LANG)   = b.langId;

    // UICard.Param — field mapping mirrors OpenTargetTranerCard exactly.
    void* param = il2cpp_object_new(paramKlass);
    _ILExternal::external<void>(RVA_UICARD_PARAM_CTOR, param);
    uintptr_t p = (uintptr_t)param;
    *(uint8_t*)(p + PARAM_IS_SHOW_BADGE) = 0;              // badge case: local-only system
    *(int32_t*)(p + PARAM_CARD_RANK)     = b.cardRank;
    *(uint8_t*)(p + PARAM_FASHION)       = b.fashion;
    *(uint8_t*)(p + PARAM_BODY_TYPE)     = b.bodyType;
    *(uint8_t*)(p + PARAM_SEX)           = (b.genderid != 1) ? 1 : 0;
    *(uint32_t*)(p + PARAM_PLAYER_ID)    = b.trainerId;
    *(void**)(p + PARAM_NAME_DATA)       = nameData;
    *(int32_t*)(p + PARAM_MONEY)         = b.money;
    *(uint8_t*)(p + PARAM_IS_GET_ZUKAN)  = b.hasZenkoku;
    *(int32_t*)(p + PARAM_ZUKAN_SEE)     = b.zukanSee;
    *(uint16_t*)(p + PARAM_PLAY_HOUR)    = b.playHour;
    *(uint16_t*)(p + PARAM_PLAY_MIN)     = b.playMin;
    *(int64_t*)(p + PARAM_START_TIME)    = b.startTime;
    *(uint32_t*)(p + PARAM_CLEAR_TIME)   = b.clearTime;
    for (int i = 0; i < 5; i++)
        *(uint32_t*)(p + PARAM_CONTEST_FIRST + i * 4) = b.contest[i];
    for (int i = 0; i < 4; i++)
        *(uint32_t*)(p + PARAM_RENSHOU_FIRST + i * 4) = b.renshou[i];
    *(int32_t*)(p + PARAM_POFFIN_COUNT)  = b.poffinCount;
    *(int16_t*)(p + PARAM_FOSSIL_COUNT)  = b.fossilCount;
    *(int32_t*)(p + PARAM_STATUE_KINDS)  = b.statueKinds;

    auto* window = Dpr::UI::UIManager::get_Instance()
                       ->CreateUIWindow<Dpr::UI::UICard>((UIWindowID)4);
    if (window == nullptr) {
        MP_LOG("[TrainerCard] CreateUIWindow<UICard> failed\n");
        return;
    }
    // Arm the model override BEFORE Open: the OpOpen coroutine loads the 3D
    // player model with LOCAL PlayerWork appearance; the LoadModels hook below
    // substitutes the peer's fashion/color/sex for this one card.
    s_modelOverrideArmed = true;
    window->Open(param, -2);
    MP_LOG("[TrainerCard] Opened card for station %d (%s)\n", station, peerName);
}

// ---------------------------------------------------------------------------
// Peer appearance on the card's 3D model
// ---------------------------------------------------------------------------
// CardModelViewController.LoadModels(this, byte fashion, byte bodyType, bool sex)
// @0x1A31010 builds the model from explicit appearance args — but its caller
// always passes the LOCAL player's values, and the model's colors resolve via
// PlayerWork.GetColorID (hooked to return the LOCAL player). While a peer card
// is opening we (a) substitute the peer's fashion/bodyType/sex for the model
// asset and (b) arm owmpArmCardColor so the model's ColorVariation renders the
// peer's color instead of ours. Note: LoadModels has NO colorId parameter —
// color is a property of the ColorVariation component, applied post-load.
extern void owmpArmCardColor(int32_t colorId, const RomData::ColorSet* customSet);

// Build a ColorSet from a peer's synced 18-float field + battle color arrays
// (6 colors × RGB each), mirroring the battle-slot construction in team_up.cpp.
static void buildPeerColorSet(RomData::ColorSet& cs, const float* fieldColors, const float* battleColors) {
    float* fieldBase = &cs.fieldSkinFace.r;   // 6 Color structs, contiguous
    for (int c = 0; c < 6; c++) {
        fieldBase[c * 4 + 0] = fieldColors[c * 3 + 0];
        fieldBase[c * 4 + 1] = fieldColors[c * 3 + 1];
        fieldBase[c * 4 + 2] = fieldColors[c * 3 + 2];
        fieldBase[c * 4 + 3] = 1.0f;
    }
    float* battleBase = &cs.battleSkinFace.r;
    for (int c = 0; c < 6; c++) {
        battleBase[c * 4 + 0] = battleColors[c * 3 + 0];
        battleBase[c * 4 + 1] = battleColors[c * 3 + 1];
        battleBase[c * 4 + 2] = battleColors[c * 3 + 2];
        battleBase[c * 4 + 3] = 1.0f;
    }
}

HOOK_DEFINE_TRAMPOLINE(CardModelViewController$$LoadModels) {
    static void Callback(void* __this, uint8_t fashion, uint8_t bodyType, uint8_t sex) {
        if (s_modelOverrideArmed) {
            s_modelOverrideArmed = false;
            uint8_t peerSex = (uint8_t)(s_pendingBlob.genderid ^ 1);  // undo the wire inversion

            // Arm the peer's color for the model's ColorVariation (applied on
            // its OnEnable, post async load).
            if (s_pendingBlob.colorId == 0xFF) {
                // Custom colors — pull the peer's synced palette from their
                // FieldPlayerNetData (present since the card can only be viewed
                // during an active MP session).
                RomData::ColorSet peerSet = {};
                bool haveCustom = false;
                if (s_cardFromStation >= 0 && s_cardFromStation < OW_MP_MAX_PLAYERS) {
                    auto& remote = getOverworldMPContext().remotePlayers[s_cardFromStation];
                    if (remote.hasCustomColors) {
                        buildPeerColorSet(peerSet, remote.customFieldColors, remote.customBattleColors);
                        haveCustom = true;
                    }
                }
                if (haveCustom) {
                    owmpArmCardColor(-1, &peerSet);
                } else {
                    owmpArmCardColor(0, nullptr);  // no palette received — safe default
                }
            } else {
                owmpArmCardColor((int32_t)s_pendingBlob.colorId, nullptr);
            }

            MP_LOG("[TrainerCard] Model override: fashion %d->%d bodyType %d->%d sex %d->%d colorId=%d\n",
                        (int)fashion, (int)s_pendingBlob.fashion,
                        (int)bodyType, (int)s_pendingBlob.bodyType,
                        (int)sex, (int)peerSex, (int)s_pendingBlob.colorId);
            Orig(__this, s_pendingBlob.fashion, s_pendingBlob.bodyType, peerSex);
            return;
        }
        Orig(__this, fashion, bodyType, sex);
    }
};

// Safety: if the card closes before its model's ColorVariation ever enabled
// (load aborted), clear a still-armed peer color so it can't leak onto the next
// unrelated ColorVariation (e.g. an overworld NPC).
extern void owmpDisarmCardColor();
HOOK_DEFINE_TRAMPOLINE(CardModelViewController$$Dispose) {
    static void Callback(void* __this) {
        owmpDisarmCardColor();
        Orig(__this);
    }
};

void exl_mp_trainer_card_hooks() {
    // Bisect result (2026-07-05): these were disabled to test the team-up sync
    // black-screen; the hang reproduced with them off, so they are NOT the
    // cause — re-enabled.
    CardModelViewController$$LoadModels::InstallAtOffset(0x1A31010);
    CardModelViewController$$Dispose::InstallAtOffset(0x1A308A0);
    MP_LOG("[TrainerCard] LoadModels + Dispose hooks installed\n");
}

void mpTrainerCardTick(float deltaTime) {
    (void)deltaTime;
    if (!s_cardPending) return;
    s_cardPending = false;
    openReceivedCard();
}
