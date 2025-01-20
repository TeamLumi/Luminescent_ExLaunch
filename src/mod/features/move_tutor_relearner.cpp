#include "exlaunch.hpp"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/UIWazaManage.h"

#include "logger/logger.h"
#include "romdata/romdata.h"

HOOK_DEFINE_INLINE(UIWazaManage$$SetupPokemonInfo$$BootType_Setups) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("UIWazaManage$$SetupPokemonInfo$$BootType_Setups\n");
        auto window = (Dpr::UI::UIWazaManage::Object*)ctx->X[0];

        bool validType = window->fields.param.fields.BootType == 0 || window->fields.param.fields.BootType == 3;
        ctx->W[22] = (uint32_t)validType;
        ctx->W[21] = (uint32_t)!validType;

        // Set up the first one
        ctx->W[2] = ctx->W[22];
        ctx->X[3] = (uint64_t)nullptr;
        Logger::log("UIWazaManage$$SetupPokemonInfo$$BootType_Setups END\n");
    }
};

HOOK_DEFINE_INLINE(UIWazaManage$$SetupPokemonInfo$$BootType_CollectMoves) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("UIWazaManage$$SetupPokemonInfo$$BootType_CollectMoves\n");
        system_load_typeinfo(0x4ced);
        auto window = (Dpr::UI::UIWazaManage::Object*)ctx->X[19];

        int32_t type = (int32_t)ctx->W[8];
        auto list = (System::Collections::Generic::List$$int32_t::Object*)ctx->X[21];
        if (type == 3)
        {
            Logger::log("UIWazaManage$$SetupPokemonInfo$$BootType_CollectMoves TYPE 3\n");
            list->Clear();

            int32_t table = window->fields.param.fields.LearnWazaNo;
            auto moves = GetMoveTutorTable(table);

            for (int32_t i=0; i<moves.size(); i++)
                list->Add(moves[i]);
        }

        ctx->W[22] = 0xffffffff;
        Logger::log("UIWazaManage$$SetupPokemonInfo$$BootType_CollectMoves END\n");
    }
};

HOOK_DEFINE_INLINE(UIWazaManage$$OnUpdate$$BootType_Learn) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("UIWazaManage$$OnUpdate$$BootType_Learn\n");
        auto window = (Dpr::UI::UIWazaManage::Object*)ctx->X[19];
        auto disp = (Dpr::UI::UIWazaManage::__c__DisplayClass39_0::Object*)ctx->X[20];
        auto selectedMove = (int32_t)ctx->W[0];

        disp->fields.selectWazaNo = selectedMove;

        auto type = (int32_t)window->fields.param.fields.BootType;
        ctx->W[8] = type;

        if (type == 3)
        {
            Logger::log("UIWazaManage$$OnUpdate$$BootType_Learn TYPE 3\n");
            disp->fields.learnWazaNo = selectedMove;

            Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
            Dpr::Message::MessageWordSetHelper::SetWazaNameWord(0, disp->fields.learnWazaNo);

            auto labelName = System::String::Create("SS_waza_remember_023");
            ctx->X[8] = (uint64_t)&labelName;
        }

        ctx->W[21] = ctx->W[0];
        Logger::log("UIWazaManage$$OnUpdate$$BootType_Learn END\n");
    }
};

HOOK_DEFINE_INLINE(UIWazaManage$$OnUpdate$$BootType_Closing) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("UIWazaManage$$OnUpdate$$BootType_Closing\n");
        auto window = (Dpr::UI::UIWazaManage::Object*)ctx->X[19];

        auto type = (int32_t)window->fields.param.fields.BootType;
        ctx->W[8] = type;

        if (type == 3)
        {
            Logger::log("UIWazaManage$$OnUpdate$$BootType_Closing TYPE 3\n");
            Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
            Dpr::Message::MessageWordSetHelper::SetPokemonNickNameWord(0, window->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>(), true);
            auto onFinished = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(window, *Dpr::UI::UIWazaManage::Method$$OnUpdate$$b__39_0);
            window->fields.msgWindowController->OpenMsgWindow(3, System::String::Create("SS_waza_remember_026"), true, false, onFinished, nullptr);

            Audio::AudioManager::getClass()->initIfNeeded();
            Audio::AudioManager::instance()->PlaySe(0xa4eb827e, nullptr);
        }
        Logger::log("UIWazaManage$$OnUpdate$$BootType_Closing END\n");
    }
};

void exl_move_tutor_relearner_main() {
    UIWazaManage$$SetupPokemonInfo$$BootType_Setups::InstallAtOffset(0x01dd4410);
    UIWazaManage$$SetupPokemonInfo$$BootType_CollectMoves::InstallAtOffset(0x01dd452c);
    UIWazaManage$$OnUpdate$$BootType_Learn::InstallAtOffset(0x01dd4cb0);
    UIWazaManage$$OnUpdate$$BootType_Closing::InstallAtOffset(0x01dd4d48);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x01dd4414);
    p.WriteInst(Nop()); // Nop out the values of w22 and w21
    p.WriteInst(Nop());
    p.WriteInst(Nop());

    // Jump to after the switch if boot type is 3
    p.Seek(0x01dd4cb4);
    p.WriteInst(0x71000d1f); // cmp w8, #3
    p.WriteInst(0x54001a20); // b.eq #0x344
}
