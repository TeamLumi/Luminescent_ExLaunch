#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/Pml/Item/ItemData.h"
#include "externals/System/Int32Class.h"
#include "externals/System/String.h"

System::String::Object* GetBikeSpriteName(int32_t itemId, bool isLarge)
{
    auto iconID = Pml::Item::ItemData::GetParam((uint16_t)itemId, Pml::Item::ItemData::PrmID::ICONID);
    auto color = (int32_t)PlayerWork::get_bicycleColor();
    auto baseName = System::String::Concat(System::String::Create("item_"),
                                           System::Int32Class::ToString(&iconID, System::String::Create("0000")),
                                           System::String::Create("_"),
                                           System::Int32Class::ToString(&color, System::String::Create("0")));

    if (isLarge)
        return System::String::Concat(baseName, System::String::Create("_L"));
    else
        return baseName;
}