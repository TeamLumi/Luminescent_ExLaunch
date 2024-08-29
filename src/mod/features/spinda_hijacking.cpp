#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "externals/BattlePokemonEntity.h"
#include "externals/Dpr/PatcheelPattern.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/PokemonCustomNodeAnim.h"
#include "externals/UnityEngine/GameObject.h"
#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(PatcheelPattern$$SetPattern) {
    // Third argument is normally a MethodInfo, we are sneaking in the PokemonParam in there :)
    static void Callback(Dpr::PatcheelPattern::Object* __this, uint32_t personalRand, Pml::PokePara::PokemonParam::Object* param) {
        if (param == nullptr)
        {
            Logger::log("[PatcheelPattern$$SetPattern] Did not receive a PokemonParam...\n");
            Orig(__this, personalRand, nullptr);
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
                    Logger::log("[PatcheelPattern$$SetPattern] Magikarp\n");
                    system_load_typeinfo(0x7deb);
                    system_load_typeinfo(0x1e84);
                    system_load_typeinfo(0x1e8d);
                    system_load_typeinfo(0x221b);
                    system_load_typeinfo(0x6d4d);

                    auto go = ((UnityEngine::Component::Object *) __this)->get_gameObject()->instance();
                    auto renderers = go->GetComponentsInChildren(true, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

                    UnityEngine::Renderer::Object* renderer = nullptr;
                    if (renderers != nullptr)
                    {
                        for (uint64_t i=0; i<renderers->max_length; i++)
                        {
                            renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[i];
                            if (((UnityEngine::_Object::Object*)renderer)->GetName()->asCString().find("Patcheel") != -1)
                                break;
                        }
                    }

                    if (renderer != nullptr)
                    {
                        auto patcheelMats = renderer->get_sharedMaterials();
                        uint32_t id = ((Pml::PokePara::CoreParam::Object*)param)->GetMultiPurposeWork() % patcheelMats->max_length;
                        Logger::log("[PatcheelPattern$$SetPattern] Using id %d\n", id);

                        for (uint64_t i=0; i<__this->fields.UVDatas->max_length; i++)
                            ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[i]->fields.renderer)->set_sharedMaterial(patcheelMats->m_Items[id]);
                    }
                }
                break;

                case array_index(SPECIES, "Alcremie"):
                {
                    Logger::log("[PatcheelPattern$$SetPattern] Alcremie\n");
                    system_load_typeinfo(0x7deb);
                    system_load_typeinfo(0x1e84);
                    system_load_typeinfo(0x1e8d);
                    system_load_typeinfo(0x221b);
                    system_load_typeinfo(0x6d4d);

                    uint32_t id = ((Pml::PokePara::CoreParam::Object*)param)->GetMultiPurposeWork();
                    Logger::log("[PatcheelPattern$$SetPattern] Using id %d\n", id);

                    for (int i=0; i<__this->fields.UVDatas->max_length; i++)
                        ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[i]->fields.renderer)->set_enabled(i == id);
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
            __this->fields._patcheelPattern_k__BackingField->SetPatcheelPattern(rand, param);
    }
};

HOOK_DEFINE_REPLACE(PatcheelPattern$$Awake) {
    static void Callback(Dpr::PatcheelPattern::Object* __this) {
        // Do nothing for now
    }
};

void exl_spinda_hijacking_main() {
    PatcheelPattern$$SetPattern::InstallAtOffset(0x01bcb200);
    BattlePokemonEntity$$SetPatcheelPattern::InstallAtOffset(0x01d77990);
    PatcheelPattern$$Awake::InstallAtOffset(0x01bcb1f0);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01e520f0, MovRegister(X2, X20) }, // BOPokemon$$Initialize
        { 0x01add5ac, LdrRegisterImmediate(X2, X20, 0x19 /*X20 + 0xC8*/) }, // Demo_PoffinEat.<Enter>d__16$$MoveNext
        { 0x01cd94f0, MovRegister(X2, X26) }, // FieldWalkingManager.<CreatePartner>d__43$$MoveNext
        { 0x01a7fc10, LdrRegisterImmediate(X2, X19, 0x6 /*X19 + 0x30*/) }, // FureaiHiroba_PokeFactory.<AddPoke>d__10$$MoveNext
        { 0x01a104b8, MovRegister(X2, X19) }, // UIModelViewController$$_SetupPokemonModel
        { 0x018d6068, LdrRegisterImmediate(X2, X8, 0x2 /*X8 + 0x10*/) }, // UgMainProc.<>c__DisplayClass9_0$$<CreatePoke>b__0
    };
    p.WriteInst(inst);
}
