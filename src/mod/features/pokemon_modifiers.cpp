#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"

#include "externals/BattlePokemonEntity.h"

#include "logger/logger.h"
#include "utils/utils.h"

HOOK_DEFINE_INLINE(PokemonModifiersTest) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto bpe = (BattlePokemonEntity::Object*)ctx->X[0];
        auto rand = (uint32_t)ctx->W[1];
        auto param = (Pml::PokePara::PokemonParam::Object*)ctx->X[2];

        bpe->SetPatcheelPattern(rand, param);

        // Apply modifiers here
        if (IsTotem(param->cast<Pml::PokePara::CoreParam>()))
        {
            Logger::log("[PokemonModifiersTest] Totem! Adjusting scale of %s by 2x\n", bpe->fields._enityName->asCString().c_str());
            bpe->fields._adjustScale = UnityEngine::Vector3::op_Multiply(bpe->fields._adjustScale, 2.0f);

            Logger::log("[PokemonModifiersTest] Totem! Adding orange rim color for %s\n", bpe->fields._enityName->asCString().c_str());
            auto renderers = bpe->fields._rendererInfos;
            for (uint64_t i=0; i<renderers->max_length; i++)
            {
                auto mats = renderers->m_Items[i].fields._Renderer_k__BackingField->get_materials();
                for (uint64_t j=0; j<mats->max_length; j++)
                {
                    auto mat = mats->m_Items[j];

                    if (mat->HasProperty(System::String::Create("_RimColor")) &&
                        mat->HasProperty(System::String::Create("_RimColorShadow")) &&
                        mat->HasProperty(System::String::Create("_RimColorValue")) &&
                        mat->HasProperty(System::String::Create("_RimPower")) &&
                        mat->HasProperty(System::String::Create("_RimStrength")))
                    {
                        UnityEngine::Color::Object newRimColor = { .fields = { .r = 1.0f, .g = 0.5f, .b = 0.0f, .a = 1.0f } };

                        Logger::log("[PokemonModifiersTest] Setting: _RimColor %f/%f/%f/%f (%06X), _RimColorValue 0.7, _RimPower 3.0, _RimStrength 10.0\n", newRimColor.fields.r, newRimColor.fields.g, newRimColor.fields.b, newRimColor.fields.a, newRimColor.ToHexCode());

                        mat->SetColor(System::String::Create("_RimColor"), newRimColor);
                        mat->SetColor(System::String::Create("_RimColorShadow"), newRimColor);
                        mat->SetFloat(System::String::Create("_RimColorValue"), 0.7f);
                        mat->SetFloat(System::String::Create("_RimPower"), 3.0f);
                        mat->SetFloat(System::String::Create("_RimStrength"), 10.0f);
                    }
                }
            }

            Logger::log("[PokemonModifiersTest] Totem! Adding effects for %s\n", bpe->fields._enityName->asCString().c_str());
            bpe->fields._locators
        }
        else
        {
            Logger::log("[PokemonModifiersTest] %s is not a Totem, doing nothing\n", bpe->fields._enityName->asCString().c_str());
        }
    }
};

void exl_pokemon_modifiers_main() {
    PokemonModifiersTest::InstallAtOffset(0x01e520f4);
}