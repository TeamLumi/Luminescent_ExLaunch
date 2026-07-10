#include "exlaunch.hpp"

#include "externals/EntityManager.h"
#include "externals/FieldManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FieldPokemonEntity.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/GameManager.h"
#include "externals/MonsLv.h"
#include "externals/PlayerWork.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/EncountDataWork.h"
#include "externals/FlagWork.h"
#include "externals/UnityEngine/Time.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/XLSXContent/FieldEncountTable.h"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Field/EncountResult.h"
#include "externals/Dpr/Field/FieldEncount.h"
#include "externals/Dpr/SubContents/Utils.h"
#include "externals/RandomGroupWork.h"

#include "externals/Audio/AudioManager.h"
#include "externals/GameData/DataManager.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/AnimationPlayer.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/Personal/SexVector.h"
#include "externals/Pml/Sex.h"
#include "externals/Dpr/FureaiHiroba/PokeFactory.h"
#include "externals/System/Action.h"
#include "externals/System/String.h"
#include "externals/XLSXContent/PersonalTable.h"
#include "externals/XLSXContent/PokemonInfo.h"

#include "externals/AttributeID.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "externals/UnityEngine/Collider.h"
#include "externals/FlagWork_Enums.h"

#include "utils/utils.h"
#include "save/save.h"

#include "logger/logger.h"
#include "exlaunch/nx/kernel/svc.h"

static constexpr int MAX_SYMBOL_POKEMON = 14;
// Core 328 + calc 16, matches Accessor::Serialize_FullData.
static constexpr int SYMBOL_POKE_FULL_DATA_SIZE = 344;
static constexpr int MAX_LAND_POKEMON = 7;
static constexpr int MAX_WATER_POKEMON = 7;
static constexpr float PROXIMITY_THRESHOLD_SQ = 1.0f;
static constexpr float GROW_DURATION = 0.5f;
static constexpr float SHRINK_DURATION = 0.5f;
static constexpr float MIN_LIFETIME = 45.0f;
static constexpr float MAX_LIFETIME = 60.0f;
static constexpr int MAX_SPAWN_ATTEMPTS = 15;
static constexpr int SPAWN_RING_MIN_DIST = 4;
static constexpr int SPAWN_RING_MAX_DIST = 28;
static constexpr int RECYCLE_DIST_SQ = 3025;  // 55 tiles
static constexpr float WANDER_MIN_INTERVAL = 3.0f;
static constexpr float WANDER_MAX_INTERVAL = 5.0f;
static constexpr float WANDER_RANGE = 5.0f;
static constexpr float RESPAWN_DELAY = 1.0f;

static constexpr float SHINY_SPARKLE_INTERVAL  = 2.5f;
static constexpr int32_t SHINY_SPARKLE_EFFECT_ID = 14;

static constexpr uint32_t SHINY_SE_PRIMARY  = 1995660494;  // PLAY_BA_SYS_RARE_EFFECT
static constexpr uint32_t SHINY_SE_FALLBACK = 2502241626;  // UI_CARD_BADGE_KIRAN

static constexpr float WALK_SPEED_MIN = 0.3f;
static constexpr float WALK_SPEED_MAX = 0.66f;
static constexpr float RUN_SPEED_MIN  = 0.7f;
static constexpr float RUN_SPEED_MAX  = 1.32f;
static constexpr float RUN_THRESHOLD  = 0.6f;

// UnityEngine.Random is shared with trainer AI and NPC behavior, stay off it.

static uint32_t s_rngState = 0x12345678;

static uint32_t Xorshift32() {
    uint32_t x = s_rngState;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    s_rngState = x;
    return x;
}

static int32_t RandRange(int32_t min, int32_t max) {
    if (min >= max) return min;
    return min + (int32_t)(Xorshift32() % (uint32_t)(max - min));
}

static float RandRange(float min, float max) {
    if (min >= max) return min;
    float t = (float)(Xorshift32() & 0xFFFFFF) / 16777216.0f;
    return min + t * (max - min);
}

static bool SafeGetTileAttribute(UnityEngine::Vector2Int::Object tile,
                                  XLSXContent::MapAttributeTable::SheetData::Object** outAttr) {
    int32_t code = 0;
    int32_t stop = 0;
    GameManager::GetAttribute(tile, &code, &stop, false);
    auto* attr = GameManager::GetAttributeTable(code);
    if (attr == nullptr) return false;
    if (outAttr) *outAttr = attr;
    return true;
}

static bool SafeDoesTileGiveEncounters(UnityEngine::Vector2Int::Object tile) {
    XLSXContent::MapAttributeTable::SheetData::Object* attr = nullptr;
    if (!SafeGetTileAttribute(tile, &attr)) return false;
    return attr->fields.Encount;
}

static bool SafeIsTileAWaterTile(UnityEngine::Vector2Int::Object tile) {
    XLSXContent::MapAttributeTable::SheetData::Object* attr = nullptr;
    if (!SafeGetTileAttribute(tile, &attr)) return false;
    return AttributeID::MATR_IsWater(attr->fields.Code);
}

// Water tiles also check cardinal neighbors to keep mons off shore walls.
static bool IsTileSafeForPokemon(int32_t gridX, int32_t gridZ) {
    UnityEngine::Vector2Int::Object center;
    center.fields.m_X = gridX;
    center.fields.m_Y = gridZ;
    if (!SafeDoesTileGiveEncounters(center)) return false;

    if (SafeIsTileAWaterTile(center)) {
        static const int32_t cardinals[][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
        for (auto& c : cardinals) {
            UnityEngine::Vector2Int::Object nb;
            nb.fields.m_X = gridX + c[0];
            nb.fields.m_Y = gridZ + c[1];
            if (!SafeDoesTileGiveEncounters(nb)) return false;
        }
    }
    return true;
}

static bool IsWorldPosOnSafeTile(float worldX, float worldZ) {
    int32_t gridX = -(int32_t)worldX;
    int32_t gridZ = (int32_t)worldZ;
    return IsTileSafeForPokemon(gridX, gridZ);
}

enum class SymbolState : uint8_t {
    EMPTY,
    LOADING,
    SPAWNED,
    DESPAWNING,
    COOLDOWN,
};

struct SymbolPokemon {
    SymbolState state;
    int32_t monsNo;
    int16_t formNo;
    int32_t level;
    bool isRare;
    int8_t sex;
    int32_t catalogId;
    bool isWaterTile;
    bool isPersistentShiny;     // true = from persistent save, skip lifetime
    int32_t persistentShinyIdx; // index in SymbolEncountersSaveData::shinies, or -1

    int32_t spawnGridX;
    int32_t spawnGridZ;

    float spawnWorldX;
    float spawnWorldY;
    float spawnWorldZ;

    UnityEngine::GameObject::Object* gameObject;
    FieldPokemonEntity::Object* entity;
    Pml::PokePara::PokemonParam::Object* pokemonParam;

    float lifetimeTimer;
    float lifetimeDuration;
    float scaleProgress;
    float cooldownTimer;
    float sparkleTimer;         // shiny-only: periodic hidden-item-style glint

    float targetScaleX;
    float targetScaleY;
    float targetScaleZ;

    float wanderTimer;
    float wanderTargetX;
    float wanderTargetZ;
    float moveSpeed;
    int32_t currentAnimIndex;

    // Rebuilt from bytes at battle time, the IL2CPP GC can relocate managed params.
    bool hasFullData;
    uint8_t fullData[SYMBOL_POKE_FULL_DATA_SIZE];
};

static SymbolPokemon s_symbolPokemon[MAX_SYMBOL_POKEMON];
static int32_t s_currentAreaID = -1;
static int32_t s_currentZoneID = -1;
static bool s_initialized = false;

bool g_symbolEncountersActive = false;

static float s_spawnDelay = 0.0f;
static constexpr float AREA_SPAWN_DELAY = 1.0f;

static bool s_loadPending[MAX_SYMBOL_POKEMON] = {};
static Il2CppObject* s_loadedPrefab[MAX_SYMBOL_POKEMON] = {};
static int s_currentlyLoadingSlot = -1;  // which slot is actively loading (-1 = none)

static MethodInfo* s_onAssetLoadedMI = nullptr;

static void OnUpdate();
static void DespawnSingle(int index);
static void CleanupAll();
static bool TrySelectSpecies(int index, bool isWaterTile);
static bool TryFindSpawnPosition(int index, int32_t playerGridX, int32_t playerGridZ, float playerY);
static void BeginLoading(int index);
static void FinishSpawning(int index);
static void OnAssetLoaded(Il2CppObject* loadedObject);
static void SpawnPersistentShiniesForZone(int32_t zoneID);
static void PlayShinySe();

static FieldManager::Object* GetFieldManagerInstance() {
    auto klass = FieldManager::getClass();
    if (klass == nullptr) return nullptr;
    auto sf = klass->static_fields;
    if (sf == nullptr) return nullptr;
    return sf->_Instance_k__BackingField;
}

static FieldPlayerEntity::Object* GetPlayerEntity() {
    auto klass = EntityManager::getClass();
    if (klass == nullptr) return nullptr;
    if (klass->static_fields == nullptr) return nullptr;
    return klass->static_fields->_activeFieldPlayer_k__BackingField;
}

static bool IsInField() {
    auto fm = GetFieldManagerInstance();
    if (fm == nullptr) return false;
    if (fm->fields._IsMenuOpen_k__BackingField) return false;
    auto player = GetPlayerEntity();
    if (player == nullptr) return false;
    return true;
}

static void PlayShinySe() {
    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    auto* audioManager = Audio::AudioManager::get_Instance();
    if (audioManager != nullptr) {
        auto* inst = audioManager->PlaySe(SHINY_SE_PRIMARY, nullptr);
        if (inst == nullptr) {
            inst = audioManager->PlaySe(SHINY_SE_FALLBACK, nullptr);
            Logger::log("[SymbolEnc] Shiny SE: primary unavailable, fallback %s\n",
                        inst != nullptr ? "played" : "ALSO failed");
        } else {
            Logger::log("[SymbolEnc] Shiny SE: primary (BA_SYS_RARE_EFFECT) played\n");
        }
    }
}

static bool TrySelectSpecies(int index, bool isWaterTile) {
    Logger::log("[SymbolEnc] TrySelectSpecies(%d, water=%d) starting\n", index, isWaterTile);

    system_load_typeinfo(0x48c1);
    system_load_typeinfo(0x6ae9);
    system_load_typeinfo(0x48c9);
    system_load_typeinfo(0x48c7);
    system_load_typeinfo(0x48c6);

    XLSXContent::FieldEncountTable::Sheettable::Object* fieldEnc = GetFieldEncountersOfCurrentZoneID();
    if (fieldEnc == nullptr) {
        Logger::log("[SymbolEnc] TrySelectSpecies: fieldEnc is null\n");
        return false;
    }

    Pml::PokeParty::Object* party = PlayerWork::get_playerParty();
    if (party == nullptr || party->fields.m_memberCount == 0) return false;
    auto* firstPokemon = (Pml::PokePara::PokemonParam::Object*)party->GetMemberPointer(0);

    Dpr::Field::FieldEncount::ENC_FLD_SPA::Object spaStruct{};
    Dpr::Field::FieldEncount::SetSpaStruct(firstPokemon, fieldEnc, &spaStruct);

    auto* encounterHolder = Dpr::Field::EncountResult::newInstance();
    auto* slots = MonsLv::newArray(12);
    Logger::log("[SymbolEnc] TrySelectSpecies: slots=%p encounterHolder=%p\n", slots, encounterHolder);

    if (isWaterTile) {
        if (fieldEnc->fields.water_mons == nullptr || fieldEnc->fields.water_mons->max_length == 0) return false;
        SetWaterSlots(&encounterHolder, slots);
    } else {
        if (!BuildGroundEncounterSlots(&encounterHolder, slots)) return false;
    }

    bool success = Dpr::Field::FieldEncount::SetEncountData(
        firstPokemon, 0, spaStruct, slots, isWaterTile ? 1 : 0, 1, &encounterHolder);
    if (!success) {
        Logger::log("[SymbolEnc] SetEncountData failed\n");
        return false;
    }

    Dpr::Field::FieldEncount::LastProc(&encounterHolder, &spaStruct);

    int32_t monsNo = encounterHolder->fields.Enemy->m_Items[0];
    int32_t level = encounterHolder->fields.Level->m_Items[0];
    int32_t formNo = encounterHolder->fields.karanaForm;
    if (monsNo == 0) return false;

    // Generate through InitialSpec like a real grass encounter (Shiny Rates hooks
    // its rareTryCount; the field generator never sets EncountResult.IsRare).
    auto* spec = (Pml::PokePara::InitialSpec::Object*)il2cpp_object_new(
        (Il2CppClass*)Pml::PokePara::InitialSpec::getClass());
    spec->ctor();  // <-- Shiny Rates ctor hook inflates rareTryCount here
    spec->fields.monsno = monsNo;
    spec->fields.formno = (uint16_t)formNo;
    spec->fields.level  = (uint16_t)level;

    auto* genParam = (Pml::PokePara::PokemonParam::Object*)il2cpp_object_new(
        (Il2CppClass*)Pml::PokePara::PokemonParam::getClass());
    genParam->ctor(spec);
    auto* genCore = (Pml::PokePara::CoreParam::Object*)genParam;

    bool isRare = genCore->IsRare();     // the genuine (base-Lumi-boosted) shiny roll

    genCore->fields.m_accessor->Serialize_FullData(s_symbolPokemon[index].fullData);
    s_symbolPokemon[index].hasFullData = true;

    s_symbolPokemon[index].monsNo = monsNo;
    s_symbolPokemon[index].formNo = (int16_t)formNo;
    s_symbolPokemon[index].level = level;
    s_symbolPokemon[index].isRare = isRare;
    s_symbolPokemon[index].sex = (int8_t)genCore->GetSex();  // InitialSpec rolled per gender ratio
    s_symbolPokemon[index].isPersistentShiny = false;
    s_symbolPokemon[index].persistentShinyIdx = -1;

    if (isRare) {
        int32_t zoneId = PlayerWork::get_zoneID();
        auto* saveData = getCustomSaveData();
        // AddShiny evicts index 0 when full; re-align live slots' save indices.
        bool willEvict = (saveData->symbolEncounters.count >= MAX_PERSISTENT_SHINIES);
        saveData->symbolEncounters.AddShiny(
            monsNo, (int16_t)formNo, level, s_symbolPokemon[index].sex,
            zoneId, s_symbolPokemon[index].spawnGridX, s_symbolPokemon[index].spawnGridZ,
            s_symbolPokemon[index].fullData);  // exact mon survives save/reload
        if (willEvict) {
            for (int j = 0; j < MAX_SYMBOL_POKEMON; j++) {
                if (j == index) continue;
                auto& other = s_symbolPokemon[j];
                if (!other.isPersistentShiny) continue;
                if (other.persistentShinyIdx == 0) {
                    // Its save entry was dropped, now a normal despawnable symbol.
                    other.isPersistentShiny = false;
                    other.persistentShinyIdx = -1;
                } else if (other.persistentShinyIdx > 0) {
                    other.persistentShinyIdx--;
                }
            }
        }
        s_symbolPokemon[index].isPersistentShiny = true;
        s_symbolPokemon[index].persistentShinyIdx = saveData->symbolEncounters.count - 1;
    }

    Logger::log("[SymbolEnc] Generated: monsNo=%d form=%d level=%d rare=%d%s\n",
        monsNo, formNo, level, isRare, isRare ? " [PERSISTED]" : "");
    return true;
}

static bool TryFindSpawnPosition(int index, int32_t playerGridX, int32_t playerGridZ, float playerY) {
    for (int attempt = 0; attempt < MAX_SPAWN_ATTEMPTS; attempt++) {
        // Radius-uniform ring sample: pick a distance, then a point on that
        // ring (dx uniform, |dz| from the circle equation, random sign).
        int32_t r = RandRange(SPAWN_RING_MIN_DIST, SPAWN_RING_MAX_DIST + 1);
        int32_t dx = RandRange(-r, r + 1);
        int32_t dz = (int32_t)__builtin_sqrtf((float)(r * r - dx * dx));
        if (RandRange(0, 2) == 0) dz = -dz;

        int32_t gridX = playerGridX + dx;
        int32_t gridZ = playerGridZ + dz;

        if (!IsTileSafeForPokemon(gridX, gridZ)) continue;

        bool occupied = false;
        for (int j = 0; j < MAX_SYMBOL_POKEMON; j++) {
            if (j == index) continue;
            if (s_symbolPokemon[j].state == SymbolState::SPAWNED ||
                s_symbolPokemon[j].state == SymbolState::LOADING) {
                if (s_symbolPokemon[j].spawnGridX == gridX &&
                    s_symbolPokemon[j].spawnGridZ == gridZ) {
                    occupied = true;
                    break;
                }
            }
        }
        if (occupied) continue;

        s_symbolPokemon[index].spawnGridX = gridX;
        s_symbolPokemon[index].spawnGridZ = gridZ;
        s_symbolPokemon[index].isWaterTile = SafeIsTileAWaterTile(
            (UnityEngine::Vector2Int::Object){ .fields = { .m_X = gridX, .m_Y = gridZ } });
        return true;
    }
    return false;
}

static void OnAssetLoaded(Il2CppObject* loadedObject) {
    Logger::log("[SymbolEnc] OnAssetLoaded callback fired, obj=%p\n", loadedObject);

    int slot = s_currentlyLoadingSlot;
    if (slot >= 0 && slot < MAX_SYMBOL_POKEMON && s_loadPending[slot]) {
        s_loadPending[slot] = false;
        s_loadedPrefab[slot] = loadedObject;
        s_currentlyLoadingSlot = -1;
        Logger::log("[SymbolEnc] Asset loaded for slot %d\n", slot);
        return;
    }
    s_currentlyLoadingSlot = -1;
    Logger::log("[SymbolEnc] WARNING: OnAssetLoaded but no pending slot (orphan)\n");
}

static void BeginLoading(int index) {
    // One async load at a time; the callback assigns by slot.
    if (s_currentlyLoadingSlot >= 0) {
        Logger::log("[SymbolEnc] Skipping load for slot %d, slot %d still loading\n", index, s_currentlyLoadingSlot);
        return;
    }

    auto& poke = s_symbolPokemon[index];
    poke.state = SymbolState::LOADING;

    Logger::log("[SymbolEnc] Loading slot %d: monsNo=%d form=%d shiny=%d (isRare feeds GetPokemonCatalog -> shiny bundle)\n",
                index, poke.monsNo, poke.formNo, (int)poke.isRare);

    poke.pokemonParam = Pml::PokePara::PokemonParam::newInstance(poke.monsNo, (uint16_t)poke.level, (uint64_t)0);

    GameData::DataManager::getClass()->initIfNeeded();
    XLSXContent::PokemonInfo::SheetCatalog::Object* catalog =
        GameData::DataManager::GetPokemonCatalog(
            poke.monsNo, poke.formNo, (Pml::Sex)poke.sex, poke.isRare, false);

    if (catalog == nullptr) {
        Logger::log("[SymbolEnc] Failed to get catalog for monsNo=%d\n", poke.monsNo);
        poke.pokemonParam = nullptr;
        poke.state = SymbolState::EMPTY;
        return;
    }

    auto* assetBundleName = catalog->fields.AssetBundleName;

    Dpr::SubContents::Utils::getClass()->initIfNeeded();
    auto* fullPath = Dpr::SubContents::Utils::GetPokemonAssetbundleName(assetBundleName);

    s_loadPending[index] = true;
    s_loadedPrefab[index] = nullptr;
    s_currentlyLoadingSlot = index;

    if (s_onAssetLoadedMI == nullptr) {
        s_onAssetLoadedMI = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_onAssetLoadedMI, 0, sizeof(MethodInfo));
        s_onAssetLoadedMI->methodPointer = (Il2CppMethodPointer)&OnAssetLoaded;
        s_onAssetLoadedMI->flags = 0x0010;
        s_onAssetLoadedMI->parameters_count = 1;
    }

    auto* actionClass = System::Action::getClass(System::Action::void_TypeInfo);
    if (s_onAssetLoadedMI->klass == nullptr) {
        s_onAssetLoadedMI->klass = (Il2CppClass*)actionClass;
    }
    auto* action = (System::Action::Object*)il2cpp_object_new((Il2CppClass*)actionClass);
    action->ctor(nullptr, s_onAssetLoadedMI);

    Logger::log("[SymbolEnc] BeginLoading: starting async load...\n");
    SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
    auto* coroutine = Dpr::SubContents::Utils::LoadAsset(fullPath, action);
    SmartPoint::AssetAssistant::Sequencer::Start(coroutine);

    Logger::log("[SymbolEnc] Asset load started for slot %d\n", index);
}

static void FinishSpawning(int index) {
    auto& poke = s_symbolPokemon[index];

    if (s_loadedPrefab[index] == nullptr) {
        return; // still loading
    }

    Il2CppObject* loadedAsset = s_loadedPrefab[index];
    s_loadedPrefab[index] = nullptr;

    Logger::log("[SymbolEnc] FinishSpawning slot %d: asset=%p\n", index, loadedAsset);

    // Instantiate a copy; Destroy on the original corrupts the asset cache and
    // SetActive corrupts prefab state.
    auto* instantiated = UnityEngine::_Object::Instantiate<UnityEngine::_Object>((UnityEngine::_Object*)loadedAsset);
    if (instantiated == nullptr) {
        Logger::log("[SymbolEnc] Instantiate returned null for slot %d\n", index);
        poke.state = SymbolState::EMPTY;
        return;
    }

    poke.gameObject = (UnityEngine::GameObject::Object*)instantiated;
    poke.gameObject->SetActive(true);

    auto player = GetPlayerEntity();
    float playerY = 0.0f;
    if (player != nullptr) {
        playerY = ((BaseEntity::Object*)player)->fields.worldPosition.fields.y;
    }

    float worldX = (float)(-poke.spawnGridX);
    float worldZ = (float)(poke.spawnGridZ);
    float worldY = playerY;

    UnityEngine::Transform::Object* transform = poke.gameObject->get_transform();

    UnityEngine::Vector3::Object pos;
    pos.fields.x = worldX;
    pos.fields.y = worldY;
    pos.fields.z = worldZ;
    transform->set_position(pos);

    UnityEngine::Vector3::Object euler;
    euler.fields.x = 0.0f;
    euler.fields.y = RandRange(0.0f, 360.0f);
    euler.fields.z = 0.0f;
    transform->set_localEulerAngles(euler);

    Dpr::FureaiHiroba::PokeFactory::SetPokeScale(transform, poke.pokemonParam);

    UnityEngine::Vector3::Object savedScale = transform->get_localScale();
    poke.targetScaleX = savedScale.fields.x;
    poke.targetScaleY = savedScale.fields.y;
    poke.targetScaleZ = savedScale.fields.z;

    UnityEngine::Vector3::Object zeroScale;
    zeroScale.fields.x = 0.0f;
    zeroScale.fields.y = 0.0f;
    zeroScale.fields.z = 0.0f;
    transform->set_localScale(zeroScale);

    poke.entity = poke.gameObject->GetComponent<FieldPokemonEntity>(FieldPokemonEntity::getClass());
    if (poke.entity != nullptr) {
        poke.entity->fields.updateEnable = true;

        auto* animPlayer = (AnimationPlayer::Object*)poke.entity->fields._animationPlayer;
        if (animPlayer != nullptr) {
            animPlayer->Play(FieldPokemonEntity::ANIM_IDLE, 0.0f, 0.0f);
            poke.currentAnimIndex = FieldPokemonEntity::ANIM_IDLE;
        }
    }

    // No colliders, mons must not block the player.
    {
        auto* colliderClass = UnityEngine::Collider::getClass();
        if (colliderClass != nullptr) {
            auto* colliders = poke.gameObject->GetComponentsInternal<UnityEngine::Collider>(
                colliderClass, true);
            if (colliders != nullptr) {
                for (uint32_t c = 0; c < colliders->max_length; c++) {
                    if (colliders->m_Items[c] != nullptr) {
                        UnityEngine::_Object::Destroy((UnityEngine::_Object::Object*)colliders->m_Items[c]);
                    }
                }
                Logger::log("[SymbolEnc] Destroyed %d colliders on slot %d\n", colliders->max_length, index);
            }
        } else {
            Logger::log("[SymbolEnc] WARNING: Collider class not initialized, skipping collider removal\n");
        }
    }

    if (RandRange(0, 2) == 0) {
        poke.moveSpeed = RandRange(WALK_SPEED_MIN, WALK_SPEED_MAX);
    } else {
        poke.moveSpeed = RandRange(RUN_SPEED_MIN, RUN_SPEED_MAX);
    }

    poke.spawnWorldX = worldX;
    poke.spawnWorldY = worldY;
    poke.spawnWorldZ = worldZ;

    poke.wanderTimer = RandRange(0.5f, 2.0f);
    poke.wanderTargetX = worldX;
    poke.wanderTargetZ = worldZ;

    poke.lifetimeDuration = RandRange(MIN_LIFETIME, MAX_LIFETIME);
    poke.lifetimeTimer = 0.0f;
    poke.scaleProgress = 0.0f;
    poke.sparkleTimer = SHINY_SPARKLE_INTERVAL;  // glint on the first frame after grow-in

    poke.state = SymbolState::SPAWNED;
    Logger::log("[SymbolEnc] Spawned slot %d: monsNo=%d at grid(%d,%d) world(%.1f,%.1f,%.1f)\n",
                index, poke.monsNo, poke.spawnGridX, poke.spawnGridZ, worldX, worldY, worldZ);

    if (poke.isRare) {
        PlayShinySe();
    }
}

static void DespawnSingle(int index) {
    auto& poke = s_symbolPokemon[index];
    if (poke.state == SymbolState::EMPTY || poke.state == SymbolState::COOLDOWN || poke.state == SymbolState::DESPAWNING) return;

    if (poke.state == SymbolState::LOADING) {
        poke.gameObject = nullptr;
        poke.entity = nullptr;
        poke.pokemonParam = nullptr;
        s_loadPending[index] = false;
        s_loadedPrefab[index] = nullptr;
        if (s_currentlyLoadingSlot == index) s_currentlyLoadingSlot = -1;
        poke.cooldownTimer = RESPAWN_DELAY;
        poke.state = SymbolState::COOLDOWN;
        return;
    }

    poke.state = SymbolState::DESPAWNING;
    Logger::log("[SymbolEnc] Despawning slot %d (shrink-out)\n", index);
}

static void CleanupAll() {
    for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
        auto& poke = s_symbolPokemon[i];
        if (poke.gameObject != nullptr) {
            UnityEngine::_Object::Destroy((UnityEngine::_Object::Object*)poke.gameObject);
        }
        poke.gameObject = nullptr;
        poke.entity = nullptr;
        poke.pokemonParam = nullptr;
        s_loadPending[i] = false;
        s_loadedPrefab[i] = nullptr;
        poke.state = SymbolState::EMPTY;
        poke.cooldownTimer = 0.0f;
    }
    s_currentlyLoadingSlot = -1;
}

static void SpawnPersistentShiniesForZone(int32_t zoneID) {
    auto* saveData = getCustomSaveData();
    auto& enc = saveData->symbolEncounters;

    for (int si = 0; si < enc.count; si++) {
        auto& shiny = enc.shinies[si];
        if (shiny.zoneID != zoneID) continue;

        // Mons persist across same-area zone changes; never spawn a shiny twice.
        bool alreadySpawned = false;
        for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
            if (s_symbolPokemon[i].state != SymbolState::EMPTY &&
                s_symbolPokemon[i].isPersistentShiny &&
                s_symbolPokemon[i].persistentShinyIdx == si) {
                alreadySpawned = true;
                break;
            }
        }
        if (alreadySpawned) continue;

        int slot = -1;
        for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
            if (s_symbolPokemon[i].state == SymbolState::EMPTY) {
                slot = i;
                break;
            }
        }
        if (slot < 0) {
            Logger::log("[SymbolEnc] No empty slot for persistent shiny monsNo=%d\n", shiny.monsNo);
            break;
        }

        auto& poke = s_symbolPokemon[slot];
        poke.monsNo = shiny.monsNo;
        poke.formNo = shiny.formNo;
        poke.level = shiny.level;
        poke.sex = shiny.sex;
        poke.isRare = true;
        // Pre-blob save entries fall back to regeneration at battle time.
        poke.hasFullData = shiny.hasFullData;
        if (shiny.hasFullData) {
            static_assert(PERSIST_POKE_DATA_SIZE == SYMBOL_POKE_FULL_DATA_SIZE,
                          "save blob size must match Serialize_FullData size");
            memcpy(poke.fullData, shiny.fullData, SYMBOL_POKE_FULL_DATA_SIZE);
        }
        poke.isPersistentShiny = true;
        poke.persistentShinyIdx = si;
        poke.spawnGridX = shiny.gridX;
        poke.spawnGridZ = shiny.gridZ;
        poke.isWaterTile = SafeIsTileAWaterTile(
            (UnityEngine::Vector2Int::Object){ .fields = { .m_X = shiny.gridX, .m_Y = shiny.gridZ } });

        BeginLoading(slot);
        Logger::log("[SymbolEnc] Spawning persistent shiny: monsNo=%d grid(%d,%d) slot=%d saveIdx=%d\n",
                    shiny.monsNo, shiny.gridX, shiny.gridZ, slot, si);
    }
}

static bool s_firstCall = true;
static float s_logTimer = 0.0f;

static void OnUpdate() {
    if (s_firstCall) {
        Logger::log("[SymbolEnc] OnUpdate hook first call!\n");
        s_firstCall = false;
    }

    if (!IsInField()) {
        g_symbolEncountersActive = false;
        return;
    }

    auto fm = GetFieldManagerInstance();
    if (fm == nullptr) return;

    auto player = GetPlayerEntity();
    if (player == nullptr) return;

    float deltaTime = UnityEngine::Time::get_deltaTime();

    int32_t currentArea = fm->get_areaID();
    int32_t currentZone = fm->fields._now_zoneID;

    if (currentArea != s_currentAreaID) {
        Logger::log("[SymbolEnc] Area changed: %d -> %d, zone=%d\n", s_currentAreaID, currentArea, currentZone);
        CleanupAll();
        s_currentAreaID = currentArea;
        s_currentZoneID = currentZone;
        s_spawnDelay = AREA_SPAWN_DELAY;
        s_rngState = (uint32_t)currentArea * 2654435761u ^ (uint32_t)currentZone * 40503u;
        if (s_rngState == 0) s_rngState = 0x12345678;
        SpawnPersistentShiniesForZone(currentZone);
    } else if (currentZone != s_currentZoneID) {
        Logger::log("[SymbolEnc] Zone changed: %d -> %d (same area %d), keeping mons\n",
                    s_currentZoneID, currentZone, currentArea);
        // Same loaded map, only the zone label changed; keep the mons.
        s_currentZoneID = currentZone;
        s_rngState = (uint32_t)currentArea * 2654435761u ^ (uint32_t)currentZone * 40503u;
        if (s_rngState == 0) s_rngState = 0x12345678;
        SpawnPersistentShiniesForZone(currentZone);
    }

    bool symbolToggleOn = PlayerWork::GetBool((int32_t)FlagWork_Flag::FLAG_SYMBOL_ENCOUNTERS);

    if (!symbolToggleOn || !DoesCurrentZoneIDHaveEncounters()) {
        if (!symbolToggleOn && g_symbolEncountersActive) {
            CleanupAll();
        }
        g_symbolEncountersActive = false;
        return;
    }

    g_symbolEncountersActive = true;

    bool repelActive = EncountDataWork::EncDataSave_CanUseSpray();

    auto* basePlayer = ((FieldObjectEntity::Object*)player);
    UnityEngine::Vector2Int::Object playerGrid = basePlayer->get_gridPosition();
    float playerY = ((BaseEntity::Object*)player)->fields.worldPosition.fields.y;

    if (s_spawnDelay > 0.0f) {
        s_spawnDelay -= deltaTime;
    }

    // One spawn attempt per frame; land and water budgets are independent.
    if (s_spawnDelay <= 0.0f && !repelActive) {
        int landCount = 0, waterCount = 0;
        for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
            if (s_symbolPokemon[i].state != SymbolState::EMPTY && s_symbolPokemon[i].state != SymbolState::COOLDOWN) {
                if (s_symbolPokemon[i].isWaterTile) waterCount++;
                else landCount++;
            }
        }
        // A zone without a water table can never fill a water slot; without this
        // gate a water pick fails selection and retries every frame.
        static int32_t s_encFlagZone = -999;
        static bool s_zoneHasLand = false, s_zoneHasWater = false;
        if (s_encFlagZone != s_currentZoneID) {
            s_encFlagZone = s_currentZoneID;
            auto* fe = GetFieldEncountersOfCurrentZoneID();
            s_zoneHasLand  = fe != nullptr && fe->fields.ground_mons != nullptr &&
                             fe->fields.ground_mons->max_length > 0;
            s_zoneHasWater = fe != nullptr && fe->fields.water_mons != nullptr &&
                             fe->fields.water_mons->max_length > 0;
        }
        bool needLand = landCount < MAX_LAND_POKEMON && s_zoneHasLand;
        bool needWater = waterCount < MAX_WATER_POKEMON && s_zoneHasWater;

        if (needLand || needWater) {
            for (int idx = 0; idx < MAX_SYMBOL_POKEMON; idx++) {
                if (s_symbolPokemon[idx].state != SymbolState::EMPTY) continue;
                if (!TryFindSpawnPosition(idx, playerGrid.fields.m_X, playerGrid.fields.m_Y, playerY)) continue;

                bool isWater = s_symbolPokemon[idx].isWaterTile;
                if ((isWater && !needWater) || (!isWater && !needLand)) continue;

                if (TrySelectSpecies(idx, isWater)) {
                    Logger::log("[SymbolEnc] Slot %d: monsNo=%d lv=%d %s grid(%d,%d) [land=%d water=%d]\n",
                        idx, s_symbolPokemon[idx].monsNo, s_symbolPokemon[idx].level,
                        isWater ? "water" : "land",
                        s_symbolPokemon[idx].spawnGridX, s_symbolPokemon[idx].spawnGridZ,
                        landCount, waterCount);
                    BeginLoading(idx);
                } else {
                    // Selection can still fail (empty time-of-day set); back off.
                    s_spawnDelay = 1.0f;
                }
                break; // Only one TrySelectSpecies call per frame
            }
        }
    }

    for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
        auto& poke = s_symbolPokemon[i];

        switch (poke.state) {
            case SymbolState::EMPTY:
                break;

            case SymbolState::LOADING:
                FinishSpawning(i);
                break;

            case SymbolState::SPAWNED: {
                poke.lifetimeTimer += deltaTime;

                UnityEngine::Transform::Object* pokeTransform = nullptr;
                if (poke.gameObject != nullptr) {
                    pokeTransform = poke.gameObject->get_transform();
                }

                // Periodic glint so a shiny is spottable at a distance.
                if (poke.isRare && poke.scaleProgress >= 1.0f && pokeTransform != nullptr) {
                    poke.sparkleTimer += deltaTime;
                    if (poke.sparkleTimer >= SHINY_SPARKLE_INTERVAL) {
                        poke.sparkleTimer = 0.0f;
                        auto* fm = GetFieldManagerInstance();
                        if (fm != nullptr) {
                            fm->CallEffect(SHINY_SPARKLE_EFFECT_ID, pokeTransform, nullptr, nullptr);
                        }
                    }
                }

                if (poke.scaleProgress < 1.0f) {
                    poke.scaleProgress += deltaTime / GROW_DURATION;
                    if (poke.scaleProgress > 1.0f) poke.scaleProgress = 1.0f;

                    if (pokeTransform != nullptr) {
                        float s = poke.scaleProgress;
                        UnityEngine::Vector3::Object scale;
                        scale.fields.x = poke.targetScaleX * s;
                        scale.fields.y = poke.targetScaleY * s;
                        scale.fields.z = poke.targetScaleZ * s;
                        pokeTransform->set_localScale(scale);
                    }
                }

                if (!poke.isPersistentShiny && poke.lifetimeTimer >= poke.lifetimeDuration) {
                    DespawnSingle(i);
                    break;
                }

                if (pokeTransform != nullptr) {
                    poke.wanderTimer -= deltaTime;
                    if (poke.wanderTimer <= 0.0f) {
                        float candidateX = poke.spawnWorldX + RandRange(-WANDER_RANGE, WANDER_RANGE);
                        float candidateZ = poke.spawnWorldZ + RandRange(-WANDER_RANGE, WANDER_RANGE);
                        if (IsWorldPosOnSafeTile(candidateX, candidateZ)) {
                            poke.wanderTargetX = candidateX;
                            poke.wanderTargetZ = candidateZ;
                        }
                        poke.wanderTimer = RandRange(WANDER_MIN_INTERVAL, WANDER_MAX_INTERVAL);
                    }

                    auto curPos = pokeTransform->get_position();
                    float ddx = poke.wanderTargetX - curPos.fields.x;
                    float ddz = poke.wanderTargetZ - curPos.fields.z;
                    float distSqToTarget = ddx * ddx + ddz * ddz;

                    int32_t desiredAnim = FieldPokemonEntity::ANIM_IDLE;

                    if (distSqToTarget > 0.04f) {
                        float dist = __builtin_sqrtf(distSqToTarget);
                        float step = poke.moveSpeed * deltaTime;
                        if (step > dist) step = dist;

                        float nx = ddx / dist;
                        float nz = ddz / dist;

                        float nextX = curPos.fields.x + nx * step;
                        float nextZ = curPos.fields.z + nz * step;

                        if (IsWorldPosOnSafeTile(nextX, nextZ)) {
                            UnityEngine::Vector3::Object newPos;
                            newPos.fields.x = nextX;
                            newPos.fields.y = curPos.fields.y;
                            newPos.fields.z = nextZ;
                            pokeTransform->set_position(newPos);
                        } else {
                            poke.wanderTargetX = curPos.fields.x;
                            poke.wanderTargetZ = curPos.fields.z;
                        }

                        float targetAngle = __builtin_atan2f(nx, nz) * (180.0f / 3.14159265f);
                        auto curEuler = pokeTransform->get_localEulerAngles();
                        float curAngle = curEuler.fields.y;
                        float angleDiff = targetAngle - curAngle;
                        if (angleDiff > 180.0f) angleDiff -= 360.0f;
                        if (angleDiff < -180.0f) angleDiff += 360.0f;
                        float rotSpeed = 5.0f;
                        float newAngle = curAngle + angleDiff * rotSpeed * deltaTime;
                        UnityEngine::Vector3::Object facing;
                        facing.fields.x = 0.0f;
                        facing.fields.y = newAngle;
                        facing.fields.z = 0.0f;
                        pokeTransform->set_localEulerAngles(facing);

                        desiredAnim = (poke.moveSpeed >= RUN_THRESHOLD) ? FieldPokemonEntity::ANIM_RUN : FieldPokemonEntity::ANIM_WALK;
                    }

                    if (desiredAnim != poke.currentAnimIndex && poke.entity != nullptr) {
                        auto* animPlayer = (AnimationPlayer::Object*)poke.entity->fields._animationPlayer;
                        if (animPlayer != nullptr) {
                            animPlayer->Play(desiredAnim, 0.2f, 0.0f);
                            poke.currentAnimIndex = desiredAnim;
                        }
                    }
                }

                {
                    float pokeX = poke.spawnWorldX;
                    float pokeZ = poke.spawnWorldZ;
                    if (pokeTransform != nullptr) {
                        auto pokePos = pokeTransform->get_position();
                        pokeX = pokePos.fields.x;
                        pokeZ = pokePos.fields.z;
                    }

                    auto* playerTransform = ((BaseEntity::Object*)player)->get_transform();
                    auto playerPos = playerTransform->get_position();
                    float pX = playerPos.fields.x;
                    float pZ = playerPos.fields.z;

                    float dx = pX - pokeX;
                    float dz = pZ - pokeZ;
                    float distSq = dx * dx + dz * dz;

                    if (i == 0) {
                        s_logTimer += deltaTime;
                        if (s_logTimer >= 2.0f) {
                            s_logTimer = 0.0f;
                            Logger::log("[SymbolEnc] Player(%.1f,%.1f) Poke0(%.1f,%.1f) distSq=%.2f\n",
                                        pX, pZ, pokeX, pokeZ, distSq);
                        }
                    }

                    // Mons left far behind respawn near the player.
                    if (distSq > (float)RECYCLE_DIST_SQ && !poke.isPersistentShiny) {
                        DespawnSingle(i);
                        break;
                    }

                    if (distSq < PROXIMITY_THRESHOLD_SQ && !repelActive && poke.scaleProgress >= 1.0f) {
                        Logger::log("[SymbolEnc] Battle triggered! monsNo=%d level=%d shiny=%d\n",
                                    poke.monsNo, poke.level, poke.isRare);

                        if (poke.isPersistentShiny && poke.persistentShinyIdx >= 0) {
                            auto* saveData = getCustomSaveData();
                            saveData->symbolEncounters.RemoveShiny(poke.persistentShinyIdx);
                            for (int j = 0; j < MAX_SYMBOL_POKEMON; j++) {
                                if (j == i) continue;
                                if (s_symbolPokemon[j].isPersistentShiny &&
                                    s_symbolPokemon[j].persistentShinyIdx > poke.persistentShinyIdx) {
                                    s_symbolPokemon[j].persistentShinyIdx--;
                                }
                            }
                        }

                        // Never carry a managed PokemonParam across frames, the GC can relocate it.
                        auto* freshParam = (Pml::PokePara::PokemonParam::Object*)il2cpp_object_new(
                            (Il2CppClass*)Pml::PokePara::PokemonParam::getClass());
                        auto* coreParam = (Pml::PokePara::CoreParam::Object*)freshParam;

                        if (poke.hasFullData) {
                            freshParam->ctor(poke.monsNo, (uint16_t)poke.level, 0);
                            coreParam->fields.m_accessor->Deserialize_FullData(poke.fullData);
                        } else {
                            // Save entry has primitives only: regenerate, then force the
                            // visible attributes to match the overworld model.
                            auto* spec = (Pml::PokePara::InitialSpec::Object*)il2cpp_object_new(
                                (Il2CppClass*)Pml::PokePara::InitialSpec::getClass());
                            spec->ctor();
                            spec->fields.monsno = poke.monsNo;
                            spec->fields.formno = (uint16_t)poke.formNo;
                            spec->fields.level  = (uint16_t)poke.level;
                            freshParam->ctor(spec);
                            coreParam->fields.m_accessor->SetSex((Pml::Sex)poke.sex);
                            if (poke.isRare) {
                                coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED);
                            }
                        }

                        auto* battleParty = (Pml::PokeParty::Object*)il2cpp_object_new(
                            (Il2CppClass*)Pml::PokeParty::getClass());
                        battleParty->ctor();
                        battleParty->AddMember(freshParam);

                        DespawnSingle(i);

                        fm->EventWildBattle(battleParty, false, true, false, false);
                        return; // exit OnUpdate entirely
                    }
                }
                break;
            }

            case SymbolState::DESPAWNING: {
                poke.scaleProgress -= deltaTime / SHRINK_DURATION;

                if (poke.scaleProgress <= 0.0f) {
                    if (poke.gameObject != nullptr) {
                        UnityEngine::_Object::Destroy((UnityEngine::_Object::Object*)poke.gameObject);
                    }
                    poke.gameObject = nullptr;
                    poke.entity = nullptr;
                    poke.pokemonParam = nullptr;
                    s_loadPending[i] = false;
                    s_loadedPrefab[i] = nullptr;
                    poke.cooldownTimer = RESPAWN_DELAY;
                    poke.state = SymbolState::COOLDOWN;
                } else if (poke.gameObject != nullptr) {
                    auto* transform = poke.gameObject->get_transform();
                    if (transform != nullptr) {
                        float s = poke.scaleProgress;
                        UnityEngine::Vector3::Object scale;
                        scale.fields.x = poke.targetScaleX * s;
                        scale.fields.y = poke.targetScaleY * s;
                        scale.fields.z = poke.targetScaleZ * s;
                        transform->set_localScale(scale);
                    }
                }
                break;
            }

            case SymbolState::COOLDOWN: {
                poke.cooldownTimer -= deltaTime;
                if (poke.cooldownTimer <= 0.0f) {
                    poke.state = SymbolState::EMPTY;
                }
                break;
            }
        }
    }
}

static void PreUpdate() {
    auto fm = GetFieldManagerInstance();
    if (fm == nullptr) {
        g_symbolEncountersActive = false;
        return;
    }
    bool symbolToggleOn = PlayerWork::GetBool((int32_t)FlagWork_Flag::FLAG_SYMBOL_ENCOUNTERS);
    if (symbolToggleOn && DoesCurrentZoneIDHaveEncounters()) {
        g_symbolEncountersActive = true;
    } else {
        g_symbolEncountersActive = false;
    }
}

HOOK_DEFINE_TRAMPOLINE(FieldManagerUpdate) {
    static void Callback(FieldManager::Object* __this) {
        PreUpdate();
        Orig(__this);
        OnUpdate();
    }
};

void exl_symbol_encounters_main() {
    Logger::addListener([](const char* message) {
        svcOutputDebugString(message, __builtin_strlen(message));
    });

    for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
        s_symbolPokemon[i].state = SymbolState::EMPTY;
        s_symbolPokemon[i].gameObject = nullptr;
        s_symbolPokemon[i].entity = nullptr;
        s_symbolPokemon[i].pokemonParam = nullptr;
    }

    s_currentAreaID = -1;
    s_currentZoneID = -1;
    s_initialized = true;

    FieldManagerUpdate::InstallAtOffset(0x0179A080);

    Logger::log("[SymbolEnc] Symbol encounters initialized\n");
}
