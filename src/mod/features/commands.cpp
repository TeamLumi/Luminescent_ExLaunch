#include "exlaunch.hpp"
#include "externals/Dpr/EvScript/EvCmdID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"

#include "features/commands/commands.h"

static bool ACTIVATED_COMMANDS[(int32_t)Dpr::EvScript::EvCmdID::NAME::CUSTOM_CMD_END];

void SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME command)
{
    ACTIVATED_COMMANDS[(int32_t)command] = true;
}

bool HandleCmdStepper(bool result)
{
    // Set the script as frozen if the Debug Stepper is on
    if (FlagWork::GetSysFlag(FlagWork_SysFlag::SYSFLAG_996) && result)
        FlagWork::SetSysFlag(FlagWork_SysFlag::SYSFLAG_997, true);
    return result;
}

// Handles overriden and new script commands, then calls the original method to handle the rest normally.
HOOK_DEFINE_TRAMPOLINE(RunEvCmdCustom) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object* __this, int32_t index) {
        // If Debug Stepper is on, and we're frozen
        if (FlagWork::GetSysFlag(FlagWork_SysFlag::SYSFLAG_996) && FlagWork::GetSysFlag(FlagWork_SysFlag::SYSFLAG_997))
            return false;

        // Overriden/New Commands
        if (ACTIVATED_COMMANDS[index])
        {
            switch ((Dpr::EvScript::EvCmdID::NAME)index)
            {
                case Dpr::EvScript::EvCmdID::NAME::_SET_WEATHER:
                    return HandleCmdStepper(SetWeather(__this));
                case Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_SELECT_PROC:
                    return HandleCmdStepper(FirstPokeSelectProc(__this));
                case Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_NO_GET:
                    return HandleCmdStepper(FirstPokeNoGet(__this));
                case Dpr::EvScript::EvCmdID::NAME::_HONEY_TREE_BTL_SET:
                    return HandleCmdStepper(HoneyTreeBattleSet(__this));
                case Dpr::EvScript::EvCmdID::NAME::_STOP_EFFECT:
                    return HandleCmdStepper(StopEffect(__this));
                case Dpr::EvScript::EvCmdID::NAME::_TEMOTI_FORMNO:
                    return HandleCmdStepper(PartyFormNo(__this));
                case Dpr::EvScript::EvCmdID::NAME::_TEMOTI_BOX_FORMNO:
                    return HandleCmdStepper(PartyBoxFormNo(__this));
                case Dpr::EvScript::EvCmdID::NAME::_GET_BOX_POKE_SEIKAKU:
                    return HandleCmdStepper(PartyBoxNature(__this));
                case Dpr::EvScript::EvCmdID::NAME::_RELEASE_BOX_POKE:
                    return HandleCmdStepper(PartyBoxRelease(__this));
                case Dpr::EvScript::EvCmdID::NAME::_TOGGLE_COLLISION_BOX:
                    return HandleCmdStepper(ToggleCollisionBox(__this));
                case Dpr::EvScript::EvCmdID::NAME::_INSTALL_CHECK:
                    return HandleCmdStepper(InstallCheck(__this));
                case Dpr::EvScript::EvCmdID::NAME::_SET_PLAYER_COLOR_INDEX:
                    return HandleCmdStepper(SetPlayerColorIndex(__this));
                case Dpr::EvScript::EvCmdID::NAME::_RIVAL_POKE_NO_GET:
                    return HandleCmdStepper(RivalPokeNoGet(__this));
                case Dpr::EvScript::EvCmdID::NAME::_SUPPORT_POKE_NO_GET:
                    return HandleCmdStepper(SupportPokeNoGet(__this));
                case Dpr::EvScript::EvCmdID::NAME::_FIRST_MONSNO_FORMNO:
                    return HandleCmdStepper(PlayerStarterMonsNoFormNo(__this));
                case Dpr::EvScript::EvCmdID::NAME::_RIVAL_MONSNO_FORMNO:
                    return HandleCmdStepper(RivalStarterMonsNoFormNo(__this));
                case Dpr::EvScript::EvCmdID::NAME::_SUPPORT_MONSNO_FORMNO:
                    return HandleCmdStepper(SupportStarterMonsNoFormNo(__this));
                case Dpr::EvScript::EvCmdID::NAME::_CHANGE_FORMNO:
                    return HandleCmdStepper(ChangeFormNo(__this));
                case Dpr::EvScript::EvCmdID::NAME::_GET_COSTUME_GENDER:
                    return HandleCmdStepper(GetCostumeGender(__this));
                case Dpr::EvScript::EvCmdID::NAME::_CASE_CALL:
                    return HandleCmdStepper(CaseCall(__this));
                case Dpr::EvScript::EvCmdID::NAME::_ADD_POKEMON_UI_EXTRA:
                    return HandleCmdStepper(AddPokemonUIExtra(__this));
                case Dpr::EvScript::EvCmdID::NAME::_POKEMON_NAME_FORM:
                    return HandleCmdStepper(PokemonNameForm(__this));
                case Dpr::EvScript::EvCmdID::NAME::_SET_AYOU_NAME:
                    return HandleCmdStepper(SetAYouName(__this));
                case Dpr::EvScript::EvCmdID::NAME::_AYOU_NAME:
                    return HandleCmdStepper(AYouName(__this));
                case Dpr::EvScript::EvCmdID::NAME::_GET_CAUGHT_LOCATION:
                    return HandleCmdStepper(GetCaughtLocation(__this));
                case Dpr::EvScript::EvCmdID::NAME::_CHECK_TUTOR_MOVE:
                    return HandleCmdStepper(CheckTutorMove(__this));
                case Dpr::EvScript::EvCmdID::NAME::_MOVE_TUTOR_UI:
                    return HandleCmdStepper(MoveTutorUI(__this));
                case Dpr::EvScript::EvCmdID::NAME::_GET_HIGHEST_RADAR_STREAK:
                    return HandleCmdStepper(GetHighestRadarStreak(__this));
                case Dpr::EvScript::EvCmdID::NAME::_GET_TILE_ATTRIBUTE:
                    return HandleCmdStepper(GetTileAttribute(__this));
                default:
                    break;
            }
        }

        // Call original method
        return HandleCmdStepper(Orig(__this, index));
    }
};

void exl_commands_main() {
    RunEvCmdCustom::InstallAtOffset(0x02c5b290);

    // Select which new commands/overrides are activated
    for (bool & i : ACTIVATED_COMMANDS)
        i = false;

    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SET_WEATHER);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_SELECT_PROC);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_NO_GET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_HONEY_TREE_BTL_SET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_STOP_EFFECT);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_TEMOTI_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_TEMOTI_BOX_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_GET_BOX_POKE_SEIKAKU);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_RELEASE_BOX_POKE);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_TOGGLE_COLLISION_BOX);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_INSTALL_CHECK);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SET_PLAYER_COLOR_INDEX);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_RIVAL_POKE_NO_GET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SUPPORT_POKE_NO_GET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_FIRST_MONSNO_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_RIVAL_MONSNO_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SUPPORT_MONSNO_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_CHANGE_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_GET_COSTUME_GENDER);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_CASE_CALL);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_ADD_POKEMON_UI_EXTRA);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_POKEMON_NAME_FORM);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SET_AYOU_NAME);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_AYOU_NAME);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_GET_CAUGHT_LOCATION);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_CHECK_TUTOR_MOVE);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_MOVE_TUTOR_UI);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_GET_HIGHEST_RADAR_STREAK);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_GET_TILE_ATTRIBUTE);
}
