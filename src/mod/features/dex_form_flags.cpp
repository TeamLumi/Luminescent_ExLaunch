#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/Pml/Sex.h"
#include "externals/ZukanWork.h"

#include "save/save.h"

void SetCaughtFlags(DPData::ZUKAN_WORK::Object* zukanData, uint32_t monsno, Pml::Sex sex, int32_t form, bool color, bool isGet) {
    switch (sex)
    {
        case Pml::Sex::MALE:
            if (color)
                zukanData->fields.male_color_flag->m_Items[monsno-1] = isGet;
            else
                zukanData->fields.male_flag->m_Items[monsno-1] = isGet;
            break;

        case Pml::Sex::FEMALE:
            if (color)
                zukanData->fields.female_color_flag->m_Items[monsno-1] = isGet;
            else
                zukanData->fields.female_flag->m_Items[monsno-1] = isGet;
            break;

        case Pml::Sex::UNKNOWN:
            if (color)
            {
                zukanData->fields.male_color_flag->m_Items[monsno-1] = isGet;
                zukanData->fields.female_color_flag->m_Items[monsno-1] = isGet;
            }
            else
            {
                zukanData->fields.male_flag->m_Items[monsno-1] = isGet;
                zukanData->fields.female_flag->m_Items[monsno-1] = isGet;
            }
            break;

        default:
            break;
    }

    getCustomSaveData()->dexForms.elements[monsno-1].setFlag(form, color, isGet);
}

HOOK_DEFINE_REPLACE(ZukanWork$$SetPoke) {
    static void Callback(uint32_t monsno, DPData::GET_STATUS get, Pml::Sex sex, int32_t form, bool color) {
        system_load_typeinfo(0xac82);
        PlayerWork::getClass()->initIfNeeded();
        ZukanWork::getClass()->initIfNeeded();

        if (monsno > DexSize)
            return;

        auto zukanData = PlayerWork::get_zukan();

        if (zukanData->fields.get_status->m_Items[monsno-1] <= get)
            zukanData->fields.get_status->m_Items[monsno-1] = get;

        SetCaughtFlags(zukanData, monsno, sex, form, color, true);

        PlayerWork::set_zukan(zukanData);
    }
};

HOOK_DEFINE_REPLACE(ZukanWork$$IsGet) {
    static bool Callback(uint32_t monsno, Pml::Sex sex, int32_t form, bool color) {
        if (monsno > DexSize)
            return false;

        return getCustomSaveData()->dexForms.elements[monsno-1].getFlag(form, color);
    }
};

HOOK_DEFINE_REPLACE(ZukanWork$$DebugSet) {
    static void Callback(uint32_t monsno, DPData::GET_STATUS get, Pml::Sex sex, int32_t form, bool color, bool isGet) {
        system_load_typeinfo(0xac6f);
        PlayerWork::getClass()->initIfNeeded();
        ZukanWork::getClass()->initIfNeeded();

        if (monsno > DexSize)
            return;

        auto zukanData = PlayerWork::get_zukan();

        zukanData->fields.get_status->m_Items[monsno-1] = get;

        SetCaughtFlags(zukanData, monsno, sex, form, color, isGet);

        PlayerWork::set_zukan(zukanData);
    }
};

void exl_dex_form_flags_main() {
    ZukanWork$$SetPoke::InstallAtOffset(0x017dbce0);
    ZukanWork$$IsGet::InstallAtOffset(0x017dcc90);
    ZukanWork$$DebugSet::InstallAtOffset(0x017dd7e0);
}
