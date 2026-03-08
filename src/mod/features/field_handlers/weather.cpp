#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_CLIENT.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Handler/Tokusei.h"
#include "externals/Dpr/Battle/Logic/Section_ChangeWeather_Check.h"
#include "externals/Dpr/Battle/Logic/Section_FieldEffect_End.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"

#include "data/utils.h"
#include "data/weathers.h"
#include "field_handlers.h"

static ExtraFieldEffectWeatherHandlers extraFieldEffectWeatherHandlers {};
static bool ACTIVATED_FIELD_WEATHER_HANDLERS[BATTLE_WEATHER_COUNT];

void SetActivatedFieldWeatherHandlers(int32_t weather)
{
    ACTIVATED_FIELD_WEATHER_HANDLERS[weather] = true;
    getExtraFieldEffectWeatherHandlers()->count++;
}

ExtraFieldEffectWeatherHandlers* getExtraFieldEffectWeatherHandlers() {
    return &extraFieldEffectWeatherHandlers;
}

using namespace Dpr::Battle::Logic;

// Types that are more/less effective under specific weather, keep at 1x.
HOOK_DEFINE_REPLACE(calc$$GetWeatherDmgRatio) {
    static int32_t Callback(uint8_t weather, uint8_t wazaType) {
        return FX32::CONST_1_0;
    }
};

bool IsStrongWeather(uint8_t weather) {
    return weather == array_index(BATTLE_WEATHERS, "Heavy Rain") ||
           weather == array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight") ||
           weather == array_index(BATTLE_WEATHERS, "Strong Winds");
}

bool IsRegularWeather(uint8_t weather) {
    return weather == array_index(BATTLE_WEATHERS, "Harsh Sunlight") ||
           weather == array_index(BATTLE_WEATHERS, "Rain") ||
           weather == array_index(BATTLE_WEATHERS, "Hail") ||
           weather == array_index(BATTLE_WEATHERS, "Sandstorm") ||
           weather == array_index(BATTLE_WEATHERS, "Snow");
}

HOOK_DEFINE_REPLACE(Handler_Field$$ADD_Fld_Weather) {
    static EventFactor::EventHandlerTable::Array* Callback(uint8_t sub_param) {
        system_load_typeinfo(0x4b38);

        if (sub_param >= BATTLE_WEATHER_COUNT || !ACTIVATED_FIELD_WEATHER_HANDLERS[sub_param])
            return Handler::Field::getClass()->static_fields->DammyHandlerTable;

        switch (sub_param)
        {
            case array_index(BATTLE_WEATHERS, "Harsh Sunlight"):
                return ADD_FieldEffect_Weather_HarshSunlight();

            case array_index(BATTLE_WEATHERS, "Rain"):
                return ADD_FieldEffect_Weather_Rain();

            case array_index(BATTLE_WEATHERS, "Hail"):
                return ADD_FieldEffect_Weather_Hail();

            case array_index(BATTLE_WEATHERS, "Sandstorm"):
                return ADD_FieldEffect_Weather_Sandstorm();

            case array_index(BATTLE_WEATHERS, "Heavy Rain"):
                return ADD_FieldEffect_Weather_HeavyRain();

            case array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"):
                return ADD_FieldEffect_Weather_ExtremelyHarshSunlight();

            case array_index(BATTLE_WEATHERS, "Strong Winds"):
                return ADD_FieldEffect_Weather_StrongWinds();

            case array_index(BATTLE_WEATHERS, "Snow"):
                return ADD_FieldEffect_Weather_Snow();

            default:
                return Handler::Field::getClass()->static_fields->DammyHandlerTable;
        }
    }
};

// Damage dealt by weather at the end of a turn.
HOOK_DEFINE_REPLACE(calc$$RecvWeatherDamage) {
    static uint16_t Callback(BTL_POKEPARAM::Object* bpp, uint8_t weather) {

        if (weather >= BATTLE_WEATHER_COUNT || !ACTIVATED_FIELD_WEATHER_HANDLERS[weather])
            return 0;

        switch (weather)
        {
            case array_index(BATTLE_WEATHERS, "Harsh Sunlight"):
                return FieldEffect_Weather_HarshSunlight_Damage(bpp);

            case array_index(BATTLE_WEATHERS, "Rain"):
                return FieldEffect_Weather_Rain_Damage(bpp);

            case array_index(BATTLE_WEATHERS, "Hail"):
                return FieldEffect_Weather_Hail_Damage(bpp);

            case array_index(BATTLE_WEATHERS, "Sandstorm"):
                return FieldEffect_Weather_Sandstorm_Damage(bpp);

            case array_index(BATTLE_WEATHERS, "Heavy Rain"):
                return FieldEffect_Weather_HeavyRain_Damage(bpp);

            case array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"):
                return FieldEffect_Weather_ExtremelyHarshSunlight_Damage(bpp);

            case array_index(BATTLE_WEATHERS, "Strong Winds"):
                return FieldEffect_Weather_StrongWinds_Damage(bpp);

            case array_index(BATTLE_WEATHERS, "Snow"):
                return FieldEffect_Weather_Snow_Damage(bpp);

            default:
                return 0;
        }
    }
};

// Message that appears when damage is dealt by weather at the end of a turn.
HOOK_DEFINE_REPLACE(Section_TurnCheck_Weather$$weatherDamage_Message) {
    static void Callback(Section_TurnCheck_Weather::Object* __this, uint8_t weather, uint8_t pokeID) {

        StrParam::Object* strParam = StrParam::newInstance();
        if (weather >= BATTLE_WEATHER_COUNT || !ACTIVATED_FIELD_WEATHER_HANDLERS[weather])
        {
            strParam->Clear();
        }
        else
        {
            switch (weather)
            {
                case array_index(BATTLE_WEATHERS, "Harsh Sunlight"):
                    FieldEffect_Weather_HarshSunlight_DamageStrParam(strParam, __this, pokeID);
                    break;

                case array_index(BATTLE_WEATHERS, "Rain"):
                    FieldEffect_Weather_Rain_DamageStrParam(strParam, __this, pokeID);
                    break;

                case array_index(BATTLE_WEATHERS, "Hail"):
                    FieldEffect_Weather_Hail_DamageStrParam(strParam, __this, pokeID);
                    break;

                case array_index(BATTLE_WEATHERS, "Sandstorm"):
                    FieldEffect_Weather_Sandstorm_DamageStrParam(strParam, __this, pokeID);
                    break;

                case array_index(BATTLE_WEATHERS, "Heavy Rain"):
                    FieldEffect_Weather_HeavyRain_DamageStrParam(strParam, __this, pokeID);
                    break;

                case array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"):
                    FieldEffect_Weather_ExtremelyHarshSunlight_DamageStrParam(strParam, __this, pokeID);
                    break;

                case array_index(BATTLE_WEATHERS, "Strong Winds"):
                    FieldEffect_Weather_StrongWinds_DamageStrParam(strParam, __this, pokeID);
                    break;

                case array_index(BATTLE_WEATHERS, "Snow"):
                    FieldEffect_Weather_Snow_DamageStrParam(strParam, __this, pokeID);
                    break;

                default:
                    strParam->Clear();
                    break;
            }
        }

        __this->fields.m_pServerCmdPutter->Message(&strParam);
    }
};

// Finds the strID for the weather starting.
HOOK_DEFINE_REPLACE(BTL_CLIENT$$getWeatherStartMessage) {
    static int32_t Callback(BTL_CLIENT::Object* __this, uint8_t weather, ChangeWeatherCause cause) {
        if (weather >= BATTLE_WEATHER_COUNT || !ACTIVATED_FIELD_WEATHER_HANDLERS[weather])
        {
            return -1;
        }
        else
        {
            switch (weather)
            {
                case array_index(BATTLE_WEATHERS, "Harsh Sunlight"):
                    return FieldEffect_Weather_HarshSunlight_StartStrID(__this, cause);

                case array_index(BATTLE_WEATHERS, "Rain"):
                    return FieldEffect_Weather_Rain_StartStrID(__this, cause);

                case array_index(BATTLE_WEATHERS, "Hail"):
                    return FieldEffect_Weather_Hail_StartStrID(__this, cause);

                case array_index(BATTLE_WEATHERS, "Sandstorm"):
                    return FieldEffect_Weather_Sandstorm_StartStrID(__this, cause);

                case array_index(BATTLE_WEATHERS, "Heavy Rain"):
                    return FieldEffect_Weather_HeavyRain_StartStrID(__this, cause);

                case array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"):
                    return FieldEffect_Weather_ExtremelyHarshSunlight_StartStrID(__this, cause);

                case array_index(BATTLE_WEATHERS, "Strong Winds"):
                    return FieldEffect_Weather_StrongWinds_StartStrID(__this, cause);

                case array_index(BATTLE_WEATHERS, "Snow"):
                    return FieldEffect_Weather_Snow_StartStrID(__this, cause);

                default:
                    return -1;
            }
        }
    }
};

// Finds the strID for the weather ending.
int32_t GetWeatherEndStrID(BTL_CLIENT::Object* client, uint8_t weather)
{
    if (weather >= BATTLE_WEATHER_COUNT || !ACTIVATED_FIELD_WEATHER_HANDLERS[weather])
    {
        return -1;
    }
    else
    {
        switch (weather)
        {
            case array_index(BATTLE_WEATHERS, "Harsh Sunlight"):
                return FieldEffect_Weather_HarshSunlight_EndStrID(client);

            case array_index(BATTLE_WEATHERS, "Rain"):
                return FieldEffect_Weather_Rain_EndStrID(client);

            case array_index(BATTLE_WEATHERS, "Hail"):
                return FieldEffect_Weather_Hail_EndStrID(client);

            case array_index(BATTLE_WEATHERS, "Sandstorm"):
                return FieldEffect_Weather_Sandstorm_EndStrID(client);

            case array_index(BATTLE_WEATHERS, "Heavy Rain"):
                return FieldEffect_Weather_HeavyRain_EndStrID(client);

            case array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"):
                return FieldEffect_Weather_ExtremelyHarshSunlight_EndStrID(client);

            case array_index(BATTLE_WEATHERS, "Strong Winds"):
                return FieldEffect_Weather_StrongWinds_EndStrID(client);

            case array_index(BATTLE_WEATHERS, "Snow"):
                return FieldEffect_Weather_Snow_EndStrID(client);

            default:
                return -1;
        }
    }
}

// Handles the weather ending.
HOOK_DEFINE_REPLACE(BTL_CLIENT$$scProc_ACT_WeatherEnd) {
    static bool Callback(BTL_CLIENT::Object* __this, int32_t* seq, System::Int32_array* args) {
        switch (*seq)
        {
            case 0:
            {
                auto strID = GetWeatherEndStrID(__this, args->m_Items[0]);
                if (strID < 0)
                    return true;

                __this->fields._m_viewCore->virtual_CMD_StartMsgStd((uint16_t)strID, nullptr);
                (*seq)++;
                return false;
            }

            case 1:
            {
                if (!__this->fields._m_viewCore->virtual_CMD_WaitMsg())
                    return false;

                __this->fields._m_viewCore->virtual_CMD_ChangeWheather(0);
                (*seq)++;
                return false;
            }

            case 2:
            {
                if (!__this->fields._m_viewCore->virtual_CMD_ChangeWheather_Wait())
                    return false;

                return true;
            }

            default:
                return false;
        }
    }
};

// Weathers that are considered "Harsh Sunlight" for the purposes of stuff like Cherrim's forms.
HOOK_DEFINE_REPLACE(calc$$IsShineWeather) {
    static bool Callback(uint8_t weather) {
        return ACTIVATED_FIELD_WEATHER_HANDLERS[weather] &&
            (weather == array_index(BATTLE_WEATHERS, "Harsh Sunlight") ||
             weather == array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"));
    }
};

// Weathers that are considered "Rain" for the purposes of stuff like Thunder's accuracy.
HOOK_DEFINE_REPLACE(calc$$IsRainWeather) {
    static bool Callback(uint8_t weather) {
        return ACTIVATED_FIELD_WEATHER_HANDLERS[weather] &&
           (weather == array_index(BATTLE_WEATHERS, "Rain") ||
            weather == array_index(BATTLE_WEATHERS, "Heavy Rain"));
    }
};

// Adjust if weathers can be replaced by others.
HOOK_DEFINE_REPLACE(Section_ChangeWeather_Check$$Execute) {
    static void Callback(Section_ChangeWeather_Check::Object* __this, Section_ChangeWeather_Check::Result::Object* pResult, Section_ChangeWeather_Check::Description::Object** description) {
        auto newWeather = (*description)->fields.weather;
        if ((*description)->fields.weather >= BATTLE_WEATHER_COUNT) {
            pResult->fields.result = ChangeWeatherResult::FAIL;
            return;
        }

        auto currWeather = __this->fields.m_pBattleEnv->instance()->fields.m_fieldStatus->GetWeather();
        if (IsRegularWeather(newWeather) && IsStrongWeather(currWeather)) {
            pResult->fields.result = ChangeWeatherResult::FAIL_CANT_OVERWRITE;
            return;
        }

        auto remainingTurns = __this->fields.m_pBattleEnv->instance()->fields.m_fieldStatus->GetWeatherRemainingTurn();
        if (newWeather != currWeather || ((*description)->fields.turn == 0xFF && remainingTurns != 0xFF)) {
            pResult->fields.result = ChangeWeatherResult::OK;
            return;
        }

        pResult->fields.result = ChangeWeatherResult::FAIL;
    }
};


HOOK_DEFINE_REPLACE(handler_Sunaokosi) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
        system_load_typeinfo(0x8a5b);
        Common::getClass()->initIfNeeded();
        Handler::Tokusei::getClass()->initIfNeeded();

        if (Common::GetEventVar(args, EventVar::Label::POKEID) == pokeID)
            Handler::Tokusei::common_weather_change(args, pokeID, array_index(BATTLE_WEATHERS, "Snow"), 0x11B, false);
    }
};


void InstallHooks_FieldEffect_Weather() {
    calc$$GetWeatherDmgRatio::InstallAtOffset(0x01f75830);
    Handler_Field$$ADD_Fld_Weather::InstallAtOffset(0x019022c0);
    calc$$RecvWeatherDamage::InstallAtOffset(0x01f75770);
    Section_TurnCheck_Weather$$weatherDamage_Message::InstallAtOffset(0x02173840);
    BTL_CLIENT$$getWeatherStartMessage::InstallAtOffset(0x01f618f0);
    BTL_CLIENT$$scProc_ACT_WeatherEnd::InstallAtOffset(0x01f61a20);

    calc$$IsShineWeather::InstallAtOffset(0x01f758a0);
    calc$$IsRainWeather::InstallAtOffset(0x01f758c0);
    Section_ChangeWeather_Check$$Execute::InstallAtOffset(0x021ae6d0);

    handler_Sunaokosi::InstallAtOffset(0x01947000);

    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Harsh Sunlight"));
    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Rain"));
    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Hail"));
    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Sandstorm"));
    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Heavy Rain"));
    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"));
    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Strong Winds"));
    SetActivatedFieldWeatherHandlers(array_index(BATTLE_WEATHERS, "Snow"));

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01d15588, Movz(W0, 0) }, // EventLauncher$$Event_getDefenderGuard, skip Sandstorm defensive effect (use DEFENDER_GUARD handler instead)
        { 0x020dc0f0, Movz(W0, 0) }, // Section_AddSickCheckFail$$checkFail_byBasicRules, skip Harsh Sunlight check for freezing (use ADDSICK_CHECKFAIL handler instead)
    };
    p.WriteInst(inst);
}