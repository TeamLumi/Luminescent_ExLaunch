#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

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

                case array_index(SPECIES, "Arbok"):
                {
                    Logger::log("[PatcheelPattern$$SetPattern] Arbok\n");
                    // TODO
                }

                case array_index(SPECIES, "Magikarp"):
                {
                    Logger::log("[PatcheelPattern$$SetPattern] Magikarp\n");

                    auto go = ((UnityEngine::Component::Object *) __this)->get_gameObject()->instance();
                    auto nodes = go->GetComponent(UnityEngine::Component::Method$$PokemonCustomNodeAnim$$GetComponent)->fields.mCustomNodeMaterials;

                    nn::vector<UnityEngine::Material::Object*> patcheelMats;
                    for (uint64_t i=0; i<nodes->max_length; i++)
                    {
                        if (nodes->m_Items[i]->fields.shaderName->asCString() == "Patcheel")
                            patcheelMats.push_back(nodes->m_Items[i]->fields.mp->fields.mat);
                    }

                    if (patcheelMats.size() > 0)
                    {
                        uint32_t id = personalRand % patcheelMats.size();
                        for (uint64_t i=0; i<__this->fields.UVDatas->max_length; i++)
                            ((UnityEngine::Renderer::Object*)__this->fields.UVDatas->m_Items[i]->renderer)->set_material(patcheelMats[id]);
                    }
                }
            }
        }
    }
};

HOOK_DEFINE_REPLACE(PatcheelPattern$$Awake) {
    static void Callback(Dpr::PatcheelPattern::Object* __this) {
        // Do nothing for now
    }
};

void exl_spinda_hijacking_main() {
    PatcheelPattern$$SetPattern::InstallAtOffset(0x01bcb200);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01e520f0, MovRegister(X2, X20) }, // BOPokemon$$Initialize
        { 0x01d779f8, MovRegister(X2, X2) }, // BattlePokemonEntity$$SetPatcheelPattern
        { 0x01d77a0c, MovRegister(X2, X2) }, // BattlePokemonEntity$$SetPatcheelPattern
        { 0x01add5ac, LdrRegisterImmediate(X2, X20, 0x19 /*X20 + 0xC8*/) }, // Demo_PoffinEat.<Enter>d__16$$MoveNext
        { 0x01cd94f0, MovRegister(X2, X26) }, // FieldWalkingManager.<CreatePartner>d__43$$MoveNext
        { 0x01a7fc10, LdrRegisterImmediate(X2, X19, 0x6 /*X19 + 0x30*/) }, // FureaiHiroba_PokeFactory.<AddPoke>d__10$$MoveNext
        { 0x01a104b8, MovRegister(X2, X19) }, // UIModelViewController$$_SetupPokemonModel
        { 0x018d6068, LdrRegisterImmediate(X2, X8, 0x2 /*X8 + 0x10*/) }, // UgMainProc.<>c__DisplayClass9_0$$<CreatePoke>b__0
    };
    p.WriteInst(inst);
}
