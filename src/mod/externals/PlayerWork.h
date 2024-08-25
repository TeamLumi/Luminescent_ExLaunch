#pragma once

#include "externals/il2cpp-api.h"

#include "externals/AzukariyaData.h"
#include "externals/DPData/_DENDOU_SAVEDATA.h"
#include "externals/DPData/_FIELDOBJ_SAVE.h"
#include "externals/DPData/_RANDOM_GROUP.h"
#include "externals/DPData/BadgeSaveData.h"
#include "externals/DPData/BoukenNote.h"
#include "externals/DPData/BTLTOWER_SAVEWORK.h"
#include "externals/DPData/CON_PHOTO_LANG_DATA.h"
#include "externals/DPData/DENDOU_SAVE_ADD.h"
#include "externals/DPData/ENC_SV_DATA.h"
#include "externals/DPData/FieldGimmickSaveData.h"
#include "externals/DPData/GMS_DATA.h"
#include "externals/DPData/KinomiGrowSaveData.h"
#include "externals/DPData/MysteryGiftSaveData.h"
#include "externals/DPData/PLAYER_DATA.h"
#include "externals/DPData/PLAYER_NETWORK_DATA.h"
#include "externals/DPData/PLAYER_SAVE_DATA.h"
#include "externals/DPData/PoffinSaveData.h"
#include "externals/DPData/POKETCH_DATA.h"
#include "externals/DPData/POKETCH_POKETORE_COUNT.h"
#include "externals/DPData/RECORD.h"
#include "externals/DPData/RECORD_ADD_DATA.h"
#include "externals/DPData/SYSTEMDATA.h"
#include "externals/DPData/TOPMENU_WORK.h"
#include "externals/DPData/TR_BATTLE_DATA.h"
#include "externals/DPData/TV_DATA.h"
#include "externals/DPData/UgCountRecord.h"
#include "externals/DPData/UgSaveData.h"
#include "externals/DPData/UnionSaveData.h"
#include "externals/DPData/ZUKAN_PERSONAL_RND_DATA.h"
#include "externals/DPData/ZUKAN_WORK.h"
#include "externals/Dpr/BallDeco/SaveBallDecoData.h"
#include "externals/Dpr/BallDeco/SaveBallDecoExtraData.h"
#include "externals/Dpr/Box/SaveBoxData.h"
#include "externals/Dpr/Box/SaveBoxTrayData.h"
#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/Dpr/Item/SaveItem.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/MT_DATA.h"
#include "externals/PLAYREPORT_DATA.h"
#include "externals/Pml/PokePara/SavePokeParty.h"
#include "externals/Pml/PokeParty.h"
#include "externals/ReBuffnameData.h"
#include "externals/SmartPoint/Components/PlayerPrefsProvider.h"
#include "externals/System/Nullable.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/Dpr/Box/SaveBoxData.h"

struct PlayerWork : ILClass<PlayerWork, 0x04c59b58> {
    struct SaveData : ILStruct<SaveData> {
        struct Fields {
            int32_t version;
            System::Int32_array* intValues;
            System::Boolean_array* boolValues;
            System::Boolean_array* systemFlags;
            System::String::Object* rivalName;
            int32_t zoneID;
            float timeScale;
            Dpr::Item::SaveItem::Array* saveItem;
            void* saveUgItem;
            void* saveItemShortcut;
            Pml::PokePara::SavePokeParty::Object playerParty;
            Dpr::Box::SaveBoxData::Object boxData;
            Dpr::Box::SaveBoxTrayData::Array* boxTray;
            DPData::PLAYER_DATA::Object playerData;
            DPData::ZUKAN_WORK::Object zukanData;
            DPData::TR_BATTLE_DATA::Array* tr_battleData;
            DPData::TOPMENU_WORK::Object topMenuData;
            DPData::_FIELDOBJ_SAVE::Object fieldObj_Save;
            DPData::RECORD_ARRAY::Object record;
            DPData::ENC_SV_DATA::Object enc_sv_data;
            DPData::PLAYER_SAVE_DATA::Object player_save_data;
            Dpr::BallDeco::SaveBallDecoData::Object ballDecoData;
            void* saveSeal;
            DPData::_RANDOM_GROUP::Object randomGroup;
            DPData::FieldGimmickSaveData::Object fieldGimmickSaveData;
            DPData::KinomiGrowSaveData::Object kinomiGrowSaveData;
            DPData::PoffinSaveData::Object poffinSaveData;
            DPData::BTLTOWER_SAVEWORK::Object btlTowerSave;
            DPData::SYSTEMDATA::Object systemData;
            DPData::POKETCH_DATA::Object poketchData;
            AzukariyaData::Object azukariyaData;
            DPData::_DENDOU_SAVEDATA::Object dendoudata;
            DPData::BadgeSaveData::Object badgeSaveData;
            DPData::BoukenNote::Object boukenNote;
            void* tvDataOld;
            DPData::UgSaveData::Object ugSaveData;
            DPData::GMS_DATA::Object gmsdata;
            DPData::PLAYER_NETWORK_DATA::Object networkdata;
            DPData::UnionSaveData::Object unionSaveData;
            DPData::CON_PHOTO_LANG_DATA::Object contstPhotoLangData;
            DPData::ZUKAN_PERSONAL_RND_DATA::Object zukanPersonalRndData;
            void* contestPhotoExtData;
            void* gmsPointExtData;
            DPData::UgCountRecord::Object ugCountRecord;
            ReBuffnameData::Object reBuffNameData;
            void* saveDataHash;
            DPData::RECORD_ADD_DATA::Object recodeAddData;
            DPData::MysteryGiftSaveData::Object mysteryGiftSaveData;
            DPData::POKETCH_POKETORE_COUNT_ARRAY::Object poketoreCountArray;
            PLAYREPORT_DATA::Object playReportData;
            MT_DATA::Object mtData;
            DPData::DENDOU_SAVE_ADD::Object dendouSaveAdd;
            DPData::TV_DATA::Object tvData;
            Dpr::BallDeco::SaveBallDecoExtraData::Object ballDecoExtraData;
        };

        static_assert(offsetof(Fields, systemFlags) == 0x18);
        static_assert(sizeof(System::Boolean_array*) == 0x8);
        static_assert(offsetof(Fields, tr_battleData) == 0x220);
        static_assert(offsetof(Fields, kinomiGrowSaveData) == 0x340);
        static_assert(offsetof(Fields, poffinSaveData) == 0x350);
        static_assert(offsetof(Fields, btlTowerSave) == 0x360);
        static_assert(offsetof(Fields, systemData) == 0x388);
        static_assert(sizeof(DPData::SYSTEMDATA::Fields) == 0x6c);
        static_assert(offsetof(Fields, azukariyaData) == 0x438);
        static_assert(offsetof(Fields, badgeSaveData) == 0x468);
        static_assert(offsetof(Fields, ugCountRecord) == 0x590);
        static_assert(offsetof(Fields, ballDecoExtraData) == 0x7a8);
        static_assert(sizeof(Fields) == 0x7b8);
    };

    struct StaticFields {
        bool _NoSavePlayerPos_k__BackingField;
        int32_t _capturedMonsNo_k__BackingField;
        int32_t _capturedBallCountTvTopic_k__BackingField;
        bool _encountEnable_k__BackingField;
        bool _isDebugWalk_k__BackingField;
        bool _isDebugPlayerVisible_k__BackingField;
        bool _isDebugBattleTextView_k__BackingField;
        bool _isPlayerInputActive;
        bool _isPlayerInputActive_ContactEvent;
        bool IsFieldKuruKuruStart;
        bool IsFieldFallStart;
        bool IsToUg;
        bool IsFromUg;
        bool ReserveZoneChangeNaminoriEnd;
        System::String::Object* _FieldWorpLinkName_k__BackingField;
        System::String::Object* _FieldWorpLabel_k__BackingField;
        bool _isDebugNpcVisble_k__BackingField;
        bool _isDebugNpcPokeVisble_k__BackingField;
        int32_t _DebugEncountMonsNo_k__BackingField;
        int32_t _FieldWeather_k__BackingField;
        int32_t _DebugWeek_k__BackingField;
        int32_t _FieldInputMode_k__BackingField;
        bool _IsZenmetuFlag_k__BackingField;
        bool _IsNeedUnloadOnFieldConnector_k__BackingField;
        int32_t _Warp_k__BackingField;
        bool _Telescope_k__BackingField;
        bool _TelescopeReturn_k__BackingField;
        bool _FieldCacheFlag_k__BackingField;
        int32_t _SafariBallNum_k__BackingField;
        int32_t _SafariStepCount_k__BackingField;
        void* _SafariCaptureMonsNo_k__BackingField;
        int32_t _IsCpuBoostMode_k__BackingField;
        Dpr::Item::ItemInfo::Object* _UsedFieldItem_k__BackingField;
        int32_t _UsedFieldWazaNo_k__BackingField;
        int32_t _UsedFieldWazaTemotiNo_k__BackingField;
        bool _Flash_k__BackingField;
        void* UsedFieldWazaInArea;
        bool _PoketchOpen_k__BackingField;
        int32_t _MovePokemonIndex_k__BackingField;
        bool _IsD26Key_k__BackingField;
        bool isNeedFixPokemonExp;
    };

    static_assert(offsetof(StaticFields, _SafariBallNum_k__BackingField) == 72);

    struct Fields : SmartPoint::Components::PlayerPrefsProvider<PlayerWork>::Fields {
        PlayerWork::SaveData::Object _saveData;
        int32_t _transitionZoneID;
        int32_t _locatorIndex;
        System::Nullable<DPData::LOCATION_WORK::Object>::Object _transitionLocation;
        bool _isBattling;
        bool _isContest;
        bool _isSealPreview;
        bool _isEvolveDemo;
        bool _isHatchDemo;
        bool _isMass;
        bool _isEncount;
        bool _isDigFossil;
        bool _isGMS;
        int32_t _loadResult;
        void* _battleSetupParam;
        Pml::PokeParty::Object* _playerParty;
        Pml::PokePara::PokemonParam::Object* _capturedPokemon;
        void* _evolveRequets;
        void* _writeSrcBuffer;
        bool _isMainSave;
        bool _isBackupSave;
        bool _autosaveCoroutineIsBusy;
        bool _autosaveWindowCoroutineIsBusy;
        void* _autosaveWindow;
        float _autosaveWindowCloseOpenTime;
    };

    static_assert(offsetof(Fields, _autosaveWindowCloseOpenTime) == 0x828);

    static inline Pml::PokeParty::Object* get_playerParty() {
        return external<Pml::PokeParty::Object*>(0x02ce2b50);
    }

    static inline DPData::CONFIG::Object* get_config() {
        return external<DPData::CONFIG::Object*>(0x02ceb800);
    }

    static inline DPData::ZUKAN_WORK::Object* get_zukan() {
        return external<DPData::ZUKAN_WORK::Object*>(0x02cf1040);
    }

    static inline void SetWarpSorawotobu(int32_t mapID, int32_t warpID) {
        external<void>(0x02cee520, mapID, warpID);
    }

    static inline bool GetSystemFlag(int32_t index) {
        return external<bool>(0x02ceda30, index);
    }

    static inline bool GetBool(int32_t index) {
        return external<bool>(0x02ce3e40, index);
    }

    static inline void SetBool(int32_t index, bool flag) {
        external<void>(0x02ced9a0, index, flag);
    }

    static inline int32_t GetInt(int32_t index) {
        return external<int32_t>(0x02ce2e20, index);
    }

    static inline void SetInt(int32_t index, int32_t value) {
        external<void>(0x02ced920, index, value);
    }

    static inline void SetMoney(int32_t money) {
        external<void>(0x02cefc80, money);
    }

    static inline int32_t get_zoneID() {
        return external<int32_t>(0x02ceea50);
    }

    static inline DPData::ENC_SV_DATA::Object get_Enc_SV_Data() {
        return external<DPData::ENC_SV_DATA::Object>(0x02cf2840, nullptr); // Passing nullptr as the "return_storage_ptr" and making this return the struct works
    }

    static inline DPData::TR_BATTLE_DATA::Array* get_tr_battleData() {
        return external<DPData::TR_BATTLE_DATA::Array*>(0x02cf1bd0, nullptr);
    }

    static inline void set_WalkEncountCount(int32_t value) {
        external<void>(0x02cf2c00, value);
    }

    static inline Dpr::Item::SaveItem::Object GetItem(int32_t itemno) {
        return external<Dpr::Item::SaveItem::Object>(0x02cefde0, itemno);
    }

    static inline int32_t get_cassetVersion() {
        return external<int32_t>(0x02cefa60);
    }

    static inline bool get_isPlayerInputActive() {
        return external<bool>(0x02cf1110);
    }

    static inline DPData::MYSTATUS::Object* get_playerStatus() {
        return external<DPData::MYSTATUS::Object*>(0x02ce2d20);
    }

    static inline void set_colorID(int32_t value) {
        external<void>(0x02cef870, value);
    }

    static inline DPData::PoffinSaveData::Object get_poffinSaveData() {
        return external<DPData::PoffinSaveData::Object>(0x02cf46d0);
    }

    static inline int32_t get_TureWalkMemberIndex() {
        return external<int32_t>(0x02cf5260);
    }

    static inline bool get_playerSex() {
        return external<bool>(0x02ce2cd0);
    }

    static inline int32_t get_defaultPokeType() {
        return external<int32_t>(0x02cf1320);
    }

    static inline int32_t get_rivalPokeType() {
        return external<int32_t>(0x02cf1590);
    }

    static inline int32_t get_supportPokeType() {
        return external<int32_t>(0x02cf1910);
    }

    static inline Dpr::Box::SaveBoxData::Object* GetBoxData() {
        return external<Dpr::Box::SaveBoxData::Object*>(0x02cf0180);
    }

    static inline Dpr::Message::MessageEnumData::MsgLangId get_msgLangID() {
        return external<Dpr::Message::MessageEnumData::MsgLangId>(0x02ce2c20);
    }

    static inline DPData::PLAYER_DATA::Object* get_playerData() {
        return external<DPData::PLAYER_DATA::Object*>(0x02ce2a50);
    }

    static inline System::String::Object* get_rivalName() {
        return external<System::String::Object*>(0x02cee9a0);
    }
};
