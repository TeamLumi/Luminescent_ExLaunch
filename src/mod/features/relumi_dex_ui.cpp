#include "exlaunch.hpp"

#include "externals/Dpr/Message/MessageHelper.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/TypePanel.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/ZukanDescriptionPanel.h"
#include "externals/Dpr/UI/ZukanInfo.h"
#include "externals/Dpr/UI/ZukanLanguageButton.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/UnityEngine/UI/ListPool.h"

#include "logger/logger.h"

UnityEngine::Sprite::Object* GetEggGroupSprite(int32_t eggGroup, int32_t langId)
{
    Dpr::UI::UIManager::getClass()->initIfNeeded();

    auto initialStr = eggGroup < 10 ? "cmn_ico_egg_0" : "cmn_ico_egg_";
    auto prefixStr = System::String::Create(initialStr + nn::to_string(eggGroup) + "_");
    auto langStr = Dpr::Message::MessageHelper::GetLanguageVariant(langId);
    auto fullStr = System::String::Concat(prefixStr, langStr);

    return Dpr::UI::UIManager::instance()->GetAtlasSprite(6, fullStr);
}

void SetEggGroupIcons(Dpr::UI::TypePanel::Object* panel1, Dpr::UI::TypePanel::Object* panel2, int32_t eggGroup1, int32_t eggGroup2, int32_t getStatus, int32_t langId)
{
    Logger::log("SetEggGroupIcons\n");

    Dpr::Message::MessageManager::getClass()->initIfNeeded();

    if (langId == 11)
        langId = Dpr::Message::MessageManager::instance()->get_UserLanguageID();

    if (panel1 != nullptr)
    {
        if (getStatus == 3)
        {
            if (eggGroup1 < 16 && eggGroup1 != -1)
            {
                panel1->fields.iconImage->set_sprite(GetEggGroupSprite(eggGroup1, langId));
                ((UnityEngine::Component::Object*)panel1)->get_gameObject()->SetActive(true);
            }
            else
            {
                panel1->Deactive();
            }
        }
        else
        {
            panel1->Deactive();
        }
    }

    if (panel2 != nullptr)
    {
        if (getStatus == 3)
        {
            if (eggGroup2 < 16 && eggGroup2 != -1)
            {
                panel2->fields.iconImage->set_sprite(GetEggGroupSprite(eggGroup2, langId));
                ((UnityEngine::Component::Object*)panel2)->get_gameObject()->SetActive(true);
            }
            else
            {
                panel2->Deactive();
            }
        }
        else
        {
            panel2->Deactive();
        }
    }
}

void SetEggGroupIconsFromZukanInfo(Dpr::UI::ZukanInfo::Object* zukanInfo, XLSXContent::PersonalTable::SheetPersonal::Object* personalData, UnityEngine::Transform::Object* statusTf, int32_t langId)
{
    Logger::log("SetEggGroupIconsFromZukanInfo\n");

    system_load_typeinfo(0x870a);
    system_load_typeinfo(0x505e);

    auto eggGroup = statusTf->Find(System::String::Create("Egg Group"));
    auto list = UnityEngine::UI::ListPool::Get(UnityEngine::UI::ListPool::Method$$Component$$Get);

    eggGroup->Find(System::String::Create("TypePanel1"))->GetComponents(list, UnityEngine::Component::Method$$Component$$GetComponents);
    auto eggGroup1 = (Dpr::UI::TypePanel::Object*)list->fields._items->m_Items[3];
    eggGroup->Find(System::String::Create("TypePanel2"))->GetComponents(list, UnityEngine::Component::Method$$Component$$GetComponents);
    auto eggGroup2 = (Dpr::UI::TypePanel::Object*)list->fields._items->m_Items[3];

    int32_t eggID1 = personalData->fields.egg_group1;
    int32_t eggID2 = personalData->fields.egg_group2 == eggID1 ? -1 : personalData->fields.egg_group2;

    SetEggGroupIcons(eggGroup1, eggGroup2, eggID1, eggID2, zukanInfo->fields._GetStatus_k__BackingField, langId);

    UnityEngine::UI::ListPool::Release<System::Collections::Generic::List$$Component>(list, UnityEngine::UI::ListPool::Method$$Component$$Release);
}

void SetMessageInUIText(UnityEngine::Transform::Object* root, System::String::Object* uiTextName, System::String::Object* msbtName, int32_t messageIndex, int32_t getStatus)
{
    auto uiText = root->Find(uiTextName)->Find(System::String::Create("ValueText"))
            ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
    if (uiText != nullptr)
    {
        if (getStatus == 3)
            uiText->SetupMessage(msbtName, messageIndex);
        else
            uiText->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
    }
}

void SetNumberInUIText(UnityEngine::Transform::Object* root, System::String::Object* uiTextName, int32_t number, int32_t getStatus)
{
    auto uiText = root->Find(uiTextName)->Find(System::String::Create("ValueText"))
            ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
    if (uiText != nullptr)
    {
        if (getStatus == 3)
        {
            Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
            Dpr::Message::MessageWordSetHelper::SetDigitWord(0, number);
            uiText->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_206"));
        }
        else
        {
            uiText->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
        }
    }
}

void ChangeLanguageNumberInUIText(UnityEngine::Transform::Object* root, System::String::Object* uiTextName, int32_t number, int32_t langId)
{
    auto uiText = root->Find(uiTextName)->Find(System::String::Create("ValueText"))
            ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
    if (uiText != nullptr)
    {
        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, number);
        uiText->ChangeLanguage(langId);
    }
}

HOOK_DEFINE_TRAMPOLINE(ZukanInfo$$SetupUITexts) {
    static void Callback(Dpr::UI::ZukanInfo::Object* __this, Dpr::UI::UIText::Object* nameText, Dpr::UI::UIText::Object* classText, Dpr::UI::UIText::Object* heightText,
                         Dpr::UI::UIText::Object* weightText, Dpr::UI::UIText::Object* descText, Dpr::UI::UIText::Object* formNameText) {
        Orig(__this, nameText, classText, heightText, weightText, descText, formNameText);

        Logger::log("ZukanInfo$$SetupUITexts\n");

        // We need this UIText to go back up the hierarchy
        // This is null in the compare panel, so we just stop here.
        if (classText == nullptr)
            return;

        system_load_typeinfo(0x870a);

        int32_t monsno = __this->fields._MonsNo_k__BackingField;
        uint16_t formno = __this->fields.modelParams->m_Items[__this->fields.modelIndexSelector->fields._CurrentIndex_k__BackingField]->fields.FormNo;

        Pml::Personal::PersonalSystem::getClass()->initIfNeeded();
        auto data = Pml::Personal::PersonalSystem::GetPersonalData(monsno, formno);

        auto statusTransform = ((UnityEngine::Component::Object*)classText)->get_transform()->GetParent();

        SetMessageInUIText(statusTransform, System::String::Create("Ability1"), System::String::Create("ss_tokusei"),
                           data->fields.tokusei1, __this->fields._GetStatus_k__BackingField);
        SetMessageInUIText(statusTransform, System::String::Create("Ability2"), System::String::Create("ss_tokusei"),
                           data->fields.tokusei2, __this->fields._GetStatus_k__BackingField);
        SetMessageInUIText(statusTransform, System::String::Create("Ability3"), System::String::Create("ss_tokusei"),
                           data->fields.tokusei3, __this->fields._GetStatus_k__BackingField);

        SetMessageInUIText(statusTransform, System::String::Create("Item1"), System::String::Create("ss_itemname"),
                           data->fields.item1, __this->fields._GetStatus_k__BackingField);
        SetMessageInUIText(statusTransform, System::String::Create("Item2"), System::String::Create("ss_itemname"),
                           data->fields.item2, __this->fields._GetStatus_k__BackingField);
        SetMessageInUIText(statusTransform, System::String::Create("Item3"), System::String::Create("ss_itemname"),
                           data->fields.item3, __this->fields._GetStatus_k__BackingField);

        SetNumberInUIText(statusTransform, System::String::Create("StatHP"),
                          data->fields.basic_hp, __this->fields._GetStatus_k__BackingField);
        SetNumberInUIText(statusTransform, System::String::Create("StatATK"),
                          data->fields.basic_atk, __this->fields._GetStatus_k__BackingField);
        SetNumberInUIText(statusTransform, System::String::Create("StatDEF"),
                          data->fields.basic_def, __this->fields._GetStatus_k__BackingField);
        SetNumberInUIText(statusTransform, System::String::Create("StatSPATK"),
                          data->fields.basic_spatk, __this->fields._GetStatus_k__BackingField);
        SetNumberInUIText(statusTransform, System::String::Create("StatSPDEF"),
                          data->fields.basic_spdef, __this->fields._GetStatus_k__BackingField);
        SetNumberInUIText(statusTransform, System::String::Create("StatSPE"),
                          data->fields.basic_agi, __this->fields._GetStatus_k__BackingField);

        SetEggGroupIconsFromZukanInfo(__this, data, statusTransform, 11);
    }
};

HOOK_DEFINE_TRAMPOLINE(ZukanDescriptionPanel$$OnSelectLanguageButton) {
    // Second parameter is Dpr::UI::IUIButton, but that's an interface. It gets cast to this type anyways.
    static void Callback(Dpr::UI::ZukanDescriptionPanel::Object* __this, Dpr::UI::ZukanLanguageButton::Object* button) {
        Orig(__this, button);

        Logger::log("ZukanDescriptionPanel$$OnSelectLanguageButton\n");

        system_load_typeinfo(0x505e);

        if (UnityEngine::_Object::op_Equality((UnityEngine::_Object::Object*)button, nullptr))
            return;

        int32_t monsno = __this->fields._CurrentZukanInfo_k__BackingField->fields._MonsNo_k__BackingField;
        uint16_t formno = __this->fields._CurrentZukanInfo_k__BackingField->fields.modelParams->m_Items[
                __this->fields._CurrentZukanInfo_k__BackingField->fields.modelIndexSelector->fields._CurrentIndex_k__BackingField]->fields.FormNo;

        Pml::Personal::PersonalSystem::getClass()->initIfNeeded();
        auto data = Pml::Personal::PersonalSystem::GetPersonalData(monsno, formno);
        auto statusTransform = ((UnityEngine::Component::Object*)__this->fields.classText)->get_transform()->GetParent();

        // Update the language for Base Stat UITexts
        ChangeLanguageNumberInUIText(statusTransform, System::String::Create("StatHP"),
                                     data->fields.basic_hp, button->fields._LangId_k__BackingField);
        ChangeLanguageNumberInUIText(statusTransform, System::String::Create("StatATK"),
                                     data->fields.basic_atk, button->fields._LangId_k__BackingField);
        ChangeLanguageNumberInUIText(statusTransform, System::String::Create("StatDEF"),
                                     data->fields.basic_def, button->fields._LangId_k__BackingField);
        ChangeLanguageNumberInUIText(statusTransform, System::String::Create("StatSPATK"),
                                     data->fields.basic_spatk, button->fields._LangId_k__BackingField);
        ChangeLanguageNumberInUIText(statusTransform, System::String::Create("StatSPDEF"),
                                     data->fields.basic_spdef, button->fields._LangId_k__BackingField);
        ChangeLanguageNumberInUIText(statusTransform, System::String::Create("StatSPE"),
                                     data->fields.basic_agi, button->fields._LangId_k__BackingField);

        SetEggGroupIconsFromZukanInfo(__this->fields._CurrentZukanInfo_k__BackingField, data, statusTransform, button->fields._LangId_k__BackingField);
    }
};

HOOK_DEFINE_TRAMPOLINE(ZukanDescriptionPanel$$MoveCatalogSelect) {
    static bool Callback(Dpr::UI::ZukanDescriptionPanel::Object* __this, int32_t addValue) {
        if (!Orig(__this, addValue))
            return false;

        Logger::log("ZukanDescriptionPanel$$MoveCatalogSelect\n");

        system_load_typeinfo(0x505e);

        int32_t monsno = __this->fields._CurrentZukanInfo_k__BackingField->fields._MonsNo_k__BackingField;
        uint16_t formno = __this->fields._CurrentZukanInfo_k__BackingField->fields.modelParams->m_Items[
                __this->fields._CurrentZukanInfo_k__BackingField->fields.modelIndexSelector->fields._CurrentIndex_k__BackingField]->fields.FormNo;

        Pml::Personal::PersonalSystem::getClass()->initIfNeeded();
        auto data = Pml::Personal::PersonalSystem::GetPersonalData(monsno, formno);
        auto statusTransform = ((UnityEngine::Component::Object*)__this->fields.classText)->get_transform()->GetParent();

        SetEggGroupIconsFromZukanInfo(__this->fields._CurrentZukanInfo_k__BackingField, data, statusTransform, 11);

        return true;
    }
};

HOOK_DEFINE_TRAMPOLINE(UIManager$$GetAtlasSprite) {
    static bool Callback(Dpr::UI::UIManager::Object* __this, int32_t spriteAtlasId, System::String::Object* name) {
        Logger::log("UIManager$$GetAtlasSprite -> %s\n", name->asCString().c_str());
        return Orig(__this, spriteAtlasId, name);
    }
};

void exl_relumi_dex_ui() {
    ZukanInfo$$SetupUITexts::InstallAtOffset(0x01bb1120);
    ZukanDescriptionPanel$$OnSelectLanguageButton::InstallAtOffset(0x01bb5b70);
    ZukanDescriptionPanel$$MoveCatalogSelect::InstallAtOffset(0x01bb57d0);

    //UIManager$$GetAtlasSprite::InstallAtOffset(0x017a9080);
}
