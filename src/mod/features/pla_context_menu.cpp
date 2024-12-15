#include "exlaunch.hpp"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/Dpr/UI/PokemonWindow.h"
#include "externals/Dpr/UI/SoftwareKeyboard.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIWazaManage.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/WazaNo.h"
#include "externals/System/Func.h"
#include "externals/UIWindowID.h"

#include "logger/logger.h"

static Pml::PokePara::PokemonParam::Object* sPokemonParam = nullptr;
static bool sCloseWindow = false;

void patchContextMenu(System::Collections::Generic::List$$ContextMenuItem_Param::Object * list, Dpr::UI::ContextMenuItem::Param::Object * argParam, Dpr::UI::PokemonWindow::DisplayClass25_0::Object * dispClass)
{
    Logger::log("patchContextMenu\n");
    list->Add(argParam);

    Pml::PokePara::CoreParam::Object * pokemonParam;
    pokemonParam = (Pml::PokePara::CoreParam::Object *) dispClass->fields.pokemonParam;
    System::Collections::Generic::HashSet$$WazaNo::Object * remindableWaza = pokemonParam->CollectRemindableWaza();

    if (remindableWaza->fields._count > 0)
    {
        Logger::log("Adding move relearner\n");
        Dpr::UI::ContextMenuItem::Param::Object * moveRelearnerParam = Dpr::UI::ContextMenuItem::Param::newInstance();
        moveRelearnerParam->fields.menuId = ContextMenuID::PARTY_CHANGE_MOVES;
        list->Add(moveRelearnerParam);
    }

    Logger::log("Adding name rater\n");
    Dpr::UI::ContextMenuItem::Param::Object * nameRaterParam = Dpr::UI::ContextMenuItem::Param::newInstance();
    nameRaterParam->fields.menuId = ContextMenuID::PARTY_CHANGE_NAME;
    list->Add(nameRaterParam);
}

void EvDataManager_EvCmdCallWazaOshieUi_b__0(Dpr::EvScript::EvDataManager::DisplayClass1544_0::Object *__this, int32_t learnWazaNo, int32_t unlearnWazaNo)
{
    // User decided to not learn the move.
    if (learnWazaNo == 0 || unlearnWazaNo == 0)
    {
        return;
    }

    __this->fields.__4__this->LearnWaza(__this->fields.param, learnWazaNo, unlearnWazaNo);
}

void EvDataManager_EvCmdCallWazaOmoidashiUi_b__1539_0(Dpr::EvScript::EvDataManager::Object *__this, int32_t learnWazaNo, int32_t unlearnWazaNo)
{
    Logger::log("EvDataManager_EvCmdCallWazaOmoidashiUi_b__1539_0\n");

    // unlearnWazaNo is always zero, because this is just selecting the move to learn.
    if (learnWazaNo == 0)
    {
        return;
    }

    auto corePokeParam = (Pml::PokePara::CoreParam::Object *) sPokemonParam;
    if (corePokeParam->AddWazaIfEmptyExist(learnWazaNo) == 0)
    {
        // Already learned the move because you had less than 4 moves
        return;
    }

    auto evDataManagerDispClass = Dpr::EvScript::EvDataManager::DisplayClass1544_0::newInstance();

    evDataManagerDispClass->fields.__4__this = __this;
    evDataManagerDispClass->fields.param = sPokemonParam;
    evDataManagerDispClass->fields.tray = 0;
    evDataManagerDispClass->fields.idx = 0;

    MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdCallWazaOshieUiParty((Il2CppMethodPointer)&EvDataManager_EvCmdCallWazaOshieUi_b__0);
    System::Action::Object* resultCallback = System::Action::getClass(System::Action::WazaNo_WazaNo_TypeInfo)->newInstance(evDataManagerDispClass, mi);

    auto param = (Dpr::UI::UIWazaManage::Param::Object) {
        .fields = {
            .BootType = 2,
            .IsOpenFromFieldScript = true,
            .PokemonParam = sPokemonParam,
            .LearnWazaNo = learnWazaNo,
            .ResultCallback = resultCallback,
        }
    };

    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    auto uiManager = Dpr::UI::UIManager::get_Instance();
    auto uiWazaManage = (Dpr::UI::UIWazaManage::Object *) uiManager->CreateUIWindow(UIWindowID::WAZA_MANAGE, Dpr::UI::UIManager::Method$$CreateUIWindow_UIWazaManage_);

    // nullptr on the MethodInfo here doesn't crash somehow
    uiWazaManage->Open(param, nullptr);
}

void createMoveRelearnerWindow(Dpr::UI::PokemonWindow::DisplayClass25_0::Object * dispClass)
{
    Logger::log("createMoveRelearnerWindow\n");
    system_load_typeinfo(0x43f0);
    system_load_typeinfo(0x43ef);
    system_load_typeinfo(0x4378);

    Dpr::EvScript::EvDataManager::Object * evDataManager = Dpr::EvScript::EvDataManager::get_Instanse();

    MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdCallWazaOmoidashiUiParty((Il2CppMethodPointer)&EvDataManager_EvCmdCallWazaOmoidashiUi_b__1539_0);
    System::Action::Object* resultCallback = System::Action::getClass(System::Action::WazaNo_WazaNo_TypeInfo)->newInstance(evDataManager, mi);

    sPokemonParam = dispClass->fields.pokemonParam;

    Dpr::UI::UIWazaManage::Param::Object param = (Dpr::UI::UIWazaManage::Param::Object) {
        .fields = {
            .BootType = 0,
            .IsOpenFromFieldScript = true,
            .PokemonParam = dispClass->fields.pokemonParam,
            .LearnWazaNo = 0,
            .ResultCallback = resultCallback,
        }
    };

    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    auto uiManager = Dpr::UI::UIManager::get_Instance();
    auto uiWazaManage = uiManager->CreateUIWindow<Dpr::UI::UIWazaManage>(UIWindowID::WAZA_MANAGE, Dpr::UI::UIManager::Method$$CreateUIWindow_UIWazaManage_);

    // nullptr on the MethodInfo here doesn't crash somehow
    uiWazaManage->Open(param, nullptr);
}

void renamePokemon(Dpr::UI::PokemonWindow::DisplayClass25_0::Object * dispClass, Dpr::UI::ContextMenuItem::Param::Object * ctxMenuParam)
{
    system_load_typeinfo(0x4469);

    auto evDataManager = Dpr::EvScript::EvDataManager::Object::get_Instanse();

    Pml::PokePara::PokemonParam::Object * pokeParam = dispClass->fields.pokemonParam;
    auto corePokeParam = (Pml::PokePara::CoreParam::Object *) pokeParam;

    auto dispClass773 = Dpr::EvScript::EvDataManager::DisplayClass773_0::newInstance();
    dispClass773->fields.pokemonParam = pokeParam;

    System::String::Object * headerLabel;
    switch (corePokeParam->GetSex())
    {
        case Pml::Sex::MALE:
            headerLabel = System::String::Create("SS_strinput_006");
            break;
        case Pml::Sex::FEMALE:
            headerLabel = System::String::Create("SS_strinput_007");
            break;
        case Pml::Sex::UNKNOWN:
        default:
            headerLabel = System::String::Create("SS_strinput_005");
            break;
    }

    Dpr::Message::MessageWordSetHelper::SetPokemonNickNameWord(0, corePokeParam, true);
    auto swKeyboardParam = Dpr::UI::SoftwareKeyboard::Param::newInstance();

    swKeyboardParam->fields.text = System::String::Create("");
    swKeyboardParam->fields.textMinLength = 1;
    swKeyboardParam->fields.textMaxLength = Dpr::UI::SoftwareKeyboard::LanguageMaxLength(6, -1);
    swKeyboardParam->fields.headerText = Dpr::UI::SoftwareKeyboard::GetMessageText(headerLabel);
    swKeyboardParam->fields.subText = nullptr;
    swKeyboardParam->fields.guideText = nullptr;
    swKeyboardParam->fields.okText = nullptr;
    swKeyboardParam->fields.invalidCharFlag = 4;

    MethodInfo* onInputCheckMI = *Dpr::EvScript::EvDataManager::Method$$EvDataManager_EvCmdNameInPoke_OnInputCheck;
    System::Func::Object * onInputCheck = System::Func::getClass(System::Func::String__SoftwareKeyboard_ErrorState__ValueTuple_bool_String__TypeInfo)->newInstance(evDataManager, onInputCheckMI);

    MethodInfo* onCompleteMI = *Dpr::EvScript::EvDataManager::Method$$EvDataManager_EvCmdNameInPoke_OnComplete;
    UnityEngine::Events::UnityAction::Object * onComplete = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::bool_String_TypeInfo)->newInstance(dispClass773, onCompleteMI);

    Dpr::UI::SoftwareKeyboard::Open(swKeyboardParam, onInputCheck, onComplete);
}

HOOK_DEFINE_INLINE(ContextMenuAction) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto contextMenuId = (int32_t) ctx->W[9];
        auto dispClass = (Dpr::UI::PokemonWindow::DisplayClass25_0::Object *) ctx->X[19];
        auto ctxMenuParam = (Dpr::UI::ContextMenuItem::Param::Object *) ctx->X[8];

        switch(contextMenuId)
        {
            case (int32_t)ContextMenuID::PARTY_CHANGE_MOVES:
                createMoveRelearnerWindow(dispClass);
                break;

            case (int32_t)ContextMenuID::PARTY_CHANGE_NAME:
                renamePokemon(dispClass, ctxMenuParam);
                break;

            default:
                break;
        }

        //Allow Field Waza to perform its original checks
        ctx->W[9] = contextMenuId - 35; // sub w9, w9, #0x23
    }
};

HOOK_DEFINE_REPLACE(DisplayClass773_0_EvCmdNameInPoke_b__1) {
    static void Callback(Dpr::EvScript::EvDataManager::DisplayClass773_0::Object *__this, bool isSuccess, System::String::Object *resultText) {
        if (!isSuccess)
        {
            Logger::log("Game considered bad nickname\n");
            return;
        }

        bool isEmpty = System::String::IsNullOrEmpty(resultText);
        if (isEmpty)
        {
            Logger::log("Nickname is null or empty\n");
            return;
        }

        Pml::PokePara::PokemonParam::Object * pokeParam = __this->fields.pokemonParam;
        auto corePokeParam = (Pml::PokePara::CoreParam *) pokeParam;

        sCloseWindow = true;
        corePokeParam->SetNickName(resultText);
    }
};

bool IsPushButton_CloseWindow(Dpr::UI::UIWindow::Object *__this, int32_t button, bool isForce)
{
    if (__this->IsPushButton(button, isForce))
    {
        return true;
    }

    if (sCloseWindow)
    {
        sCloseWindow = false;
        return true;
    }

    return false;
}

void exl_pla_context_menu_main() {
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;

    // Add to Party Context Menu
    exl::patch::CodePatcher p(0x019fa08c);
    p.WriteInst(MovRegister(X0, X21));
    p.WriteInst(MovRegister(X1, X23));
    p.WriteInst(MovRegister(X2, X19));
    p.BranchLinkInst((void*)&patchContextMenu);

    // Assign code to the Context Menu options
    ContextMenuAction::InstallAtOffset(0x019fbf7c);

    // Queue the window closing after nicknaming
    DisplayClass773_0_EvCmdNameInPoke_b__1::InstallAtOffset(0x019aef00);

    // Close the window
    p.Seek(0x019f8460);
    p.BranchLinkInst((void*)&IsPushButton_CloseWindow);
}
