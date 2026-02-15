#include "exlaunch.hpp"

#include "externals/Dpr/UI/UIManager.h"
#include "externals/UnityEngine/Mathf.h"

int32_t HorizontalRepeat(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount)
{
    if (vSelection == vCount - 1)
    {
        // Last Row
        return Dpr::UI::UIManager::Repeat(hSelection, 0, lasthCount - 1);
    }
    else
    {
        // Any other row
        return Dpr::UI::UIManager::Repeat(hSelection, 0, hCount - 1);
    }
}

int32_t HorizontalClamp(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount)
{
    if (vSelection == vCount - 1)
    {
        // Last Row
        return UnityEngine::Mathf::Clamp(hSelection, 0, lasthCount - 1);
    }
    else
    {
        // Any other row
        return UnityEngine::Mathf::Clamp(hSelection, 0, hCount - 1);
    }
}

int32_t VerticalRepeat(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount)
{
    if (hSelection + 1 > lasthCount)
    {
        // Column with a missing item on last row
        return Dpr::UI::UIManager::Repeat(vSelection, 0, vCount - 2);
    }
    else
    {
        // Any other column
        return Dpr::UI::UIManager::Repeat(vSelection, 0, vCount - 1);
    }
}

int32_t VerticalClamp(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount)
{
    if (hSelection + 1 > lasthCount)
    {
        // Column with a missing item on last row
        return UnityEngine::Mathf::Clamp(vSelection, 0, vCount - 2);
    }
    else
    {
        // Any other column
        return UnityEngine::Mathf::Clamp(vSelection, 0, vCount - 1);
    }
}