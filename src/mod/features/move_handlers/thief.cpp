#include "exlaunch.hpp"

#include "externals/BTL_STRID_SET.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/ItemWork.h"

#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

// Changes the handler for Thief to put the item into the player's bag.
HOOK_DEFINE_REPLACE(Handler_Dorobou) {
    static void Callback(EventFactor::EventHandlerArgs::Object **args, uint8_t pokeID)
    {
        system_load_typeinfo(0xa92f);
        system_load_typeinfo(0xa965);
        system_load_typeinfo(0x43b9);
        system_load_typeinfo(0x43ba);
        Common::getClass()->initIfNeeded();

        // Check that we're looking at the attacking pokémon.
        uint32_t attackingPoke = Common::GetEventVar(args, EventVar::Label::POKEID_ATK);
        if (attackingPoke == pokeID)
        {
            // Check that the attacking pokémon is not holding an item.
            bool attackerHasNoItem = Common::Dorobou_CheckEnable(args, pokeID);
            if (attackerHasNoItem)
            {
                // Check the target??
                uint32_t targetPoke = Common::GetEventVar(args, EventVar::Label::POKEID_TARGET1);
                if ((targetPoke & 0xff) != 0x1f)
                {
                    // Check if the target is holding an item.
                    BTL_POKEPARAM::Object* targetPokeParam = Common::GetPokeParam(args, (uint8_t)targetPoke);
                    uint16_t item = targetPokeParam->GetItem();
                    if (item != 0)
                    {
                        // Check if the item can be thieved. (Form change items, wild pokémon using the move, etc.)
                        bool cantSteal = Common::CheckCantStealPoke(args, pokeID, (uint8_t)targetPoke);
                        if (!cantSteal)
                        {
                            if (Common::GetCompetitor(args) != 0) // Is a trainer
                            {
                                Section_FromEvent_SwapItem::Description::Object* swapItemDesc = Section_FromEvent_SwapItem::Description::newInstance();
                                swapItemDesc->fields.userPokeID = pokeID;
                                swapItemDesc->fields.targetPokeID = (uint8_t)targetPoke;
                                swapItemDesc->fields.isIncRecordCount_StealItemFromWildPoke = true;

                                swapItemDesc->fields.successMessage1->Setup(BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::Dorobou);
                                swapItemDesc->fields.successMessage1->AddArg((int32_t)attackingPoke);
                                swapItemDesc->fields.successMessage1->AddArg((int32_t)targetPoke & 0xff);
                                swapItemDesc->fields.successMessage1->AddArg(item);

                                bool swapped = Common::SwapItem(args, &swapItemDesc);
                                if (swapped)
                                {
                                    Section_FromEvent_SetWazaEffectIndex::Description::Object* setWazaDesc = Section_FromEvent_SetWazaEffectIndex::Description::newInstance();
                                    setWazaDesc->fields.effectIndex = 1;
                                    Common::SetWazaEffectIndex(args, &setWazaDesc);
                                }
                            }
                            else // Is wild
                            {
                                Section_FromEvent_SetItem::Description::Object* setItemDesc = Section_FromEvent_SetItem::Description::newInstance();
                                setItemDesc->fields.userPokeID = pokeID;
                                setItemDesc->fields.targetPokeID = (uint8_t)targetPoke;
                                setItemDesc->fields.itemID = 0;

                                setItemDesc->fields.successMessage->Setup(BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::Dorobou);
                                setItemDesc->fields.successMessage->AddArg((int32_t)attackingPoke);
                                setItemDesc->fields.successMessage->AddArg((int32_t)targetPoke & 0xff);
                                setItemDesc->fields.successMessage->AddArg(item);

                                bool itemSet = Common::SetItem(args, &setItemDesc);
                                if (itemSet)
                                {
                                    if (ItemWork::IsAddItem(item, 1))
                                    {
                                        ItemWork::AddItem(item, 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};

// Remove the check for if the attacking Pokémon is holding an item (for wilds)
HOOK_DEFINE_REPLACE(Dorobou_CheckEnable) {
    static bool Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID)
    {
        BTL_POKEPARAM::Object *pokeparam = (*args)->fields.pBattleEnv->instance()->fields.m_pokecon->GetPokeParamConst(pokeID);
        uint16_t item = pokeparam->GetItem();

        if (Common::GetCompetitor(args) != 0) // Is a trainer
        {
            // Keep default behavior (true if attacking poke has no item)
            System::Int32_array *work = (*args)->fields.pMyFactor->fields.m_data->fields.work;
            if (work->max_length > 0)
            {
                if (work->m_Items[0] != 0)
                {
                    return false;
                }
            }

            return item == 0;
        }
        else // Is wild
        {
            // Always set to true (attacking poke has no item)
            return true;
        }
    }
};

void InstallHooks_Move_Thief() {
    Handler_Dorobou::InstallAtOffset(0x01807cb0);
    Dorobou_CheckEnable::InstallAtOffset(0x01d0e720);
}