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
        system_load_typeinfo(0x4672);

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
    exl::patch::CodePatcher p(0);

    // Dpr.EvScript.EvDataManager.<PreRequestAssetSetUp>d__1779$$MoveNext
    p.Seek(0x019afe40);
    p.BranchLinkInst((void*)&Convert_AreaID);
    p.WriteInst(MovRegister(X21, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager.<RequestAssetSetUp>d__1778$$MoveNext
    p.Seek(0x019b07c0);
    p.BranchLinkInst((void*)&Convert_AreaID);
    p.WriteInst(MovRegister(X21, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager.<StartUpCreate>d__1773$$MoveNext
    p.Seek(0x019b1a34);
    p.BranchLinkInst((void*)&Convert_AreaID);

    // Dpr.EvScript.EvDataManager$$CreateWorpPoint
    p.Seek(0x02c401a8);
    p.BranchLinkInst((void*)&Convert_AreaID);

    // Dpr.EvScript.EvDataManager$$CheckPlaceData
    p.Seek(0x02c52db8);
    p.BranchLinkInst((void*)&Convert_AreaID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$CreateWarpEntity
    p.Seek(0x02ca2a94);
    p.BranchLinkInst((void*)&Convert_AreaID);
}

void zone_codes_main() {
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);

    // Viewer$$CreateLoadMenu
    p.Seek(0x017ecae8);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X22, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ece8c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X22, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ed298);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X24, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ed578);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X28, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ed64c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X27, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017edd5c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X28, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ee1c4);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ee27c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ee344);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x017ee43c);
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
    p.Seek(0x018e9190);
    p.BranchLinkInst((void*)&Convert_ZoneID);

    // Dpr.EvScript.EvDataManager$$SpLabel_Init
    p.Seek(0x02c42bbc);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$SpLabel_Obj
    p.Seek(0x02c42c8c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$SpLabel_Flag
    p.Seek(0x02c42d5c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$SpLabel_Scene
    p.Seek(0x02c42e2c);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X20, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$StackMapChange
    p.Seek(0x02c59104);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x24));

    // Dpr.EvScript.EvDataManager$$EvCmdShopOpen
    p.Seek(0x02c88ef4);
    p.BranchLinkInst((void*)&Convert_ZoneID);
    p.WriteInst(MovRegister(X23, X0));
    p.WriteInst(Branch(0x20));
    p.Seek(0x02c88f28);
    p.WriteInst(Nop());
}

void arrive_flag_main() {
    EvDataManager_ARRIVEDATA_SetArriveFlag::InstallAtOffset(0x02c436a0);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);

    // Dpr.EvScript.EvDataManager$$MapJumpInit
    p.Seek(0x02c426c0);
    p.BranchLinkInst((void*)&ConvertZoneIDToArriveFlag);
    p.WriteInst(Branch(0x14));

    // Dpr.EvScript.EvDataManager$$WalkInit
    p.Seek(0x02c425f4);
    p.BranchLinkInst((void*)&ConvertZoneIDToArriveFlag);
    p.WriteInst(Branch(0x14));
}

void extra_map_script_main() {
    EvDataManager$$WarpUpdateEnd::InstallAtOffset(0x02c4da20);
}

void exl_area_zone_codes_main() {
    area_codes_main();
    zone_codes_main();
    arrive_flag_main();
    extra_map_script_main();
}
