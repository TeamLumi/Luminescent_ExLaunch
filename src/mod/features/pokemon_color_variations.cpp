#include "exlaunch.hpp"

#include "externals/BattlePokemonEntity.h"
#include "externals/FieldPokemonEntity.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "externals/UnityEngine/Renderer.h"

#include "logger/logger.h"

void SetPokeColorVariation(Pml::PokePara::CoreParam::Object* coreParam, RendererInfo::Array* renderers)
{
    system_load_typeinfo(0x1e84);
    system_load_typeinfo(0x1e8d);

    for (uint64_t i=0; i<renderers->max_length; i++)
    {
        auto materials = renderers->m_Items[i].fields._Renderer_k__BackingField->get_materials();
        for (uint64_t j=0; j<materials->max_length; j++)
        {
            auto material = materials->m_Items[j];

            if (material->HasProperty(System::String::Create("_ConstantColor0")))
            {
                UnityEngine::Color::Object constantColor0 = material->GetColor(System::String::Create("_ConstantColor0"));
                Logger::log("BOPokemon: Constant0 is %f,%f,%f,%f\n", constantColor0.fields.r, constantColor0.fields.g, constantColor0.fields.b, constantColor0.fields.a);
                auto shiftedConstantColor0 = constantColor0.HueShift(20.0);
                Logger::log("BOPokemon: After shift, Constant0 is %f,%f,%f,%f\n", shiftedConstantColor0.fields.r, shiftedConstantColor0.fields.g, shiftedConstantColor0.fields.b, shiftedConstantColor0.fields.a);
                shiftedConstantColor0 = constantColor0;
                shiftedConstantColor0.fields.r = 1.0;
                shiftedConstantColor0.fields.g = 0.9f;
                shiftedConstantColor0.fields.b = 0.9f;
                shiftedConstantColor0.fields.a = 1.0f;
                material->SetColor(System::String::Create("_ConstantColor0"), shiftedConstantColor0);
            }

            if (material->HasProperty(System::String::Create("_ConstantColorSd0")))
            {
                UnityEngine::Color::Object constantColorSd0 = material->GetColor(System::String::Create("_ConstantColorSd0"));
                Logger::log("BOPokemon: ConstantSd0 is %f,%f,%f,%f\n", constantColorSd0.fields.r, constantColorSd0.fields.g, constantColorSd0.fields.b, constantColorSd0.fields.a);
                auto shiftedConstantColorSd0 = constantColorSd0.HueShift(20.0);
                shiftedConstantColorSd0 = constantColorSd0;
                shiftedConstantColorSd0.fields.r = 1.0f;
                shiftedConstantColorSd0.fields.g = 0.9f;
                shiftedConstantColorSd0.fields.b = 0.9f;
                shiftedConstantColorSd0.fields.a = 1.0f;
                Logger::log("BOPokemon: After shift, ConstantSd0 is %f,%f,%f,%f\n", shiftedConstantColorSd0.fields.r, shiftedConstantColorSd0.fields.g, shiftedConstantColorSd0.fields.b, shiftedConstantColorSd0.fields.a);
                material->SetColor(System::String::Create("_ConstantColorSd0"), shiftedConstantColorSd0);
            }
        }
    }
}

HOOK_DEFINE_INLINE(BOPokemon_Initialize_SetSpindaPattern) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto bpe = (BattlePokemonEntity::Object*)ctx->X[0];
        uint32_t rand = ctx->W[1];
        auto coreParam = (Pml::PokePara::CoreParam::Object*)ctx->X[20];

        auto renderers = bpe->fields._rendererInfos;

        Logger::log("BOPokemon: Setting the Spinda Pattern\n");
        bpe->SetPatcheelPattern(rand);

        SetPokeColorVariation(coreParam, renderers);
    }
};

HOOK_DEFINE_INLINE(UI_SetupPokemonModel) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto fpe = (FieldPokemonEntity::Object*)ctx->X[0];
        auto coreParam = (Pml::PokePara::CoreParam::Object*)ctx->X[19];

        bool fpeNotNull = UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object*)fpe, nullptr);

        if (fpeNotNull)
        {

        }

        ctx->W[0] = (uint32_t)fpeNotNull;
    }
};

void exl_pokemon_color_variations_main() {
    BOPokemon_Initialize_SetSpindaPattern::InstallAtOffset(0x01e520f4);
    //UI_SetupPokemonModel::InstallAtOffset(0x01a10440);
}
