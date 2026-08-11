#include "exlaunch.hpp"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/Message/MessageMsgFile.h"
#include "externals/Dpr/UI/ShopItemItem.h"
#include "externals/FieldManager.h"
#include "externals/GameData/DataManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/SmartPoint/AssetAssistant/AssetManager.h"
#include "externals/System/Action.h"
#include "externals/System/Primitives.h"
#include "externals/System/Text/StringBuilder.h"
#include "externals/UgMainProc.h"
#include "externals/UgResManager.h"

#include "logger/logger.h"

const int32_t AUDIOEVENTID_SHINY = 753281501;
const int32_t AUDIOEVENTID_TEST = 3075682360;

// Shiny-only periodic glint on the wandering UG Pokemon themselves — the same
// hidden-item-style "kirakira" the overworld uses, so a shiny is spottable at a
// distance (on top of the shiny model + the scene-init jingle above).
// EF_F_GRASS_SPARKLE=14, played via FieldManager::CallEffect parented to the mon
// (the effect prefab is audio-less; FieldManager stays alive underground).
static constexpr float UG_SHINY_SPARKLE_INTERVAL   = 2.5f;
static constexpr int32_t UG_SHINY_SPARKLE_EFFECT_ID = 14;
static constexpr int32_t UG_SPARKLE_MAX_MONS        = 64;
static float s_ugSparkleTimers[UG_SPARKLE_MAX_MONS] = {};

HOOK_DEFINE_INLINE(UgMainProc_DisplayClass9_0$$CreatePoke_b__0_ShinyFix) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto disp = (UgMainProc::DisplayClass9_0::Object*)ctx->X[19];
        auto monsno = (int32_t)ctx->W[0];
        auto formno = (int32_t)ctx->W[1];
        auto sex = (Pml::Sex)ctx->W[2];
        auto shiny = disp->fields.param->cast<Pml::PokePara::CoreParam>()->IsRare();

        ctx->X[0] = (uint64_t)GameData::DataManager::GetPokemonCatalog(monsno, formno, sex, shiny, false);
    }
};

HOOK_DEFINE_REPLACE(UgResManager$$AppendAsset_ShinyFix) {
    static void Callback(Pml::PokePara::PokemonParam::Object* param, System::Action::Object* callback) {
        system_load_typeinfo(0xa0b6);

        UgResManager::getClass()->initIfNeeded();
        GameData::DataManager::getClass()->initIfNeeded();
        SmartPoint::AssetAssistant::AssetManager::getClass()->initIfNeeded();

        if (UgResManager::getClass()->static_fields->_assetOpe == nullptr)
            UgResManager::getClass()->static_fields->_assetOpe = System::Collections::Generic::List$$UgResManager_AppendData::newInstance();

        auto appendData = UgResManager::AppendData::newInstance();

        auto monsno = param->cast<Pml::PokePara::CoreParam>()->GetMonsNo();
        auto formno = param->cast<Pml::PokePara::CoreParam>()->GetFormNo();
        auto sex = param->cast<Pml::PokePara::CoreParam>()->GetSex();
        auto shiny = param->cast<Pml::PokePara::CoreParam>()->IsRare();
        auto catalog = GameData::DataManager::GetPokemonCatalog(monsno, formno, sex, shiny, false);

        auto assetBundleName = System::String::Concat(System::String::Create("pokemons/field/"), catalog->fields.AssetBundleName);
        auto req = SmartPoint::AssetAssistant::AssetManager::AppendAssetBundleRequest(assetBundleName, true, nullptr, nullptr);

        appendData->fields.Operation = req;
        appendData->fields.callback = callback;

        UgResManager::getClass()->static_fields->_assetOpe->Add(appendData);
    }
};

HOOK_DEFINE_TRAMPOLINE(UgMainProc$$OnSceneInit_ShinySound) {
    static void Callback(UgMainProc::Object* __this) {
        Orig(__this);

        // Check if there's at least one shiny generated
        auto arr = __this->fields._ugMons->instance()->fields;
        for (int32_t i=0; i<arr._size; i++) {
            if (arr._items->m_Items[i]->fields.pokeParam->cast<Pml::PokePara::CoreParam>()->IsRare()) {
                Logger::log("[UgMainProc$$OnSceneInit] Found shiny! In slot %d\n", i);
                Audio::AudioManager::getClass()->initIfNeeded();
                Audio::AudioManager::get_Instance()->PlaySe(AUDIOEVENTID_SHINY, nullptr);
                break;
            }
        }
    }
};


// UgMainProc::update(float time) — the underground's per-frame tick for the wild
// symbol mons. After the vanilla update, run the shiny glint pass over _ugMons.
HOOK_DEFINE_TRAMPOLINE(UgMainProc$$Update_ShinySparkle) {
    static bool Callback(UgMainProc::Object* __this, float time) {
        bool result = Orig(__this, time);

        if (__this->fields._ugMons != nullptr) {
            auto arr = __this->fields._ugMons->instance()->fields;
            FieldManager::getClass()->initIfNeeded();
            auto* fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;

            for (int32_t i = 0; i < arr._size && i < UG_SPARKLE_MAX_MONS; i++) {
                auto* sm = (arr._items != nullptr) ? arr._items->m_Items[i] : nullptr;
                if (sm == nullptr) { s_ugSparkleTimers[i] = 0.0f; continue; }
                auto& f = sm->fields;
                if (!f.Active || !f.isLoaded || f.transform == nullptr ||
                    f.pokeParam == nullptr ||
                    !f.pokeParam->cast<Pml::PokePara::CoreParam>()->IsRare()) {
                    s_ugSparkleTimers[i] = 0.0f;
                    continue;
                }
                s_ugSparkleTimers[i] += time;
                if (s_ugSparkleTimers[i] >= UG_SHINY_SPARKLE_INTERVAL) {
                    s_ugSparkleTimers[i] = 0.0f;
                    if (fm != nullptr) {
                        fm->CallEffect(UG_SHINY_SPARKLE_EFFECT_ID, f.transform, nullptr, nullptr);
                    }
                }
            }
        }
        return result;
    }
};

void exl_ug_shinies_main() {
    UgMainProc_DisplayClass9_0$$CreatePoke_b__0_ShinyFix::InstallAtOffset(0x018d5ea0);
    UgResManager$$AppendAsset_ShinyFix::InstallAtOffset(0x01b1b110);
    UgMainProc$$OnSceneInit_ShinySound::InstallAtOffset(0x018d2af0);
    UgMainProc$$Update_ShinySparkle::InstallAtOffset(0x018d4270);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;

    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x02057cec, Nop() }, // Do not add category 1 (spheres) items to UG sellers
        { 0x02057e40, Nop() }, // Do not add category 1 (spheres) items to UG sellers
    };
    p.WriteInst(inst);

}
