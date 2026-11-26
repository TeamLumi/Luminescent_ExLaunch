#include "exlaunch.hpp"

#include "externals/Pml/Item/ItemManager.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/XLSXContent/ItemTable.h"
#include "externals/XLSXContent/PersonalTable.h"
#include "romdata/romdata.h"

#include "logger/logger.h"

uint32_t GetWazaMachineNo(uint16_t itemno) {
    system_load_typeinfo(game::TypeInfo::Pml_Item_ItemData_GetWazaMashineMax);
    Pml::Item::ItemManager::getClass()->initIfNeeded();
    auto tms = Pml::Item::ItemManager::getClass()->static_fields->s_Instance->fields.m_alldata->fields.WazaMachine;

    for (uint64_t i=0; i<tms->max_length; i++)
    {
        if (tms->m_Items[i]->fields.itemNo == (int32_t)itemno)
            return tms->m_Items[i]->fields.machineNo;
    }

    return 0xffffffff;
}

HOOK_DEFINE_REPLACE(ItemTableExtensions_GetWazaMachineNo) {
    static uint32_t Callback(XLSXContent::ItemTable::SheetItem::Object* item) {
        return GetWazaMachineNo(item->fields.no);
    }
};

HOOK_DEFINE_REPLACE(ItemData_GetWazaMachineNo) {
    static uint32_t Callback(uint16_t item) {
        return GetWazaMachineNo(item);
    }
};

HOOK_DEFINE_REPLACE(PersonalSystem_CheckPersonalWazaMachine) {
    static bool Callback(uint16_t machineNo) {
        Logger::log("CheckPersonalWazaMachine\n");
        uint16_t personalID = Pml::Personal::PersonalSystem::getClass()->static_fields->s_PersonalData->fields.id;
        uint16_t monsno = Pml::Personal::PersonalSystem::getClass()->static_fields->s_PersonalData->fields.monsno;
        uint16_t formIndex = Pml::Personal::PersonalSystem::getClass()->static_fields->s_PersonalData->fields.form_index;

        uint16_t formno;
        if (personalID == monsno) formno = 0;
        else formno = personalID - formIndex + 1;

        return CanLearnTM(monsno, formno, machineNo);
    }
};

HOOK_DEFINE_REPLACE(PersonalTableExtensions_CheckWazaMachine) {
    static bool Callback(XLSXContent::PersonalTable::SheetPersonal::Object* data, int32_t _machineNo) {
        Logger::log("CheckWazaMachine\n");
        uint16_t personalID = data->fields.id;
        uint16_t monsno = data->fields.monsno;
        uint16_t formIndex = data->fields.form_index;

        uint16_t formno;
        if (personalID == monsno) formno = 0;
        else formno = personalID - formIndex + 1;

        return CanLearnTM(monsno, formno, _machineNo);
    }
};

void exl_extended_tm_learnsets_main() {
    ItemTableExtensions_GetWazaMachineNo::InstallAtOffset(game::Address::Pml_Item_ItemTableExtensions_GetWazaMashineNo);
    ItemData_GetWazaMachineNo::InstallAtOffset(game::Address::Pml_Item_ItemData_GetWazaMashineNo);
    PersonalSystem_CheckPersonalWazaMachine::InstallAtOffset(game::Address::Pml_Personal_PersonalSystem_CheckPersonalWazaMachine);
    PersonalTableExtensions_CheckWazaMachine::InstallAtOffset(game::Address::Pml_Personal_PersonalTableExtensions_CheckWazaMachine);
}
