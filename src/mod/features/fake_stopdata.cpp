#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldEventEntity.h"
#include "externals/FlagWork.h"
#include "externals/System/String.h"

#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(Dpr_EvScript_EvDataManager$$OnEventEnter) {
    static void Callback(Dpr::EvScript::EvDataManager::Object* __this, float deltaTime, FieldEventEntity::Object* eventEntity) {
        Orig(__this, deltaTime, eventEntity);

        system_load_typeinfo(0x4382);

        auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
        if (player->fields._currentSequence_k__BackingField != 4096)
            return;

        // Not any of these types
        if (__this->fields._liftEntity == nullptr &&
            __this->fields._nagisaGymGearEntity == nullptr &&
            __this->fields._nomoseGymSwitchEntity == nullptr &&
            __this->fields._eyePaintingEntity == nullptr &&
            __this->fields._embankmentEntity == nullptr &&
            __this->fields._doorEntity == nullptr) {

            Logger::log("[Fake StopData] Not any specific type\n");

            // Starts with "STOPDATA_"
            if (eventEntity->fields._enityName->StartsWith(System::String::Create("STOPDATA_"))) {

                Logger::log("[Fake StopData] Starts with \"STOPDATA_\"\n");

                // Grab the script name after the prefix
                auto script = eventEntity->fields._enityName->Substring(9);

                // Check the assigned work and that the script name is not empty
                if ((eventEntity->fields.locatorIndex == 500 || FlagWork::GetWork(eventEntity->fields.locatorIndex) == eventEntity->fields.clipIndex) &&
                    !System::String::IsNullOrEmpty(script)) {

                    Logger::log("[Fake StopData] Work is good\n");
                    Logger::log("[Fake StopData] checkHeight is %d\n", eventEntity->fields.checkHeight);

                    __this->PlayerInputActive(false, false);
                    /*UnityEngine::RectInt::Object rect = { .fields = {
                            .m_XMin =   (int32_t)-eventEntity->cast<UnityEngine::Component>()->get_transform()->get_position().fields.x,
                            .m_YMin =   (int32_t)eventEntity->cast<UnityEngine::Component>()->get_transform()->get_position().fields.z - 1,
                            .m_Width =  (int32_t)eventEntity->fields.size.fields.x,
                            .m_Height = (int32_t)eventEntity->fields.size.fields.y,
                        }
                    };
                    auto playerNowGrid = FieldObjectEntity::PositionToGrid(player->fields.worldPosition);
                    __this->fields._heroMoveGridCenterFrontDir = __this->SetupHeroMoveGridCenterFrontDir(&rect, &playerNowGrid, &player->fields._oldGridPosition_k__BackingField);*/
                    player->StopCrossInputAndBicycle();
                    __this->JumpLabel(script, nullptr);

                    Logger::log("[Fake StopData] Script %s should start soon\n", script->asCString().c_str());
                }
            }
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(FieldEventEntity$$Awake) {
    static void Callback(FieldEventEntity::Object* __this) {
        Orig(__this);

        // Starts with "STOPDATA_"
        if (__this->fields._enityName->StartsWith(System::String::Create("STOPDATA_"))) {
            __this->fields.checkHeight = true;
        }
    }
};

void exl_fake_stopdata_main() {
    Dpr_EvScript_EvDataManager$$OnEventEnter::InstallAtOffset(0x02c4cd00);
    FieldEventEntity$$Awake::InstallAtOffset(0x01792d50);
};