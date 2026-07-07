#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "externals/BattlePokemonEntity.h"
#include "externals/Dpr/Battle/View/Objects/BOPokemon.h"
#include "externals/Dpr/Demo/Demo_Hatch.h"
#include "externals/Dpr/PatcheelPattern.h"
#include "externals/Dpr/SubContents/TimeLineMotion.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/PokemonCustomNodeAnim.h"
#include "externals/UnityEngine/GameObject.h"

#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(PatcheelPattern$$SetPattern) {
    // Third argument is normally a MethodInfo, we are sneaking in the PokemonParam in there :)
    static void Callback(Dpr::PatcheelPattern::Object* __this, uint32_t personalRand, Pml::PokePara::PokemonParam::Object* param) {
        system_load_typeinfo(0x7deb);
        system_load_typeinfo(0x6d4d);

        if (param == nullptr)
        {
            Logger::log("[PatcheelPattern$$SetPattern] Did not receive a PokemonParam...\n");
            Orig(__this, personalRand, nullptr);
        }
        else if (param->cast<Pml::PokePara::CoreParam>()->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG))
        {
            Logger::log("[PatcheelPattern$$SetPattern] Egg\n");

            auto go = ((UnityEngine::Component::Object *) __this)->get_gameObject()->instance();
            auto renderers = go->GetComponentsInChildren(true, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

            UnityEngine::Renderer::Object* renderer = nullptr;
            if (renderers != nullptr)
            {
                for (uint64_t i=0; i<renderers->max_length; i++)
                {
                    renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[i];
                    if (((UnityEngine::_Object::Object*)renderer)->GetName()->asCString().find("Patcheel") != nn::string::npos)
                        break;
                }
            }

            if (renderer != nullptr)
            {
                auto patcheelMats = renderer->get_materials();
                uint32_t id = ((Pml::PokePara::CoreParam::Object*)param)->GetType1() % (patcheelMats->max_length - 1);
                Logger::log("[PatcheelPattern$$SetPattern] Using id %d\n", id);

                for (uint64_t i=0; i<__this->fields.UVDatas->max_length; i++)
                    ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[i]->fields.renderer)->set_material(patcheelMats->m_Items[id+1]);
            }
        }
        else
        {
            switch(((Pml::PokePara::CoreParam::Object*)param)->GetMonsNo())
            {
                case array_index(SPECIES, "Spinda"):
                {
                    Logger::log("[PatcheelPattern$$SetPattern] Spinda\n");
                    Orig(__this, personalRand, nullptr);
                }
                break;

                case array_index(SPECIES, "Arbok"):
                case array_index(SPECIES, "Magikarp"):
                {
                    Logger::log("[PatcheelPattern$$SetPattern] Arbok and Magikarp\n");

                    auto go = ((UnityEngine::Component::Object *) __this)->get_gameObject()->instance();
                    auto renderers = go->GetComponentsInChildren(true, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

                    UnityEngine::Renderer::Object* renderer = nullptr;
                    if (renderers != nullptr)
                    {
                        for (uint64_t i=0; i<renderers->max_length; i++)
                        {
                            renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[i];
                            if (((UnityEngine::_Object::Object*)renderer)->GetName()->asCString().find("Patcheel") != nn::string::npos)
                                break;
                        }
                    }

                    if (renderer != nullptr)
                    {
                        auto patcheelMats = renderer->get_materials();
                        uint32_t id = ((Pml::PokePara::CoreParam::Object*)param)->GetMultiPurposeWork() % (patcheelMats->max_length - 1);
                        Logger::log("[PatcheelPattern$$SetPattern] Using id %d\n", id);

                        for (uint64_t i=0; i<__this->fields.UVDatas->max_length; i++)
                            ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[i]->fields.renderer)->set_material(patcheelMats->m_Items[id+1]);
                    }
                }
                break;

                case array_index(SPECIES, "Pikachu"):
                {
                    if (param->cast<Pml::PokePara::CoreParam>()->GetFormNo() == 7)
                    {
                        Logger::log("[PatcheelPattern$$SetPattern] Cap Pikachu\n");

                        uint32_t id = ((Pml::PokePara::CoreParam::Object*)param)->GetMultiPurposeWork();
                        Logger::log("[PatcheelPattern$$SetPattern] Using id %d\n", id);

                        for (uint64_t i=0; i<__this->fields.UVDatas->max_length; i++)
                            ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[i]->fields.renderer)->set_enabled(i == (uint64_t)id);
                    }
                }
                break;

                case array_index(SPECIES, "Smeargle"):
                {
                    Logger::log("[PatcheelPattern$$SetPattern] Smeargle\n");

                    uint32_t id = ((Pml::PokePara::CoreParam::Object*)param)->GetMultiPurposeWork();
                    Logger::log("[PatcheelPattern$$SetPattern] Using variant %06X\n", id);

                    if (__this->fields.UVDatas->max_length > 0)
                    {
                        auto materials = ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[0]->fields.renderer)->get_materials();
                        auto color = UnityEngine::Color::CreateFromRGB((int32_t)id);

                        materials->m_Items[0]->SetColor(System::String::Create("_ConstantColor0"), color);
                        materials->m_Items[0]->SetColor(System::String::Create("_ConstantColorSd0"), color);
                        materials->m_Items[0]->SetColor(System::String::Create("_ConstantColor"), color);
                        materials->m_Items[0]->SetColor(System::String::Create("_RimColor"), color);
                        materials->m_Items[0]->SetColor(System::String::Create("_RimColorShadow"), color);
                    }
                }
                break;

                case array_index(SPECIES, "Alcremie"):
                {
                    Logger::log("[PatcheelPattern$$SetPattern] Alcremie\n");

                    uint32_t id = ((Pml::PokePara::CoreParam::Object*)param)->GetMultiPurposeWork();
                    Logger::log("[PatcheelPattern$$SetPattern] Using id %d\n", id);

                    for (uint64_t i=0; i<__this->fields.UVDatas->max_length; i++)
                        ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[i]->fields.renderer)->set_enabled(i == (uint64_t)id);
                }
                break;
            }
        }
    }
};

HOOK_DEFINE_REPLACE(BattlePokemonEntity$$SetPatcheelPattern) {
    // Third argument is normally a MethodInfo, we are sneaking in the PokemonParam in there :)
    static void Callback(BattlePokemonEntity::Object* __this, uint32_t rand, Pml::PokePara::PokemonParam::Object* param) {
        Logger::log("BattlePokemonEntity$$SetPatcheelPattern\n");
        system_load_typeinfo(0x2226);
        UnityEngine::_Object::getClass()->initIfNeeded();

        if (UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object*)__this->fields._patcheelPattern_k__BackingField, nullptr))
            __this->fields._patcheelPattern_k__BackingField->SetPattern(rand, param);
    }
};

HOOK_DEFINE_REPLACE(PatcheelPattern$$Awake) {
    static void Callback(Dpr::PatcheelPattern::Object* __this) {
        // Do nothing for now
    }
};

HOOK_DEFINE_INLINE(TimeLineBinder$$Bind_b__30_0) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto pokeParam = (Pml::PokePara::PokemonParam::Object*)ctx->X[0];
        auto motion = (Dpr::SubContents::TimeLineMotion::Object*)ctx->X[20];

        // Sneak a PokemonParam in there :)
        motion->fields.boPokemon = (Dpr::Battle::View::Objects::BOPokemon::Object*)pokeParam;

        ctx->W[0] = pokeParam->cast<Pml::PokePara::CoreParam>()->GetPersonalRnd();
    }
};

HOOK_DEFINE_INLINE(TimeLineMotion$$Update) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto patcheel = (Dpr::PatcheelPattern::Object*)ctx->X[0];
        auto __this = (Dpr::SubContents::TimeLineMotion::Object*)ctx->X[19];
        patcheel->SetPattern(__this->fields.Pattern, (Pml::PokePara::PokemonParam::Object*)__this->fields.boPokemon);
    }
};

HOOK_DEFINE_TRAMPOLINE(Demo_Hatch_DisplayClass9_0$$Enter_b__2) {
    // Setting the pattern for the egg
    static void Callback(Dpr::Demo::Demo_Hatch::DisplayClass9_0::Object* __this, UnityEngine::_Object::Object* asset) {
        Orig(__this, asset);

        system_load_typeinfo(0x98bd);

        if (System::String::op_Equality(asset->get_Name(), __this->fields.tamagoCatalog->fields.AssetBundleName)) {
            auto pokeParam = __this->fields.__4__this->fields.param;
            auto go = (UnityEngine::GameObject::Object*)asset;
            auto patcheel = go->GetComponent(UnityEngine::GameObject::Method$$PatcheelPattern$$GetComponent);
            if (UnityEngine::_Object::op_Inequality(patcheel->cast<UnityEngine::_Object>(), nullptr)) {
                auto eggPokeParam = Pml::PokePara::PokemonParam::newInstance(pokeParam);
                eggPokeParam->cast<Pml::PokePara::CoreParam>()->ChangeEgg();
                patcheel->SetPattern(pokeParam->cast<Pml::PokePara::CoreParam>()->GetPersonalRnd(), eggPokeParam);
            }
        }
    }
};


void exl_spinda_hijacking_main() {
    PatcheelPattern$$SetPattern::InstallAtOffset(0x01bcb200);
    BattlePokemonEntity$$SetPatcheelPattern::InstallAtOffset(0x01d77990);
    PatcheelPattern$$Awake::InstallAtOffset(0x01bcb1f0);

    TimeLineBinder$$Bind_b__30_0::InstallAtOffset(0x0186687c);
    TimeLineMotion$$Update::InstallAtOffset(0x01869978);

    // Egg in hatching Demo
    Demo_Hatch_DisplayClass9_0$$Enter_b__2::InstallAtOffset(0x01ad8260);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01e520f0, MovRegister(X2, X20) }, // BOPokemon$$Initialize
        { 0x01add5ac, LdrRegisterImmediate(X2, X20, 0x19 /*X20 + 0xC8*/) }, // Demo_PoffinEat.<Enter>d__16$$MoveNext
        { 0x01cd94f0, MovRegister(X2, X26) }, // FieldWalkingManager.<CreatePartner>d__43$$MoveNext
        { 0x01a7fc10, LdrRegisterImmediate(X2, X19, 0x6 /*X19 + 0x30*/) }, // FureaiHiroba_PokeFactory.<AddPoke>d__10$$MoveNext
        { 0x01a104b8, MovRegister(X2, X19) }, // UIModelViewController$$_SetupPokemonModel
        { 0x018d6068, LdrRegisterImmediate(X2, X19, 0x3 /*X8 + 0x18*/) }, // UgMainProc.<>c__DisplayClass9_0$$<CreatePoke>b__0
    };
    p.WriteInst(inst);
}
