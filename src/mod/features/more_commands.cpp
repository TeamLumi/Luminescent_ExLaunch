#include <cstring>
#include <utility>
#include <vector>

#include "exlaunch.hpp"
#include "externals/Dpr/Box/BoxPokemonWork.h"
#include "externals/Dpr/EvScript/EvCmdID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/EvScript/EvScriptData.h"
#include "externals/EvData/Aregment.h"
#include "externals/EvData/ArgType.h"
#include "externals/EvData/_EvData.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "externals/SmartPoint/Components/DebugMenu.h"
#include "externals/UnityEngine/BoxCollider.h"
#include "externals/UnityEngine/Collider.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/WeatherWork.h"

#include "logger/logger.h"


// UTILITY METHODS

// Converts the IEEE-754 formatted int32 into its proper float value.
float ConvertToFloat(int32_t value)
{
    float fresult;
    std::memcpy(&fresult, &value, sizeof(value));
    return fresult;
}

// Returns either the value at the given work variable or the direct int value depending on the given argument's type.
// Returns 0 for any other argument type.
int32_t GetWorkOrIntValue(EvData::Aregment::Object arg)
{
    EvData::ArgType argType = (EvData::ArgType)arg.fields.argType;
    int32_t data = arg.fields.data;
    int32_t result = 0;

    switch (argType)
    {
        case EvData::ArgType::Work:
            result = PlayerWork::GetInt(data);
            break;
        case EvData::ArgType::Float:
            result = (int32_t)ConvertToFloat(data);
            break;
        default:
            break;
    }

    return result;
}

// Returns either the value at the given work variable or the direct float value depending on the given argument's type.
// Returns 0 for any other argument type.
float GetWorkOrFloatValue(EvData::Aregment::Object arg)
{
    EvData::ArgType argType = (EvData::ArgType)arg.fields.argType;
    int32_t data = arg.fields.data;
    float result = 0;

    switch (argType)
    {
        case EvData::ArgType::Work:
            result = PlayerWork::GetInt(data);
            break;
        case EvData::ArgType::Float:
            result = ConvertToFloat(data);
            break;
        default:
            break;
    }

    return result;
}

// If the given argument is a work variable, set it to the given value.
// Does nothing otherwise.
void SetWorkToValue(EvData::Aregment::Object arg, int32_t value)
{
    EvData::ArgType argType = (EvData::ArgType)arg.fields.argType;
    int32_t data = arg.fields.data;

    if (argType == EvData::ArgType::Work)
    {
        Logger::log("Setting work %d to value %d\n", data, value);
        PlayerWork::SetInt(data, value);
    }
}

// Checks if the given PokemonParam is null or an egg.
bool IsNullOrEgg(Pml::PokePara::PokemonParam::Object * param)
{
    Pml::PokePara::CoreParam::Object * coreParam = (Pml::PokePara::CoreParam::Object *)param;
    return coreParam == nullptr || coreParam->IsNull() || coreParam->IsEgg(2);
}


// SCRIPT COMMAND METHODS

// Sets the weather.
// Arguments:
//   [Work, Number] weather: The ID of the weather to change to.
bool SetWeather(Dpr::EvScript::EvDataManager::Object * manager)
{
    Logger::log("_SET_WEATHER\n");

    return true;
}

// Stops a Field Effect.
// Arguments:
//   [Work, Number] index: The index of the field effect to stop. 0-10
//   [Work, Number] isForce: Unknown use.
//   [Work, Number] fadeTime: The time in seconds it takes for the effect to fade.
bool StopEffect(Dpr::EvScript::EvDataManager::Object * manager)
{
    Logger::log("_STOP_EFFECT\n");
    system_load_typeinfo(0x45a2);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t isForce = GetWorkOrIntValue(args->m_Items[2]);

            float fadeTime = 0;
            if (args->max_length >= 4)
            {
                fadeTime = GetWorkOrFloatValue(args->m_Items[3]);
            }

            if (index < (int32_t)manager->fields._scriptEffects->max_length)
            {
                if (manager->fields._scriptEffects->m_Items[index] != nullptr)
                {
                    Logger::log("Stopping Effect %d with isForce %d in %f seconds\n", index, isForce, fadeTime);
                    manager->fields._scriptEffects->m_Items[index]->Stop(fadeTime, isForce == 1);
                }

                if (index < (int32_t)manager->fields._scriptScaleCorutine->max_length)
                {
                    if (manager->fields._scriptScaleCorutine->m_Items[index] != nullptr)
                    {
                        SmartPoint::AssetAssistant::Sequencer::Stop(manager->fields._scriptScaleCorutine->m_Items[index]);
                    }
                }
            }
        }
    }

    return true;
}

// Returns the form id of the Pokémon at the given index in the party.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work] result: The work in which to put the result in. -1 if the chosen Pokémon is null or an egg.
bool PartyFormsNo(Dpr::EvScript::EvDataManager::Object * manager)
{
    Logger::log("_TEMOTI_FORMNO\n");
    system_load_typeinfo(0x44f9);
    
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        Pml::PokeParty::Object * party = PlayerWork::get_playerParty();
        Pml::PokePara::PokemonParam::Object * param = party->GetMemberPointer(index);
        int32_t result = -1;

        if (!IsNullOrEgg(param))
        {
            Logger::log("Calling GetPokemonFormNo with party index: %d\n", index);
            result = manager->GetPokemonFormNo(param);
        }

        if (args->max_length >= 3)
        {
            SetWorkToValue(args->m_Items[2], result);
        }
    }

    return true;
}

// Returns the form id of the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
//   [Work] result: The work in which to put the result in. -1 if the chosen Pokémon is null or an egg.
bool PartyBoxFormsNo(Dpr::EvScript::EvDataManager::Object * manager)
{
    Logger::log("_TEMOTI_BOX_FORMNO\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

            Pml::PokePara::PokemonParam::Object * param = manager->GetPokemonParam(trayIndex, index);
            int32_t result = -1;

            if (!IsNullOrEgg(param))
            {
                Logger::log("Calling GetPokemonFormNo with tray index %d and index %d\n", trayIndex, index);
                result = manager->GetPokemonFormNo(param);
            }

            if (args->max_length >= 4)
            {
                SetWorkToValue(args->m_Items[3], result);
            }
        }
    }

    return true;
}

// Returns the nature id of the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
//   [Work] result: The work in which to put the result in. -1 if the chosen Pokémon is null or an egg.
bool PartyBoxNature(Dpr::EvScript::EvDataManager::Object * manager)
{
    Logger::log("_GET_BOX_POKE_SEIKAKU\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

            Pml::PokePara::PokemonParam::Object * param = manager->GetPokemonParam(trayIndex, index);
            int32_t result = -1;

            if (!IsNullOrEgg(param))
            {
                Logger::log("Calling GetSeikaku with tray index %d and index %d\n", trayIndex, index);
                result = ((Pml::PokePara::CoreParam *)param)->GetSeikaku();
            }

            if (args->max_length >= 4)
            {
                SetWorkToValue(args->m_Items[3], result);
            }
        }
    }

    return true;
}

// Releases the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
bool PartyBoxRelease(Dpr::EvScript::EvDataManager::Object * manager)
{
    Logger::log("_RELEASE_BOX_POKE\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

            Pml::PokePara::PokemonParam::Object * param = manager->GetPokemonParam(trayIndex, index);

            if (!IsNullOrEgg(param))
            {
                Logger::log("Calling ClearPokemon with tray index %d and index %d\n", trayIndex, index);
                Dpr::Box::BoxPokemonWork::Object::ClearPokemon(trayIndex, index);

            }
        }
    }

    return true;
}

// Toggles a BoxCollider of the given name on the current map.
// Arguments:
//   [Label] label: The name of the BoxCollider to toggle.
bool ToggleCollisionBox(Dpr::EvScript::EvDataManager::Object * manager)
{
    Logger::log("_TOGGLE_COLLISION_BOX\n");
    system_load_typeinfo(0x5b6);
    EvData::Aregment::Array* args = manager->fields._evArg;
    Dpr::EvScript::EvScriptData::Object * evData = manager->fields._evData;

    if (args->max_length >= 2)
    {
        if ((EvData::ArgType)args->m_Items[1].fields.argType == EvData::ArgType::String)
        {
            System::String::Object * objectName = evData->fields.EvData->GetString(args->m_Items[1].fields.data);
            UnityEngine::GameObject::Object* object = UnityEngine::GameObject::Object::Find(objectName);

            if (object != nullptr)
            {
                UnityEngine::BoxCollider::Object * boxCollider = (UnityEngine::BoxCollider::Object *)object->GetComponent(UnityEngine::Component::Method$$BoxCollider$$GetComponent);
                if (boxCollider != nullptr)
                {
                    UnityEngine::Collider::Object * collider = (UnityEngine::Collider::Object *)boxCollider;

                    if (args->max_length >= 3)
                    {
                        int32_t value = GetWorkOrIntValue(args->m_Items[2]);
                        Logger::log("Setting the collider to: %0d\n", value != 0);
                        collider->set_enabled(value != 0);
                    }
                }
            }
        }
    }

    return true;
}

// Handles overriden and new script commands, then calls the original method to handle the rest normally.
HOOK_DEFINE_TRAMPOLINE(RunEvCmdCustom) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object *__this, int32_t index) {
        // Overriden/New Commands
        switch ((Dpr::EvScript::EvCmdID::NAME)index)
        {
            case Dpr::EvScript::EvCmdID::NAME::_SET_WEATHER:
                return SetWeather(__this);
            case Dpr::EvScript::EvCmdID::NAME::_STOP_EFFECT:
                return StopEffect(__this);
            case Dpr::EvScript::EvCmdID::NAME::_TEMOTI_FORMNO:
                return PartyFormsNo(__this);
            case Dpr::EvScript::EvCmdID::NAME::_TEMOTI_BOX_FORMNO:
                return PartyBoxFormsNo(__this);
            case Dpr::EvScript::EvCmdID::NAME::_GET_BOX_POKE_SEIKAKU:
                return PartyBoxNature(__this);
            case Dpr::EvScript::EvCmdID::NAME::_RELEASE_BOX_POKE:
                return PartyBoxRelease(__this);
            case Dpr::EvScript::EvCmdID::NAME::_TOGGLE_COLLISION_BOX:
                return ToggleCollisionBox(__this);
            default:
                break;
        }

        // Call original method
        return Orig(__this, index);
    }
};


void exl_commands_main() {
    RunEvCmdCustom::InstallAtOffset(0x02c5b290);
}
