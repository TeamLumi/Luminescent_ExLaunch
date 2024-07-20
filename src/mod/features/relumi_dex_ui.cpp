#include "exlaunch.hpp"

#include "externals/Dpr/Message/MessageHelper.h"
#include "externals/Dpr/Message/MessageManager.h"
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

    SetEggGroupIcons(eggGroup1, eggGroup2, eggID1, eggID2, zukanInfo->fields._GetStatus_k__BackingField, 11);

    UnityEngine::UI::ListPool::Release<System::Collections::Generic::List$$Component>(list, UnityEngine::UI::ListPool::Method$$Component$$Release);
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

        auto ability1Text = statusTransform->Find(System::String::Create("Ability1"))->Find(System::String::Create("ValueText"))
                ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        if (ability1Text != nullptr)
        {
            if (__this->fields._GetStatus_k__BackingField == 3)
                ability1Text->SetupMessage(System::String::Create("ss_tokusei"), data->fields.tokusei1);
            else
                ability1Text->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
        }

        auto ability2Text = statusTransform->Find(System::String::Create("Ability2"))->Find(System::String::Create("ValueText"))
                ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        if (ability2Text != nullptr)
        {
            if (__this->fields._GetStatus_k__BackingField == 3)
                ability2Text->SetupMessage(System::String::Create("ss_tokusei"), data->fields.tokusei2);
            else
                ability2Text->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
        }

        auto ability3Text = statusTransform->Find(System::String::Create("Ability3"))->Find(System::String::Create("ValueText"))
                ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        if (ability3Text != nullptr)
        {
            if (__this->fields._GetStatus_k__BackingField == 3)
                ability3Text->SetupMessage(System::String::Create("ss_tokusei"), data->fields.tokusei3);
            else
                ability3Text->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
        }

        auto item1Text = statusTransform->Find(System::String::Create("Item1"))->Find(System::String::Create("ValueText"))
                ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        if (item1Text != nullptr)
        {
            if (__this->fields._GetStatus_k__BackingField == 3)
                item1Text->SetupMessage(System::String::Create("ss_itemname"), data->fields.item1);
            else
                item1Text->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
        }

        auto item2Text = statusTransform->Find(System::String::Create("Item2"))->Find(System::String::Create("ValueText"))
                ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        if (item2Text != nullptr)
        {
            if (__this->fields._GetStatus_k__BackingField == 3)
                item2Text->SetupMessage(System::String::Create("ss_itemname"), data->fields.item2);
            else
                item2Text->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
        }

        auto item3Text = statusTransform->Find(System::String::Create("Item3"))->Find(System::String::Create("ValueText"))
                ->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        if (item3Text != nullptr)
        {
            if (__this->fields._GetStatus_k__BackingField == 3)
                item3Text->SetupMessage(System::String::Create("ss_itemname"), data->fields.item3);
            else
                item3Text->SetupMessage(System::String::Create("ss_pokedex"), System::String::Create("SS_pokedex_029"));
        }

        SetEggGroupIconsFromZukanInfo(__this, data, statusTransform, 11);
    }
};

HOOK_DEFINE_TRAMPOLINE(ZukanDescriptionPanel$$OnSelectLanguageButton) {
    // Second parameter is Dpr::UI::IUIButton, but that's an interface. It gets casted to this type anyways.
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
