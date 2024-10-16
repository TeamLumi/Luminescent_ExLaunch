#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/Pml/Item/ItemData.h"
#include "externals/Pml/PmlUse.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/WazaNo.h"
#include "externals/XLSXContent/ItemTable.h"

#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(CollectRemindableWaza) {
    static System::Collections::Generic::HashSet$$WazaNo::Object * Callback(Pml::PokePara::CoreParam::Object *__this) {
        Logger::log("CollectRemindableWaza\n");
        system_load_typeinfo(0x3199);
        
        auto hashSet = Orig(__this);

        Pml::PmlUse::Object * pmlUse = Pml::PmlUse::get_Instance();
        XLSXContent::ItemTable::Object * itemTable = pmlUse->fields.itemPrmTotal;
        XLSXContent::ItemTable::SheetWazaMachine::Array * wazaMachine = itemTable->fields.WazaMachine;

        XLSXContent::ItemTable::SheetWazaMachine::Object * iWazaMachine;
        for (size_t i = 0; i < wazaMachine->max_length; i++)
        {
            iWazaMachine = wazaMachine->m_Items[i];
            if (PlayerWork::GetItem(iWazaMachine->fields.itemNo).fields.Count == 0)
            {
                continue;
            }

            uint32_t machineNo = Pml::Item::ItemData::GetWazaMachineNo(iWazaMachine->fields.itemNo);
            bool canLearnTm = __this->CheckWazaMachine(machineNo);
            if (!canLearnTm)
            {
                continue;
            }
            if (__this->GetWazaNo(0) == iWazaMachine->fields.wazaNo)
            {
                continue;
            }

            if (__this->GetWazaNo(1) == iWazaMachine->fields.wazaNo)
            {
                continue;
            }

            if (__this->GetWazaNo(2) == iWazaMachine->fields.wazaNo)
            {
                continue;
            }

            if (__this->GetWazaNo(3) == iWazaMachine->fields.wazaNo)
            {
                continue;
            }

            hashSet->AddPrimitive(iWazaMachine->fields.wazaNo);
        }

        Logger::log("Added TM's move to list\n");
        return hashSet;
    }
};

void exl_relearn_tms_main() {
    CollectRemindableWaza::InstallAtOffset(0x02047aa0);
}
