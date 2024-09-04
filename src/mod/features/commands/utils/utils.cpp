#include "externals/il2cpp-api.h"

#include "externals/EvData.h"
#include "externals/FieldPoketch.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "externals/ZukanWork.h"
#include "externals/Dpr/EvScript/EvDataManager.h"

#include "logger/logger.h"

float ConvertToFloat(int32_t value)
{
    float fresult;
    std::memcpy(&fresult, &value, sizeof(value));
    return fresult;
}

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

System::String::Object* GetStringText(Dpr::EvScript::EvDataManager::Object* manager, EvData::Aregment::Object arg) {
    EvData::Object* evData = manager->fields._evData->fields._EvData;
    auto argType = static_cast<EvData::ArgType>(arg.fields.argType);

    return (argType == EvData::ArgType::String) ? evData->GetString(arg.fields.data) : System::String::Create("");
}

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

bool IsNullOrEgg(Pml::PokePara::PokemonParam::Object * param)
{
    Pml::PokePara::CoreParam::Object * coreParam = (Pml::PokePara::CoreParam::Object *)param;
    return coreParam == nullptr || coreParam->IsNull() || coreParam->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG);
}

bool AddPokemonToParty(int32_t monsno, int32_t formno, uint32_t level, uint8_t maxedIVs, uint16_t itemno)
{
    auto spec = Pml::PokePara::InitialSpec::newInstance();
    spec->fields.monsno = monsno;
    spec->fields.formno = formno;
    spec->fields.level = level;
    spec->fields.talentVNum = maxedIVs;

    auto pokeParam = Pml::PokePara::PokemonParam::newInstance(spec);
    auto coreParam = (Pml::PokePara::CoreParam::Object*)pokeParam;

    coreParam->SetItem(itemno);

    DPData::MYSTATUS::Object* status = PlayerWork::get_playerStatus();
    int32_t zone = PlayerWork::get_zoneID();

    poketool::poke_memo::poketool_poke_memo::SetFromCapture(coreParam, status, zone);

    Pml::PokeParty::Object* party = PlayerWork::get_playerParty();
    bool added = party->AddMember(pokeParam);

    if (added)
    {
        ZukanWork::SetPoke(pokeParam, 3);
        FieldPoketch::AddPokemonHistory(pokeParam);
    }

    return added;
}