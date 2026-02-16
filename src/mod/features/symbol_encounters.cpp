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
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/AnimationPlayer.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/Personal/SexVector.h"
#include "externals/Pml/Sex.h"
#include "externals/System/Action.h"
#include "externals/System/String.h"
#include "externals/System/Type.h"
#include "externals/XLSXContent/PersonalTable.h"
#include "externals/XLSXContent/PokemonInfo.h"

#include "externals/AttributeID.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "externals/UnityEngine/Collider.h"
#include "externals/FlagWork_Enums.h"
#include "externals/ZoneWork.h"
#include "externals/ZukanWork.h"

#include "save/save.h"

#include "logger/logger.h"
#include "exlaunch/nx/kernel/svc.h"

// ============================================================
// Constants
// ============================================================

static constexpr int MAX_SYMBOL_POKEMON = 14;
static constexpr int MAX_LAND_POKEMON = 7;
static constexpr int MAX_WATER_POKEMON = 7;
static constexpr float PROXIMITY_THRESHOLD_SQ = 1.0f;
static constexpr float GROW_DURATION = 0.5f;
static constexpr float SHRINK_DURATION = 0.5f;
static constexpr float MIN_LIFETIME = 45.0f;
static constexpr float MAX_LIFETIME = 60.0f;
static constexpr int SPAWN_MIN_DIST_SQ = 16;
static constexpr int SPAWN_MAX_DIST_SQ = 2500;
static constexpr int MAX_SPAWN_ATTEMPTS = 15;
static constexpr float WANDER_MIN_INTERVAL = 3.0f;
static constexpr float WANDER_MAX_INTERVAL = 5.0f;
static constexpr float WANDER_RANGE = 5.0f;
static constexpr float RESPAWN_DELAY = 1.0f;

// TODO: Replace with the actual shiny sparkle SE hash from Wwise bank
static constexpr uint32_t SHINY_SPARKLE_SE = 0x4491b890;

// ============================================================
// External method bindings not in existing headers
// ============================================================

// FureaiHiroba_PokeFactory.SetPokeScale (STATIC)
static void SetPokeScale(UnityEngine::Transform::Object* transform, Pml::PokePara::PokemonParam::Object* pokemonParam) {
    _ILExternal::external<void>(0x1A7EA40, transform, pokemonParam);
}

// FieldPokemonEntity.Animation indices (from il2cpp dump)
static constexpr int32_t ANIM_IDLE = 0;
static constexpr int32_t ANIM_WALK = 1;
static constexpr int32_t ANIM_RUN  = 2;

// Speed thresholds
static constexpr float WALK_SPEED_MIN = 0.3f;
static constexpr float WALK_SPEED_MAX = 0.66f;
static constexpr float RUN_SPEED_MIN  = 0.7f;
static constexpr float RUN_SPEED_MAX  = 1.32f;
static constexpr float RUN_THRESHOLD  = 0.6f;

// ============================================================
// Custom PRNG — avoids polluting Unity's global Random state.
// UnityEngine.Random is shared with trainer AI, NPC behavior, etc.
// ============================================================

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

// ============================================================
// Encounter table helpers
// ============================================================

extern XLSXContent::FieldEncountTable::Sheettable::Object* GetFieldEncountersOfCurrentZoneID();

// Encounter slot modifier functions from encounter_slots.cpp
extern void SetBaseGroundSlots(Dpr::Field::EncountResult::Object** encounterHolder, MonsLv::Array* slots);
extern void SetTimeOfDaySlots(MonsLv::Array* slots);
extern void SetSwarmSlots(MonsLv::Array* slots);
extern void SetTrophyGardenSlots(MonsLv::Array* slots);
extern void SetGBASlots(MonsLv::Array* slots);
extern void SetWaterGBASlots(MonsLv::Array* slots);
extern bool DoesTileGiveEncounters(UnityEngine::Vector2Int::Object tile);
extern bool IsTileAWaterTile(UnityEngine::Vector2Int::Object tile);

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

static bool DoesCurrentZoneIDHaveEncounters() {
    XLSXContent::FieldEncountTable::Sheettable::Object* fieldEnc = GetFieldEncountersOfCurrentZoneID();
    return fieldEnc != nullptr;
}

// Check if a grid tile is valid for Pokemon placement.
// For water tiles, also checks cardinal neighbors (keeps Pokemon 1 tile from water edges).
// For land tiles, only checks the center (narrow cave corridors are fine).
static bool IsTileSafeForPokemon(int32_t gridX, int32_t gridZ) {
    UnityEngine::Vector2Int::Object center;
    center.fields.m_X = gridX;
    center.fields.m_Y = gridZ;
    if (!SafeDoesTileGiveEncounters(center)) return false;

    // Water tiles need extra margin to prevent walking through shore walls
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

// World position → grid tile safe check
static bool IsWorldPosOnSafeTile(float worldX, float worldZ) {
    int32_t gridX = -(int32_t)worldX;
    int32_t gridZ = (int32_t)worldZ;
    return IsTileSafeForPokemon(gridX, gridZ);
}

// ============================================================
// State machine
// ============================================================

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

    float targetScaleX;
    float targetScaleY;
    float targetScaleZ;

    float wanderTimer;
    float wanderTargetX;
    float wanderTargetZ;
    float moveSpeed;
    int32_t currentAnimIndex;
};

// ============================================================
// Static state
// ============================================================

static SymbolPokemon s_symbolPokemon[MAX_SYMBOL_POKEMON];
static int32_t s_currentAreaID = -1;
static int32_t s_currentZoneID = -1;
static bool s_initialized = false;

bool g_symbolEncountersActive = false;


static float s_spawnDelay = 0.0f;
static constexpr float AREA_SPAWN_DELAY = 1.0f;

static bool s_loadPending[MAX_SYMBOL_POKEMON] = {};
static Il2CppObject* s_loadedPrefab[MAX_SYMBOL_POKEMON] = {};

static MethodInfo* s_onAssetLoadedMI = nullptr;

// ============================================================
// Forward declarations
// ============================================================

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

// ============================================================
// Helpers
// ============================================================

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
        audioManager->PlaySe(SHINY_SPARKLE_SE, nullptr);
    }
}

// ============================================================
// Species selection from encounter table
// ============================================================

static bool TrySelectSpecies(int index, bool isWaterTile) {
    Logger::log("[SymbolEnc] TrySelectSpecies(%d, water=%d) starting\n", index, isWaterTile);

    // Load the same IL2CPP typeinfos the encounter system needs
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

    // Get lead party Pokemon for ability-based encounter effects
    Pml::PokeParty::Object* party = PlayerWork::get_playerParty();
    if (party == nullptr || party->fields.m_memberCount == 0) return false;
    auto* firstPokemon = (Pml::PokePara::PokemonParam::Object*)party->GetMemberPointer(0);

    // Set up the spa struct (repel/ability info)
    Dpr::Field::FieldEncount::ENC_FLD_SPA::Object spaStruct{};
    Dpr::Field::FieldEncount::SetSpaStruct(firstPokemon, fieldEnc, &spaStruct);

    // Create result holder and slot array
    auto* encounterHolder = Dpr::Field::EncountResult::newInstance();
    auto* slots = MonsLv::newArray(12);
    Logger::log("[SymbolEnc] TrySelectSpecies: slots=%p encounterHolder=%p\n", slots, encounterHolder);

    if (isWaterTile) {
        if (fieldEnc->fields.water_mons == nullptr || fieldEnc->fields.water_mons->max_length == 0 ||
            fieldEnc->fields.encRate_wat <= 0) return false;
        for (uint32_t i = 0; i < slots->max_length && i < fieldEnc->fields.water_mons->max_length; i++) {
            slots->m_Items[i] = fieldEnc->fields.water_mons->m_Items[i];
        }
        if (fieldEnc->fields.gbaRuby != nullptr) {
            SetWaterGBASlots(slots);
        }
    } else {
        if (fieldEnc->fields.ground_mons == nullptr || fieldEnc->fields.ground_mons->max_length == 0 ||
            fieldEnc->fields.encRate_gr <= 0) return false;
        SetBaseGroundSlots(&encounterHolder, slots);
        if (fieldEnc->fields.day != nullptr && fieldEnc->fields.night != nullptr) {
            SetTimeOfDaySlots(slots);
        }
        if (EncountDataWork::IsTairyouHassei()) {
            if (fieldEnc->fields.tairyo != nullptr) {
                SetSwarmSlots(slots);
            }
        }
        int32_t zoneId = PlayerWork::get_zoneID();
        if (ZoneWork::IsHillBackZone(zoneId) && ZukanWork::GetZenkokuFlag()) {
            SetTrophyGardenSlots(slots);
        }
        if (fieldEnc->fields.gbaRuby != nullptr) {
            SetGBASlots(slots);
        }
    }

    // Use the game's own encounter generation — applies ability effects, probability weighting, etc.
    bool success = Dpr::Field::FieldEncount::SetEncountData(
        firstPokemon, 0, spaStruct, slots, isWaterTile ? 0 : 0, 1, &encounterHolder);
    if (!success) {
        Logger::log("[SymbolEnc] SetEncountData failed\n");
        return false;
    }

    // Apply form changes (Unown, etc.)
    Dpr::Field::FieldEncount::LastProc(&encounterHolder, &spaStruct);

    // Read the result
    if (encounterHolder->fields.Enemy == nullptr || encounterHolder->fields.Level == nullptr) {
        Logger::log("[SymbolEnc] EncountResult has null Enemy/Level\n");
        return false;
    }

    int32_t monsNo = encounterHolder->fields.Enemy->m_Items[0];
    int32_t level = encounterHolder->fields.Level->m_Items[0];
    int32_t formNo = encounterHolder->fields.karanaForm;
    bool isRare = encounterHolder->fields.IsRare;

    if (monsNo == 0) return false;

    s_symbolPokemon[index].monsNo = monsNo;
    s_symbolPokemon[index].formNo = (int16_t)formNo;
    s_symbolPokemon[index].level = level;
    s_symbolPokemon[index].isRare = isRare;
    s_symbolPokemon[index].isPersistentShiny = false;
    s_symbolPokemon[index].persistentShinyIdx = -1;

    // Determine sex from personal data
    XLSXContent::PersonalTable::SheetPersonal::Object* personal =
        Pml::Personal::PersonalSystem::GetPersonalData(monsNo, formNo);
    if (personal != nullptr) {
        uint8_t sexRate = personal->fields.sex;
        switch (sexRate) {
            case (uint8_t)Pml::Personal::SexVector::ONLY_FEMALE:
                s_symbolPokemon[index].sex = (int8_t)Pml::Sex::FEMALE;
                break;
            case (uint8_t)Pml::Personal::SexVector::UNKNOWN:
                s_symbolPokemon[index].sex = (int8_t)Pml::Sex::UNKNOWN;
                break;
            default:
                s_symbolPokemon[index].sex = (int8_t)Pml::Sex::MALE;
                break;
        }
    } else {
        s_symbolPokemon[index].sex = (int8_t)Pml::Sex::MALE;
    }

    // Persist shinies to save data
    if (isRare) {
        int32_t zoneId = PlayerWork::get_zoneID();
        auto* saveData = getCustomSaveData();
        saveData->symbolEncounters.AddShiny(
            monsNo, (int16_t)formNo, level, s_symbolPokemon[index].sex,
            zoneId, s_symbolPokemon[index].spawnGridX, s_symbolPokemon[index].spawnGridZ);
        s_symbolPokemon[index].isPersistentShiny = true;
        s_symbolPokemon[index].persistentShinyIdx = saveData->symbolEncounters.count - 1;
    }

    Logger::log("[SymbolEnc] Generated: monsNo=%d form=%d level=%d rare=%d%s\n",
        monsNo, formNo, level, isRare, isRare ? " [PERSISTED]" : "");
    return true;
}

// ============================================================
// Position finding
// ============================================================

static bool TryFindSpawnPosition(int index, int32_t playerGridX, int32_t playerGridZ, float playerY) {
    for (int attempt = 0; attempt < MAX_SPAWN_ATTEMPTS; attempt++) {
        int32_t dx = RandRange(-50, 51);
        int32_t dz = RandRange(-50, 51);

        int32_t distSq = dx * dx + dz * dz;
        if (distSq < SPAWN_MIN_DIST_SQ || distSq > SPAWN_MAX_DIST_SQ) continue;

        int32_t gridX = playerGridX + dx;
        int32_t gridZ = playerGridZ + dz;

        // Use the safe check (water-aware: water tiles need cardinal neighbor margin)
        if (!IsTileSafeForPokemon(gridX, gridZ)) continue;

        // Check not occupied by another symbol Pokemon
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

// ============================================================
// Asset load callback
// ============================================================

static void OnAssetLoaded(Il2CppObject* loadedObject) {
    Logger::log("[SymbolEnc] OnAssetLoaded callback fired, obj=%p\n", loadedObject);

    for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
        if (s_loadPending[i]) {
            s_loadPending[i] = false;
            s_loadedPrefab[i] = loadedObject;
            Logger::log("[SymbolEnc] Asset loaded for slot %d\n", i);
            return;
        }
    }
    // Orphan load (area changed during async load) — just log it.
    // Don't Destroy (corrupts asset cache) or SetActive (corrupts prefab state).
    Logger::log("[SymbolEnc] WARNING: OnAssetLoaded but no pending slot (orphan)\n");
}

// ============================================================
// Asset loading
// ============================================================

static void BeginLoading(int index) {
    auto& poke = s_symbolPokemon[index];
    poke.state = SymbolState::LOADING;

    Logger::log("[SymbolEnc] Loading slot %d: monsNo=%d form=%d\n", index, poke.monsNo, poke.formNo);

    Logger::log("[SymbolEnc] BeginLoading: creating PokemonParam...\n");
    poke.pokemonParam = Pml::PokePara::PokemonParam::newInstance(poke.monsNo, (uint16_t)poke.level, (uint64_t)0);
    if (poke.pokemonParam == nullptr) {
        Logger::log("[SymbolEnc] Failed to create PokemonParam for monsNo=%d\n", poke.monsNo);
        poke.state = SymbolState::EMPTY;
        return;
    }
    Logger::log("[SymbolEnc] BeginLoading: PokemonParam created=%p\n", poke.pokemonParam);

    GameData::DataManager::getClass()->initIfNeeded();
    Logger::log("[SymbolEnc] BeginLoading: getting catalog...\n");
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
    if (assetBundleName == nullptr) {
        Logger::log("[SymbolEnc] No AssetBundleName for monsNo=%d\n", poke.monsNo);
        poke.pokemonParam = nullptr;
        poke.state = SymbolState::EMPTY;
        return;
    }

    Logger::log("[SymbolEnc] BeginLoading: getting asset path...\n");
    Dpr::SubContents::Utils::getClass()->initIfNeeded();
    auto* fullPath = Dpr::SubContents::Utils::GetPokemonAssetbundleName(assetBundleName);
    if (fullPath == nullptr) {
        Logger::log("[SymbolEnc] GetPokemonAssetbundleName returned null\n");
        poke.pokemonParam = nullptr;
        poke.state = SymbolState::EMPTY;
        return;
    }

    s_loadPending[index] = true;
    s_loadedPrefab[index] = nullptr;

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

// ============================================================
// Finish spawning
// ============================================================

static void FinishSpawning(int index) {
    auto& poke = s_symbolPokemon[index];

    if (s_loadedPrefab[index] == nullptr) {
        return; // still loading
    }

    Il2CppObject* loadedAsset = s_loadedPrefab[index];
    s_loadedPrefab[index] = nullptr;

    Logger::log("[SymbolEnc] FinishSpawning slot %d: asset=%p\n", index, loadedAsset);

    // Instantiate a copy. Don't touch the original — Destroy corrupts asset cache,
    // SetActive corrupts prefab state. The original may appear as a "brick wall"
    // but that's less critical than breaking zone transitions.
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

    SetPokeScale(transform, poke.pokemonParam);

    UnityEngine::Vector3::Object savedScale = transform->get_localScale();
    poke.targetScaleX = savedScale.fields.x;
    poke.targetScaleY = savedScale.fields.y;
    poke.targetScaleZ = savedScale.fields.z;

    UnityEngine::Vector3::Object zeroScale;
    zeroScale.fields.x = 0.0f;
    zeroScale.fields.y = 0.0f;
    zeroScale.fields.z = 0.0f;
    transform->set_localScale(zeroScale);

    // Get FieldPokemonEntity component
    {
        System::RuntimeTypeHandle::Object handle {};
        handle.fields.value = &FieldPokemonEntity::getClass()->_1.byval_arg;
        poke.entity = _ILExternal::external<FieldPokemonEntity::Object*>(
            0x026a8240, poke.gameObject, System::Type::GetTypeFromHandle(handle));
    }
    if (poke.entity != nullptr) {
        poke.entity->fields.updateEnable = true;

        auto* animPlayer = (AnimationPlayer::Object*)poke.entity->fields._animationPlayer;
        if (animPlayer != nullptr) {
            animPlayer->Play(ANIM_IDLE, 0.0f, 0.0f);
            poke.currentAnimIndex = ANIM_IDLE;
        }
    }

    // Destroy all colliders so our Pokemon don't physically block the player
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

    // Assign random movement speed
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

    poke.state = SymbolState::SPAWNED;
    Logger::log("[SymbolEnc] Spawned slot %d: monsNo=%d at grid(%d,%d) world(%.1f,%.1f,%.1f)\n",
                index, poke.monsNo, poke.spawnGridX, poke.spawnGridZ, worldX, worldY, worldZ);

    // Play shiny sparkle sound when a shiny Pokemon spawns
    if (poke.isRare) {
        PlayShinySe();
    }
}

// ============================================================
// Despawn
// ============================================================

static void DespawnSingle(int index) {
    auto& poke = s_symbolPokemon[index];
    if (poke.state == SymbolState::EMPTY || poke.state == SymbolState::COOLDOWN || poke.state == SymbolState::DESPAWNING) return;

    if (poke.state == SymbolState::LOADING) {
        // No gameObject to animate — just cancel
        poke.gameObject = nullptr;
        poke.entity = nullptr;
        poke.pokemonParam = nullptr;
        s_loadPending[index] = false;
        s_loadedPrefab[index] = nullptr;
        poke.cooldownTimer = RESPAWN_DELAY;
        poke.state = SymbolState::COOLDOWN;
        return;
    }

    // SPAWNED → DESPAWNING: shrink-out animation
    // scaleProgress stays at current value (shrinks from wherever it is)
    poke.state = SymbolState::DESPAWNING;
    Logger::log("[SymbolEnc] Despawning slot %d (shrink-out)\n", index);
}

// Immediately destroy all symbol Pokemon GameObjects and reset state.
// Used for area/zone transitions where we need instant cleanup.
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
}

// ============================================================
// Spawn persistent shinies for the current zone from save data
// ============================================================

static void SpawnPersistentShiniesForZone(int32_t zoneID) {
    auto* saveData = getCustomSaveData();
    auto& enc = saveData->symbolEncounters;

    for (int si = 0; si < enc.count; si++) {
        auto& shiny = enc.shinies[si];
        if (shiny.zoneID != zoneID) continue;

        // Find an empty slot
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

// ============================================================
// Per-frame update (called AFTER game's FieldManager.Update)
// ============================================================

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

    // Check for area/zone change
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
        Logger::log("[SymbolEnc] Zone changed: %d -> %d (same area %d)\n", s_currentZoneID, currentZone, currentArea);
        CleanupAll();
        s_currentZoneID = currentZone;
        s_spawnDelay = AREA_SPAWN_DELAY;
        s_rngState = (uint32_t)currentArea * 2654435761u ^ (uint32_t)currentZone * 40503u;
        if (s_rngState == 0) s_rngState = 0x12345678;
        SpawnPersistentShiniesForZone(currentZone);
    }

    // Check if symbol encounters are toggled on via the Silph Scope Mk.II key item
    bool symbolToggleOn = PlayerWork::GetBool((int32_t)FlagWork_Flag::FLAG_SYMBOL_ENCOUNTERS);

    if (!symbolToggleOn || !DoesCurrentZoneIDHaveEncounters()) {
        if (!symbolToggleOn && g_symbolEncountersActive) {
            // Just toggled off — clean up existing Pokemon
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

    // Tick spawn delay
    if (s_spawnDelay > 0.0f) {
        s_spawnDelay -= deltaTime;
    }

    // Spawn ONE Pokemon per frame (staggered to avoid burst of game API calls)
    // Land and water have independent budgets
    if (s_spawnDelay <= 0.0f && !repelActive) {
        int landCount = 0, waterCount = 0;
        for (int i = 0; i < MAX_SYMBOL_POKEMON; i++) {
            if (s_symbolPokemon[i].state != SymbolState::EMPTY && s_symbolPokemon[i].state != SymbolState::COOLDOWN) {
                if (s_symbolPokemon[i].isWaterTile) waterCount++;
                else landCount++;
            }
        }
        bool needLand = landCount < MAX_LAND_POKEMON;
        bool needWater = waterCount < MAX_WATER_POKEMON;

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
                }
                break; // Only one TrySelectSpecies call per frame
            }
        }
    }

    // Update each slot
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

                // Grow-in animation
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

                // Lifetime expired — begin shrink-out via DESPAWNING state
                // Persistent shinies never auto-despawn
                if (!poke.isPersistentShiny && poke.lifetimeTimer >= poke.lifetimeDuration) {
                    DespawnSingle(i);
                    break;
                }

                // === Wander: move transform directly toward random targets ===
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

                    int32_t desiredAnim = ANIM_IDLE;

                    if (distSqToTarget > 0.04f) {
                        float dist = __builtin_sqrtf(distSqToTarget);
                        float step = poke.moveSpeed * deltaTime;
                        if (step > dist) step = dist;

                        float nx = ddx / dist;
                        float nz = ddz / dist;

                        float nextX = curPos.fields.x + nx * step;
                        float nextZ = curPos.fields.z + nz * step;

                        // Validate next position — don't walk through walls
                        if (IsWorldPosOnSafeTile(nextX, nextZ)) {
                            UnityEngine::Vector3::Object newPos;
                            newPos.fields.x = nextX;
                            newPos.fields.y = curPos.fields.y;
                            newPos.fields.z = nextZ;
                            pokeTransform->set_position(newPos);
                        } else {
                            // Hit a wall — stop and pick a new target next tick
                            poke.wanderTargetX = curPos.fields.x;
                            poke.wanderTargetZ = curPos.fields.z;
                        }

                        // Smooth rotation toward movement direction
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

                        desiredAnim = (poke.moveSpeed >= RUN_THRESHOLD) ? ANIM_RUN : ANIM_WALK;
                    }

                    if (desiredAnim != poke.currentAnimIndex && poke.entity != nullptr) {
                        auto* animPlayer = (AnimationPlayer::Object*)poke.entity->fields._animationPlayer;
                        if (animPlayer != nullptr) {
                            animPlayer->Play(desiredAnim, 0.2f, 0.0f);
                            poke.currentAnimIndex = desiredAnim;
                        }
                    }
                }

                // Proximity check for battle
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

                    if (distSq < PROXIMITY_THRESHOLD_SQ && !repelActive) {
                        Logger::log("[SymbolEnc] Battle triggered! monsNo=%d level=%d shiny=%d\n",
                                    poke.monsNo, poke.level, poke.isRare);

                        // Remove persistent shiny from save data on battle
                        if (poke.isPersistentShiny && poke.persistentShinyIdx >= 0) {
                            auto* saveData = getCustomSaveData();
                            saveData->symbolEncounters.RemoveShiny(poke.persistentShinyIdx);
                            // Fix up indices for any other active persistent shinies
                            for (int j = 0; j < MAX_SYMBOL_POKEMON; j++) {
                                if (j == i) continue;
                                if (s_symbolPokemon[j].isPersistentShiny &&
                                    s_symbolPokemon[j].persistentShinyIdx > poke.persistentShinyIdx) {
                                    s_symbolPokemon[j].persistentShinyIdx--;
                                }
                            }
                            Logger::log("[SymbolEnc] Removed persistent shiny idx=%d from save\n",
                                        poke.persistentShinyIdx);
                        }

                        // Guard: pokemonParam could be null if loading partially failed
                        if (poke.pokemonParam == nullptr) {
                            Logger::log("[SymbolEnc] pokemonParam null for slot %d, skipping battle\n", i);
                            DespawnSingle(i);
                            break;
                        }

                        // Build a PokeParty with the exact Pokemon touched
                        Pml::PokeParty::getClass()->initIfNeeded();
                        auto* battleParty = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)Pml::PokeParty::getClass());
                        battleParty->ctor();
                        battleParty->AddMember(poke.pokemonParam);

                        // Shrink-out the touched Pokemon (DESPAWNING handles destroy)
                        DespawnSingle(i);

                        // PokeParty overload ensures the exact species/level/form battles
                        fm->EventWildBattle(battleParty, false, true, false, false);
                        return; // exit OnUpdate entirely
                    }
                }
                break;
            }

            case SymbolState::DESPAWNING: {
                poke.scaleProgress -= deltaTime / SHRINK_DURATION;

                if (poke.scaleProgress <= 0.0f) {
                    // Shrink complete — destroy
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

// ============================================================
// Lightweight pre-update: just set the encounter suppression flag
// Runs BEFORE the game's FieldManager.Update so encounter checks see the flag
// ============================================================

static void PreUpdate() {
    auto fm = GetFieldManagerInstance();
    if (fm == nullptr) {
        g_symbolEncountersActive = false;
        return;
    }
    // Check toggle flag + zone — lightweight, no heavy processing
    bool symbolToggleOn = PlayerWork::GetBool((int32_t)FlagWork_Flag::FLAG_SYMBOL_ENCOUNTERS);
    if (symbolToggleOn && DoesCurrentZoneIDHaveEncounters()) {
        g_symbolEncountersActive = true;
    } else {
        g_symbolEncountersActive = false;
    }
}

// ============================================================
// Hook: Trampoline on FieldManager.Update
// PreUpdate runs BEFORE (sets encounter suppression flag)
// OnUpdate runs AFTER (all heavy work: spawning, movement, proximity)
// This keeps the game's own update timing clean.
// ============================================================

HOOK_DEFINE_TRAMPOLINE(FieldManagerUpdate) {
    static void Callback(FieldManager::Object* __this) {
        PreUpdate();
        Orig(__this);
        OnUpdate();
    }
};

// ============================================================
// Initialization
// ============================================================

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
