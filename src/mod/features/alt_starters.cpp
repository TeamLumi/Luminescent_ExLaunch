#include "exlaunch.hpp"

#include "data/clip_names.h"
#include "data/utils.h"
#include "externals/BaseEntity.h"
#include "externals/DG/Tweening/DOVirtual.h"
#include "externals/DG/Tweening/TweenCallback.h"
#include "externals/Dpr/Demo/Demo_Gosanke.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/SubContents/Utils.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "externals/UnityEngine/Transform.h"
#include "romdata/romdata.h"
#include "utils/utils.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(PlayerWork_get_defaultPokeNo) {
    static int32_t Callback() {
        int32_t starter = PlayerWork::get_playerStatus()->fields.defaultPokeType;
        return GetStarter(starter).monsNo;
    }
};

HOOK_DEFINE_REPLACE(PlayerWork_get_defaultPokeType) {
    static int32_t Callback() {
        return PlayerWork::get_playerStatus()->fields.defaultPokeType;
    }
};

HOOK_DEFINE_REPLACE(PlayerWork_get_rivalPokeNo) {
    static int32_t Callback() {
        int32_t rivalStarter = PlayerWork::get_rivalPokeType();
        return GetStarter(rivalStarter).monsNo;
    }
};

HOOK_DEFINE_REPLACE(PlayerWork_get_rivalPokeType) {
    static int32_t Callback() {
        int32_t starter = PlayerWork::get_defaultPokeType();

        switch (starter)
        {
            case 0:
                return 1;
            case 1:
                return 2;
            case 2:
                return 0;

            default:
                return 1;
        }
    }
};

HOOK_DEFINE_REPLACE(PlayerWork_get_supportPokeNo) {
    static int32_t Callback() {
        int32_t supportStarter = PlayerWork::get_supportPokeType();
        return GetStarter(supportStarter).monsNo;
    }
};

HOOK_DEFINE_REPLACE(PlayerWork_get_supportPokeType) {
    static int32_t Callback() {
        int32_t starter = PlayerWork::get_defaultPokeType();

        switch (starter)
        {
            case 0:
                return 2;
            case 1:
                return 0;
            case 2:
                return 1;

            default:
                return 2;
        }
    }
};

HOOK_DEFINE_REPLACE(Demo_Starters_SelectPoke) {
    static void Callback(Dpr::Demo::Demo_Gosanke::Object* __this) {
        system_load_typeinfo(game::TypeInfo::Dpr_Demo_Demo_Gosanke_SelectPoke_b_22_0);

        int32_t cursor = __this->fields.nowCursolPosIndex;
        RomData::Starter starter = GetStarter(cursor);

        Dpr::SubContents::Utils::getClass()->initIfNeeded();
        Dpr::SubContents::Utils::PlayVoice(starter.monsNo, starter.formNo, 0, nullptr);
    }
};

HOOK_DEFINE_INLINE(Demo_Starters_SelectPoke_1) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[19] = ctx->X[0];
        ctx->X[21] = ctx->X[0];
    }
};

HOOK_DEFINE_INLINE(Demo_Starters_SelectPoke_2) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (Dpr::Demo::Demo_Gosanke::Object*)ctx->X[21];
        ctx->W[0] = GetStarter(__this->fields.nowCursolPosIndex).monsNo;
    }
};

HOOK_DEFINE_INLINE(Demo_Starters_Enter_Starter_0) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto spec = (Pml::PokePara::InitialSpec::Object*)ctx->X[23];

        RomData::Starter starter = GetStarter(0);
        ctx->W[8] = (uint32_t)starter.monsNo; // MonsNo is set on the next instruction
        spec->fields.formno = starter.formNo;
    }
};

HOOK_DEFINE_INLINE(Demo_Starters_Enter_Starter_1) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto spec = (Pml::PokePara::InitialSpec::Object*)ctx->X[23];

        RomData::Starter starter = GetStarter(1);
        ctx->W[8] = (uint32_t)starter.monsNo; // MonsNo is set on the next instruction
        spec->fields.formno = starter.formNo;
    }
};

HOOK_DEFINE_INLINE(Demo_Starters_Enter_Starter_2) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto spec = (Pml::PokePara::InitialSpec::Object*)ctx->X[23];

        RomData::Starter starter = GetStarter(2);
        ctx->W[8] = (uint32_t)starter.monsNo; // MonsNo is set on the next instruction
        spec->fields.formno = starter.formNo;
    }
};

HOOK_DEFINE_TRAMPOLINE(Demo_Starters_ShowPokeView) {
    static void* Callback(Dpr::Demo::Demo_Gosanke::Object* __this, int32_t cursolIndex) {
        auto renderCamera = __this->fields.RenderCamera;

        RomData::Starter starter = GetStarter(cursolIndex);
        auto catalog = GetDefaultCatalog(starter.monsNo, starter.formNo);
        UnityEngine::Vector3::Object pos = catalog->fields.DistributionModelOffset;
        UnityEngine::Vector3::Object rot = catalog->fields.DistributionModelRotationAngle;
        float scale = catalog->fields.DistributionScale;

        pos.fields.y -= 0.35f;
        rot.fields.y += 180.0f;

        auto cameraTransform = ((UnityEngine::Component::Object*)renderCamera)->get_transform();
        auto cubeTransform = cameraTransform->Find(System::String::Create("Cube"));

        cubeTransform->set_localPosition(pos);
        cubeTransform->set_localEulerAngles(rot);
        cubeTransform->set_localScale({ .fields = { .x = scale, .y = scale, .z = scale, }});

        return Orig(__this, cursolIndex); // Returns DG_Tweening_Tweener_o*
    }
};

HOOK_DEFINE_INLINE(PokeRenderViewer_ShowPokeView) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto transform = *((UnityEngine::Transform::Object**)ctx->X[21]);
        auto baseEntity = *((BaseEntity::Object**)ctx->X[23]);
        UnityEngine::Transform::Object* entityTransform = baseEntity->get_transform();

        UnityEngine::Vector3::Object pos = transform->get_localPosition();
        entityTransform->set_localPosition(pos);

        UnityEngine::Vector3::Object rot = transform->get_localEulerAngles();
        entityTransform->set_localEulerAngles(rot);

        UnityEngine::Vector3::Object scale = transform->get_localScale();
        entityTransform->set_localScale(scale);
    }
};

HOOK_DEFINE_INLINE(Demo_Gosanke_PokeConfirm_Cry) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto callback = (DG::Tweening::TweenCallback::Object*)ctx->X[0];
        bool ignoreTimeScale = (bool)ctx->W[1];

        int32_t starterId = PlayerWork::get_defaultPokeType();
        RomData::Starter starter = GetStarter(starterId);
        Dpr::SubContents::Utils::getClass()->initIfNeeded();
        Dpr::SubContents::Utils::PlayVoice(starter.monsNo, starter.formNo, 0, nullptr);

        DG::Tweening::DOVirtual::DelayedCall(1.6f, callback, ignoreTimeScale);
    }
};

HOOK_DEFINE_INLINE(DemoBase_DrawMessage_SetMonTags) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (Dpr::Demo::DemoBase::Object*)ctx->X[0];
        auto NextMessageNo = (int32_t)ctx->W[1];

        int32_t starterId = NextMessageNo - 2;
        RomData::Starter starter = GetStarter(starterId);

        Dpr::Message::MessageWordSetHelper::SetGlossaryWord(0, System::String::Create("ss_zkn_type"), starter.monsNo);
        Dpr::Message::MessageWordSetHelper::SetMonsNameWord(1, starter.monsNo);

        __this->DrawMessage(2); // Always do the generalized Turtwig message
    }
};

void exl_alt_starters_main() {
    PlayerWork_get_defaultPokeNo::InstallAtOffset(game::Address::PlayerWork_get_defaultPokeNo);
    PlayerWork_get_defaultPokeType::InstallAtOffset(game::Address::PlayerWork_get_defaultPokeType);
    PlayerWork_get_rivalPokeNo::InstallAtOffset(game::Address::PlayerWork_get_rivalPokeNo);
    PlayerWork_get_rivalPokeType::InstallAtOffset(game::Address::PlayerWork_get_rivalPokeType);
    PlayerWork_get_supportPokeNo::InstallAtOffset(game::Address::PlayerWork_get_supportPokeNo);
    PlayerWork_get_supportPokeType::InstallAtOffset(game::Address::PlayerWork_get_supportPokeType);

    Demo_Starters_SelectPoke::InstallAtOffset(game::Address::Dpr_Demo_Demo_Gosanke_SelectPoke_b_22_0);

    Demo_Starters_Enter_Starter_0::InstallAtOffset(game::Address::Dpr_Demo_Demo_Gosanke_Enter_d_13_MoveNext + 0x1A8);
    Demo_Starters_Enter_Starter_1::InstallAtOffset(game::Address::Dpr_Demo_Demo_Gosanke_Enter_d_13_MoveNext + 0x1D8);
    Demo_Starters_Enter_Starter_2::InstallAtOffset(game::Address::Dpr_Demo_Demo_Gosanke_Enter_d_13_MoveNext + 0x20C);

    Demo_Starters_ShowPokeView::InstallAtOffset(game::Address::Dpr_Demo_Demo_Gosanke_ShowPokeView);
    PokeRenderViewer_ShowPokeView::InstallAtOffset(game::Address::PokeRenderViewer_ShowPokeView + 0x2BC);

    Demo_Gosanke_PokeConfirm_Cry::InstallAtOffset(game::Address::Dpr_Demo_Demo_Gosanke_PokeConfirm_d_23_MoveNext + 0x370);

    DemoBase_DrawMessage_SetMonTags::InstallAtOffset(game::Address::Dpr_Demo_Demo_Gosanke_PokeConfirm_d_23_MoveNext + 0xB8);

    int32_t idleAnim1 = array_index(FIELD_MON_CLIPS, "Idle");         // Starter idle anim, Priority 1
    int32_t idleAnim2 = array_index(FIELD_MON_CLIPS, "Kw_Wait");      // Starter idle anim, Priority 2
    int32_t idleAnim3 = array_index(FIELD_MON_CLIPS, "FieldWait1");   // Starter idle anim, Priority 3
    int32_t confirmAnim = array_index(FIELD_MON_CLIPS, "Roar01");     // Starter selection anim

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::Dpr_Demo_Demo_Gosanke_PokeConfirm_d_23_MoveNext + 0x2F0, Movz(W0, 0) }, // Starter 0 value to save
        { game::Address::Dpr_Demo_Demo_Gosanke_PokeConfirm_d_23_MoveNext + 0x1C0, Movz(W0, 1) }, // Starter 1 value to save
        { game::Address::Dpr_Demo_Demo_Gosanke_PokeConfirm_d_23_MoveNext + 0x308, Movz(W0, 2) }, // Starter 2 value to save

        { game::Address::PokeRenderViewer_ShowPokeView + 0x2C0, Branch(0x4c) }, // PokeRenderViewer_ShowPokeView skip some re-written code

        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0xCC, CmpImmediate(W9, idleAnim1) },                         // Change animation on starter idle (Priority 1)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0x11C, Movz(W1, idleAnim1) },                                 // Change animation on starter idle (Priority 1)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0xD8, LdrRegisterImmediate(X20, X8, idleAnim1 + 0x4) },  // Change animation on starter idle (Priority 1)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0x12C, CmpImmediate(W9, idleAnim2) },                         // Change animation on starter idle (Priority 2)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0x17C, Movz(W1, idleAnim2) },                                 // Change animation on starter idle (Priority 2)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0x138, LdrRegisterImmediate(X20, X8, idleAnim2 + 0x4) },  // Change animation on starter idle (Priority 2)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0x1BC, CmpImmediate(W9, idleAnim3) },                         // Change animation on starter idle (Priority 3)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0x204, Movz(W1, idleAnim3) },                                 // Change animation on starter idle (Priority 3)
        { game::Address::PokeRenderViewer_PokeAnimationUpdate_d_18_MoveNext + 0x1C8, LdrRegisterImmediate(X20, X8, idleAnim3 + 0x4) },  // Change animation on starter idle (Priority 3)

        { game::Address::Dpr_Demo_Demo_Gosanke_PokeConfirm_d_23_MoveNext + 0x318, Movz(W9, confirmAnim) }, // Change animation on starter selection
    };
    p.WriteInst(inst);
}
