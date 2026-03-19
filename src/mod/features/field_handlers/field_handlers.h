#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_CLIENT.h"
#include "externals/Dpr/Battle/Logic/ChangeWeatherCause.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Section_FieldEffect_End.h"
#include "externals/Dpr/Battle/Logic/Section_TurnCheck_Weather.h"
#include "features/field_handlers/data/extra_field_handlers.h"

#include "logger/logger.h"

ExtraFieldEffectWeatherHandlers* getExtraFieldEffectWeatherHandlers();
ExtraFieldEffectHandlers* getExtraFieldEffectHandlers();

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateFieldEffectEventHandlerTable(long size);
EventFactor::EventHandlerTable::Object* CreateFieldEffectEventHandler(EventID eventID, Il2CppMethodPointer methodPointer);
EventFactor::EventHandlerTable::Object* CreateFieldEffectEventHandler(EventID eventID, MethodInfo* method);
void SetFieldEffectFunctionTable(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t fieldEffect, Il2CppMethodPointer methodPointer);
void SetFieldEffectFunctionTable(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t fieldEffect, MethodInfo* method);


/// WEATHER FIELD HANDLERS

// Installs hooks for Weather.
void InstallHooks_FieldEffect_Weather();

// Generates the field effect handler table for the Harsh Sunlight weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_HarshSunlight();

// Returns the end-of-turn weather damage dealt by Harsh Sunlight.
uint16_t FieldEffect_Weather_HarshSunlight_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Harsh Sunlight.
void FieldEffect_Weather_HarshSunlight_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Harsh Sunlight weather starting.
int32_t FieldEffect_Weather_HarshSunlight_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Harsh Sunlight weather ending.
int32_t FieldEffect_Weather_HarshSunlight_EndStrID(BTL_CLIENT::Object* client);


// Generates the field effect handler table for the Rain weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Rain();

// Returns the end-of-turn weather damage dealt by Rain.
uint16_t FieldEffect_Weather_Rain_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Rain.
void FieldEffect_Weather_Rain_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Rain weather starting.
int32_t FieldEffect_Weather_Rain_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Rain weather ending.
int32_t FieldEffect_Weather_Rain_EndStrID(BTL_CLIENT::Object* client);


// Generates the field effect handler table for the Hail weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Hail();

// Returns the end-of-turn weather damage dealt by Hail.
uint16_t FieldEffect_Weather_Hail_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Hail.
void FieldEffect_Weather_Hail_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Hail weather starting.
int32_t FieldEffect_Weather_Hail_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Hail weather ending.
int32_t FieldEffect_Weather_Hail_EndStrID(BTL_CLIENT::Object* client);


// Generates the field effect handler table for the Sandstorm weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Sandstorm();

// Returns the end-of-turn weather damage dealt by Sandstorm.
uint16_t FieldEffect_Weather_Sandstorm_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Sandstorm.
void FieldEffect_Weather_Sandstorm_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Sandstorm weather starting.
int32_t FieldEffect_Weather_Sandstorm_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Sandstorm weather ending.
int32_t FieldEffect_Weather_Sandstorm_EndStrID(BTL_CLIENT::Object* client);


// Generates the field effect handler table for the Heavy Rain weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_HeavyRain();

// Returns the end-of-turn weather damage dealt by Heavy Rain.
uint16_t FieldEffect_Weather_HeavyRain_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Heavy Rain.
void FieldEffect_Weather_HeavyRain_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Heavy Rain weather starting.
int32_t FieldEffect_Weather_HeavyRain_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Heavy Rain weather ending.
int32_t FieldEffect_Weather_HeavyRain_EndStrID(BTL_CLIENT::Object* client);


// Generates the field effect handler table for the Extremely Harsh Sunlight weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_ExtremelyHarshSunlight();

// Returns the end-of-turn weather damage dealt by Extremely Harsh Sunlight.
uint16_t FieldEffect_Weather_ExtremelyHarshSunlight_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Extremely Harsh Sunlight.
void FieldEffect_Weather_ExtremelyHarshSunlight_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Extremely Harsh Sunlight weather starting.
int32_t FieldEffect_Weather_ExtremelyHarshSunlight_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Extremely Harsh Sunlight weather ending.
int32_t FieldEffect_Weather_ExtremelyHarshSunlight_EndStrID(BTL_CLIENT::Object* client);


// Generates the field effect handler table for the Strong Winds weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_StrongWinds();

// Returns the end-of-turn weather damage dealt by Strong Winds.
uint16_t FieldEffect_Weather_StrongWinds_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Strong Winds.
void FieldEffect_Weather_StrongWinds_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Strong Winds weather starting.
int32_t FieldEffect_Weather_StrongWinds_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Strong Winds weather ending.
int32_t FieldEffect_Weather_StrongWinds_EndStrID(BTL_CLIENT::Object* client);


// Generates the field effect handler table for the Snow weather.
EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Snow();

// Returns the end-of-turn weather damage dealt by Snow.
uint16_t FieldEffect_Weather_Snow_Damage(BTL_POKEPARAM::Object* bpp);

// Sets up the end-of-turn weather damage message for Snow.
void FieldEffect_Weather_Snow_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID);

// Returns the strID for the Snow weather starting.
int32_t FieldEffect_Weather_Snow_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause);

// Returns the strID for the Snow weather ending.
int32_t FieldEffect_Weather_Snow_EndStrID(BTL_CLIENT::Object* client);


/// OTHER FIELD HANDLERS

// Adds field effect handlers for Mud Sport.
void Handlers_FieldEffect_MudSport(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds field effect removal code for Mud Sport.
void FieldEffect_End_MudSport(Section_FieldEffect_End::Object* __this, Section_FieldEffect_End::Result::Object* pResult, Section_FieldEffect_End::Description::Object** description);

// Adds field effect handlers for Water Sport.
void Handlers_FieldEffect_WaterSport(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds field effect removal code for Water Sport.
void FieldEffect_End_WaterSport(Section_FieldEffect_End::Object* __this, Section_FieldEffect_End::Result::Object* pResult, Section_FieldEffect_End::Description::Object** description);
