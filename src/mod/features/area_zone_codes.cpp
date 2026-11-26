#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "utils/utils.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(EvDataManager_ARRIVEDATA_SetArriveFlag) {
    static void Callback(Dpr::EvScript::EvDataManager::Object* __this, int32_t id) {
        int32_t sysFlag = ConvertZoneIDToArriveFlag(id);
        FlagWork::SetSysFlag(sysFlag, true);
    }
};

HOOK_DEFINE_TRAMPOLINE(EvDataManager$$WarpUpdateEnd) {
    static void Callback(Dpr::EvScript::EvDataManager::Object* __this) {
        system_load_typeinfo(game::TypeInfo::Dpr_EvScript_EvDataManager_WarpUpdate);

        PlayerWork::getClass()->initIfNeeded();
        int32_t zoneID = PlayerWork::get_zoneID();
        auto zoneCode = Convert_ZoneID(nullptr, &zoneID)->asCString();
        auto label = System::String::Create("SP_" + zoneCode + "_END");

        // We need to clear the callback, otherwise we recursively call WarpUpdateEnd
        __this->fields._eventEndDelegate = nullptr;

        // We don't wait for the script, if a command takes more than one frame, it's over
        __this->JumpLabel(label, nullptr);
        __this->UpdateEvdata(0.0f, true);
        Orig(__this);
    }
};

void area_codes_main() {
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;

    // Dpr.EvScript.EvDataManager.<PreRequestAssetSetUp>d__1779$$MoveNext
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_PreRequestAssetSetUp_d_1779_MoveNext + 0xA0);
    p.BranchLinkInst((void*)&Convert_AreaID);
    p.WriteInst(MovRegister(X21, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager.<RequestAssetSetUp>d__1778$$MoveNext
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_RequestAssetSetUp_d_1778_MoveNext + 0x3C0);
    p.BranchLinkInst((void*)&Convert_AreaID);
    p.WriteInst(MovRegister(X21, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager.<StartUpCreate>d__1773$$MoveNext
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_StartUpCreate_d_1783_MoveNext + 0x484);
    p.BranchLinkInst((void*)&Convert_AreaID);

    // Dpr.EvScript.EvDataManager$$CreateWorpPoint
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_CreateWorpPoint + 0x1A8);
    p.BranchLinkInst((void*)&Convert_AreaID);

    // Dpr.EvScript.EvDataManager$$CheckPlaceData
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_CheckPlaceData + 0x58);
    p.BranchLinkInst((void*)&Convert_AreaID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$CreateWarpEntity
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_CreateWarpEntity + 0xB4);
    p.BranchLinkInst((void*)&Convert_AreaID);
}

void zone_codes_main() {
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;

    // Viewer$$CreateLoadMenu
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x5D8);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X22, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x97C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X22, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0xD88);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X24, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x1068);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X28, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x113C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X27, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x184C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X28, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x1CB4);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x1D6C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x1E34);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Viewer_CreateLoadMenu + 0x1F2C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));

    // TODO: Figure out a way to make these UG patches work properly. ILCA's compiler did some weird stuff and it's breaking...
    // UgMiniMapComponent$$Initialize
    /*p.Seek(0x018d7040);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x24));
    p.Seek(0x018d713c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X26, X0));
    p.WriteInst(Branch(0x28));

    // UgMiniMapComponent$$GetMapGroupGoneFlag
    p.Seek(0x018d7b3c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x28));
    p.Seek(0x018d7d58);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X22, X0));
    p.WriteInst(Branch(0x28));

    // UgMiniMapComponent$$UpdateSecretBase
    p.Seek(0x018da108);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X26, X0));
    p.WriteInst(Branch(0x24));

    // UgMiniMapComponent$$UpdateLandMarkIcons
    p.Seek(0x018da550);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X24, X0));
    p.WriteInst(Branch(0x28));*/

    // ColiseumRoomManager$$SetZoneData
    p.Seek(game::Address::ColiseumRoomManager_SetZoneData + 0x50);
    p.BranchLinkInst((void*)&Convert_ZoneID);

    // Dpr.EvScript.EvDataManager$$SpLabel_Init
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_SpLabel_Init + 0x4C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$SpLabel_Obj
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_SpLabel_Obj + 0x4C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$SpLabel_Flag
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_SpLabel_Flag + 0x4C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$SpLabel_Scene
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_SpLabel_Scene + 0x4C);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$StackMapChange
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_StackMapChange + 0xC4);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$EvCmdShopOpen
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_EvCmdShopOpen + 0x134);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_EvCmdShopOpen + 0x168);
    p.WriteInst(Nop());
}

void arrive_flag_main() {
    EvDataManager_ARRIVEDATA_SetArriveFlag::InstallAtOffset(game::Address::Dpr_EvScript_EvDataManager_ARRIVEDATA_SetArriveFlag);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;

    // Dpr.EvScript.EvDataManager$$MapJumpInit
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_MapJumpInit + 0x90);
    p.BranchLinkInst((void*)&ConvertZoneIDToArriveFlag);
    p.WriteInst(Branch(0x14));

    // Dpr.EvScript.EvDataManager$$WalkInit
    p.Seek(game::Address::Dpr_EvScript_EvDataManager_WalkInit + 0x64);
    p.BranchLinkInst((void*)&ConvertZoneIDToArriveFlag);
    p.WriteInst(Branch(0x14));
}

void extra_map_script_main() {
    EvDataManager$$WarpUpdateEnd::InstallAtOffset(game::Address::Dpr_EvScript_EvDataManager_WarpUpdateEnd);
}

void exl_area_zone_codes_main() {
    area_codes_main();
    zone_codes_main();
    arrive_flag_main();
    extra_map_script_main();
}
