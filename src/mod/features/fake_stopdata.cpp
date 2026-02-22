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

            // Starts with "STOPDATA_"
            if (eventEntity->fields._enityName->StartsWith(System::String::Create("STOPDATA_"))) {

                // Grab the script name after the prefix
                auto script = eventEntity->fields._enityName->Substring(9);

                // Check the assigned work and that the script name is not empty
                if ((eventEntity->fields.locatorIndex == 500 || FlagWork::GetWork(eventEntity->fields.locatorIndex) == eventEntity->fields.clipIndex) &&
                    !System::String::IsNullOrEmpty(script)) {

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