#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/Dpr/Field/SwayGrass.h"
#include "externals/System/Primitives.h"
#include "externals/System/Math.h"
#include "externals/GameManager.h"
#include "externals/AttributeID.h"
#include "externals/RandomGroupWork.h"
#include "externals/FieldObjectEntity.h"
#include "externals/UnityEngine/RaycastHit.h"
#include "externals/UnityEngine/Physics.h"
#include "externals/Layer.h"
#include "externals/PlayerWork.h"
#include "externals/TairyouHasseiPokeManager.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"

#include "logger/logger.h"

constexpr int32_t SWAY_GRASS_SEARCH_SIZE = 9;
constexpr int32_t AREA = SWAY_GRASS_SEARCH_SIZE * SWAY_GRASS_SEARCH_SIZE;
constexpr int32_t NUM_RINGS = 4;


enum class CenterMaterial : int32_t {
    INVALID = -1,
    GRASS = 0,
    WATER = 1,
    CAVE = 2
};

static void log_MatCheck(int32_t attrCode) {
    nn::string material;
    if (AttributeID::MATR_IsGrass(attrCode)) {
        material = "Grass";
    }

    else if (AttributeID::MATR_IsWater((attrCode))) {
        material = "Water";
    }

    else if (AttributeID::MATR_IsDunFloor(attrCode)) {
        material = "Dungeon Floor";
    }
    else {
        material = "Invalid";
    }

    Logger::log("[LotSwayGrass] Tile is %s.\n", material.c_str());
}

static CenterMaterial MaterialCheck(int32_t attrCode) {
    log_MatCheck(attrCode);
    if (AttributeID::MATR_IsGrass(attrCode)) {
        return CenterMaterial::GRASS;
    }

    else if (AttributeID::MATR_IsWater((attrCode))) {
        return CenterMaterial::WATER;
    }

    else if (AttributeID::MATR_IsDunFloor(attrCode)) {
        return CenterMaterial::CAVE;
    }

    else {
        return CenterMaterial::INVALID;
    }
}

static CenterMaterial CheckCenterTile(const UnityEngine::Vector2Int::Object& center, int32_t* outCode, int32_t* outStop) {
    GameManager::getClass()->initIfNeeded();
    GameManager::GetAttribute(center, outCode, outStop, false);
    return MaterialCheck(GameManager::GetAttributeTable(*outCode)->fields.Code);
}

HOOK_DEFINE_INLINE(EvCmd_POKETORE_START) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("[POKETORE_START] LotSwayGrass returned %d. Setting Flag %d\n", static_cast<int32_t>(ctx->X[1]), static_cast<int32_t>(ctx->X[0]));
        FlagWork::SetWork(static_cast<int32_t>(ctx->X[0]), static_cast<int32_t>(ctx->X[1]));
    }
};

HOOK_DEFINE_INLINE(FieldManager$$CheckAvailableFieldItem) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[0] = (AttributeID::MATR_IsGrass(ctx->W[0]) ||
                AttributeID::MATR_IsWater(ctx->W[0]) ||
                AttributeID::MATR_IsDunFloor(ctx->W[0]));
    }
};

HOOK_DEFINE_REPLACE(SwayGrass$$LotSwayGrass) {
    static int32_t Callback(UnityEngine::Vector2Int::Object* grid, float height) {
        Logger::log("[LotSwayGrass]\n");
        int32_t maxRankFound = 0;
        int32_t candidates = 0;
        int32_t outCode, outStop;
        system_load_typeinfo(0x8255);

        Dpr::Field::SwayGrass::getClass()->initIfNeeded();
        auto gData = Dpr::Field::SwayGrass::getClass()->static_fields->grass_data;

        auto radarType = CheckCenterTile(*grid, &outCode, &outStop);

        if (gData == nullptr) {
            // || radarType == CenterMaterial::INVALID
            Logger::log("[LotSwayGrass] Nullptr\n");
            return 0;
        }

        // Merge after Debug
        if (radarType == CenterMaterial::INVALID) {
            Logger::log("[LotSwayGrass] Center tile is Invalid.\n");
            return 0;
        }

        // Reset Sway Slots
        for (int32_t i = 0; i < gData->max_length; i++) {
            gData->m_Items[i]->fields.enable = false;
        }

        Logger::log("[LotSwayGrass] Slots reset.\n");

        auto rank = System::Int32_array::newArray(AREA);
        auto attrCode = System::Int32_array::newArray(AREA);

        Logger::log("[LotSwayGrass] Arrays initialised.\n");

        for (int32_t i = 0; i < AREA; i++) {
            rank->m_Items[i] = 0;
            attrCode->m_Items[i] = -1;
        }

        Logger::log("[LotSwayGrass] Ranks and attrCodes reset\n");

        for (int32_t dy = -NUM_RINGS; dy <= NUM_RINGS; dy++) {
            for (int32_t dx = -NUM_RINGS; dx <= NUM_RINGS; dx++) {
                int32_t index = (dy + NUM_RINGS) * 9 + (dx + NUM_RINGS);
                int32_t chebyshevRing = System::Math::Max(abs(dx), abs(dy));
                auto cell = (UnityEngine::Vector2Int::Object) {
                        .fields = {
                                .m_X = grid->fields.m_X + dx,
                                .m_Y = grid->fields.m_Y + dy
                        }
                };

                GameManager::getClass()->initIfNeeded();
                GameManager::GetAttribute(cell, &outCode, &outStop, false);
                auto cellAttrCode = GameManager::GetAttributeTable(outCode)->fields.Code;
                attrCode->m_Items[index] = cellAttrCode;

                if (MaterialCheck(cellAttrCode) != radarType) {
                    continue;
                }

                rank->m_Items[index] = chebyshevRing;
                if (chebyshevRing > maxRankFound) {
                    maxRankFound = chebyshevRing;
                }

                candidates++;
            }
        }

        Logger::log("[LotSwayGrass] Total candidates: %d\n", candidates);
        if (candidates == 0)
            return 0;

        int32_t iroRange = Dpr::Field::SwayGrass::RensaNum(Dpr::Field::SwayGrass::getClass()->static_fields->rensa_count, false);
        int32_t swayPatchNo = candidates < 1 ? 0 : System::Math::Min(candidates, 4);
        Logger::log("[LotSwayGrass] swayPatchNo: %d\n", swayPatchNo);
        if (swayPatchNo < 1)
            return 0;

        int32_t threshold = maxRankFound;
        int32_t selectedPatch = 0;

        while (selectedPatch < swayPatchNo) {
            int32_t poolIndex = RandomGroupWork::RandomValue(AREA);

            if (rank->m_Items[poolIndex] >= threshold) {
                int32_t rdx = (poolIndex % SWAY_GRASS_SEARCH_SIZE) - NUM_RINGS;
                int32_t rdy = (poolIndex / SWAY_GRASS_SEARCH_SIZE) - NUM_RINGS;

                auto cell = (UnityEngine::Vector2Int::Object) {
                    .fields = {
                            .m_X = grid->fields.m_X + rdx,
                            .m_Y = grid->fields.m_Y + rdy
                    }
                };

                Logger::log("[LotSwayGrass] m_X = %d (%d + %d), m_Y = %d (%d + %d)\n", cell.fields.m_X,
                            grid->fields.m_X, rdx, cell.fields.m_Y, grid->fields.m_Y, rdy);

                auto pos = (UnityEngine::Vector3::Object) {
                    .fields = {
                        .x = -static_cast<float>(cell.fields.m_X),
                        .y = height + 2.0f,
                        .z = static_cast<float>(cell.fields.m_Y),
                    }
                };

                //Raycast down
                auto down = (UnityEngine::Vector3::Object) {
                        .fields = {
                                .x = 0.0f,
                                .y = -1.0f,
                                .z = 0.0f
                        }
                };

                UnityEngine::RaycastHit::Object hitInfo = {};

                Layer::getClass()->initIfNeeded();
                bool raycastHit = UnityEngine::Physics::Raycast(pos, down, &hitInfo, 3.0f,
                                                                                              Layer::getClass()->static_fields->Ground);

                if (raycastHit && radarType != CenterMaterial::WATER) {
                    pos.fields.y = hitInfo.fields.m_Point.fields.y;
                    Logger::log("[LotSwayGrass][Raycast] HIT newY=%.3f colliderId=%d\n",
                                hitInfo.fields.m_Point.fields.y,
                                (int)hitInfo.fields.m_Collider);
                }

                else if (!raycastHit) {
                    //
                    Logger::log("[LotSwayGrass][Raycast] MISS keepY=%.3f\n", pos.fields.y);
                }

                else {
                    pos.fields.y = height;
                }

                // "GrassData" slot
                if (gData->m_Items[selectedPatch] == nullptr) {
                    gData->m_Items[selectedPatch] = Dpr::Field::SwayGrass::GrassData::newInstance();
                    Logger::log("[LotSwayGrass] Allocated GrassData slot %d\n", selectedPatch);
                    // Attricode is automatically set to -1 in ctor
                }

                auto g = gData->m_Items[selectedPatch];
                g->fields.position = pos;
                g->fields.enable = raycastHit;
                g->fields.effectTime = 0.0f;
                g->fields.random_iro = RandomGroupWork::RandomValue(iroRange);
                g->fields.random_kakure = 128;
                g->fields.rank = rank->m_Items[poolIndex];
                g->fields.attricode = attrCode->m_Items[poolIndex];

                Logger::log("[LotSwayGrass][Patch] #%d pos=(%.2f,%.2f,%.2f) enable=%d rank=%d attr=%d\n",
                            selectedPatch,
                            g->fields.position.fields.x,
                            g->fields.position.fields.y,
                            g->fields.position.fields.z,
                            (int)g->fields.enable,
                            g->fields.rank,
                            g->fields.attricode
                );

                rank->m_Items[poolIndex] = -1;
                threshold -= 2;
                if (threshold < 1) threshold = 1;

                selectedPatch++;
            }
        }

        bool anyEnabled = false;
        for (int i = 0; i < selectedPatch; i++) {
            if (gData->m_Items[i]->fields.enable) {
                anyEnabled = true;
                break;
            }
        }

        if (!anyEnabled)
            return 1;

        Dpr::Field::SwayGrass::getClass()->initIfNeeded();
        Dpr::Field::SwayGrass::getClass()->static_fields->swayZone = PlayerWork::get_zoneID();
        Dpr::Field::SwayGrass::getClass()->static_fields->is_swaygrass_flag = true;
        TairyouHasseiPokeManager::getClass()->static_fields->ForceStop = true;

        Logger::log("[LotSwayGrass] Calling Sway BGM...\n");
        Dpr::Field::SwayGrass::getClass()->static_fields->_callSwayBGM = true;
        return 2;
    }
};

HOOK_DEFINE_REPLACE(SwayGrass$$PlayEffect) {
    static void Callback(float deltatime, int32_t i) {
        system_load_typeinfo(0x8257);
        Dpr::Field::SwayGrass::getClass()->initIfNeeded();
        auto gDataPatch = Dpr::Field::SwayGrass::getClass()->static_fields->grass_data->m_Items[i];

        if (gDataPatch == nullptr || i < 0)
            return;

        if (gDataPatch->fields.effectTime > 0.0f) {
            gDataPatch->fields.effectTime -= deltatime;
            return;
        }

        int32_t effectIndex;
        switch (gDataPatch->fields.attricode) {
            case 0xA9: {
                effectIndex = 0x131;
                break;
            }
            case 0x5: {
                effectIndex = 0x12F;
                break;
            }
            default: {
                effectIndex = 0x12D;
                break;
            }
        }

        FieldManager::getClass()->initIfNeeded();
        auto manager = FieldManager::getClass()->static_fields->_Instance_k__BackingField->instance();

        if (gDataPatch->fields.random_kakure == 0) {
            effectIndex += 1;
        }

        Logger::log("[PlayEffect] Calling effect %d\n", effectIndex);
        manager->CallEffect(effectIndex, gDataPatch->fields.position, nullptr, nullptr);
        Logger::log("[PlayEffect] Called effect %d\n", effectIndex);

        if (gDataPatch->fields.random_iro == 0)
            manager->CallEffect(0xE, gDataPatch->fields.position, nullptr, nullptr);

        gDataPatch->fields.effectTime = (RandomGroupWork::Value() * 15.0f + 1.0f) / 30.0f;
    }
};

HOOK_DEFINE_TRAMPOLINE(SwayGrass$$CheckSpEncount) {
    static bool Callback(Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object* info,UnityEngine::Vector3::Object* pos,float size) {
        auto res = Orig(info, pos, size);
        Logger::log("[CheckSpEncount] %s\n", res ? "True" : "False");
        return res;
    }
};

HOOK_DEFINE_TRAMPOLINE(SwayGrass$$CheckSwayGrass) {
    static int32_t Callback(UnityEngine::Vector3::Object* pos, float size) {
        auto res = Orig(pos, size);
        Logger::log("[CheckSwayGrass] %d\n", res);
        return res;
    }
};


void exl_expanded_radar_main() {
    SwayGrass$$LotSwayGrass::InstallAtOffset(0x019bb540);
    SwayGrass$$PlayEffect::InstallAtOffset(0x019bc950);
    //SwayGrass$$CheckSpEncount::InstallAtOffset(0x019b43d0);
    //SwayGrass$$CheckSwayGrass::InstallAtOffset(0x019bafe0);
    //EvCmd_POKETORE_START::InstallAtOffset(0x02c99b04);
    FieldManager$$CheckAvailableFieldItem::InstallAtOffset(0x017a2aa0);
}