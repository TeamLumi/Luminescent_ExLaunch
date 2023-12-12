#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/EvScript/EvScriptData.h"
#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/Effect/EffectInstance.h"
#include "externals/EvData/Aregment.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/MulticastDelegate.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/Quaternion.h"
#include "externals/UnityEngine/RectInt.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::EvScript {
    struct EvDataManager : ILClass<EvDataManager, 0x04c59c50> {
        struct WorpEventData : ILStruct<WorpEventData> {
            struct Fields {
                void* Entity;
                int32_t State;
                float Time;
            };
        };

        struct EvCallData : ILStruct<EvCallData> {
            struct Fields {
                int32_t EventListIndex;
                int32_t LabelIndex;
                int32_t CommandIndex;
            };
        };

        struct MsgOpenParam : ILStruct<MsgOpenParam> {
            struct Fields {
                void* MsbtFile;
                void* Label;
                int32_t LabelIndex;
                int32_t WindowType;
                bool Input;
                void* TrainerName;
                int32_t EndType;
                bool PlayTextFeedSe;
            };
        };

        struct EventEndDelegate : ILClass<EventEndDelegate> {
            struct Fields : System::MulticastDelegate::Fields {
                void* Entity;
                int32_t State;
                float Time;
            };
        };

        struct EntityParam : ILClass<EntityParam> {
            struct Fields {
                System::String::Object* TalkLabel;
                System::String::Object* ContactLabel;
                int32_t VanishFlagIndex;
                bool IsContact;
                int32_t WorkIndex;
                int32_t WorkValue;
                int32_t Type;
                UnityEngine::RectInt::Object StopGridArea;
                bool IsEventRunning;
                int32_t MoveCodeIndex;
                bool Kairiki;
                bool Iwakudaki;
                bool Iaigiri;
                bool Rockclimb;
                int32_t Dowsing;
                bool SnowBall;
                bool AzukariyaOldMan;
                bool HoneyTree;
                bool HeightIgnore;
                UnityEngine::Vector2::Object ContactSize;
                bool IsContactCenter;
                bool Sit;
                int32_t IdleAnimOverride;
                int32_t WalkAnimOverride;
                int32_t TrainerID;
                int32_t FieldObjectIndex;
                System::Int32_array* NearObject;
                float TalkRange;
                bool isTalkHit;
                float TalkAngle;
                bool SaveObject;
                int32_t LocaitionZoneID;
                bool IsObject;
                UnityEngine::Vector2::Object TalkSegment;
                uint8_t TalkBit;
                int32_t CharacterGraphicsIndex;
                bool BattleReturnNotRotate;
                bool isNoPlayerHit;
                bool IsInvalidVanishActive;
            };
        };

        struct DisplayClass773_0 : ILClass<DisplayClass773_0, 0x04c5edf0> {
            struct Fields {
                Pml::PokePara::PokemonParam::Object* pokemonParam;
                int32_t trayIndex;
                int32_t index;
                EvDataManager::Object * __4__this;
            };

            inline void ctor() {
                external<void>(0x019aeef0, this);
            }
        };

        struct DisplayClass1544_0 : ILClass<DisplayClass1544_0, 0x04c5f000> {
            struct Fields {
                EvDataManager::Object* __4__this;
                Pml::PokePara::PokemonParam::Object* param;
                int32_t tray;
                int32_t idx;
            };

            inline void ctor() {
                external<void>(0x019ae530, this);
            }
        };

        enum class TalkState : int32_t {
            Init = 0,
            EndWait = 1,
            CloseWait = 2
        };

        struct Fields {
            void * OnTalkStartCallBack;
            bool _isScriptLoad;
            void * _eventList;
            int32_t _eventListIndex;
            void * _callQueue;
            void * _findAllLabel;
            void * _hit_object;
            void * _hit_object_sub;
            UnityEngine::Vector3::Object _hit_position;
            int32_t _cmp_flag;
            void * _scriptOperation;
            int32_t _areaID;
            void * _warpList;
            void * _warpData;
            void * _warpRoot;
            void * _se_datas;
            void * _voice_datas;
            UnityEngine::Vector2Int::Object _eventEndPosition;
            void * _posEventLabelReserve;
            void * _entityParamList;
            void * _stopRoot;
            bool _isInitFirstMap;
            void * __dummyPlayer_k__BackingField;
            void * _updateDelegate;
            EventEndDelegate::Object * _eventEndDelegate;
            void * _doorEntity;
            Dpr::EvScript::EvDataManager::WorpEventData::Object _worpEventData;
            UnityEngine::Vector2Int::Object _eqZoneWorpGrid;
            bool _isEqZoneWorp;
            void * _liftEntity;
            void * _tobariGymWallEntity;
            void * _nagisaGymGearEntity;
            void * _nomoseGymSwitchEntity;
            void * _eyePaintingEntity;
            void * _embankmentEntity;
            void * _mistPlate;
            void * _PokemonCenter_k__BackingField;
            void * _Telescope_k__BackingField;
            void * _TelescopeNagisa_k__BackingField;
            bool AzukariyaInitEventFlag;
            void * _FieldWazaCutIn_k__BackingField;
            void * _InterviewWork_k__BackingField;
            void * TvCommercials;
            int32_t TvCommercialsCurrentIndex;
            void * _FieldShip_k__BackingField;
            void * _callLabel_SceneChange;
            void * _callLabel_UpdateSP;
            void * _callLabel_AdjustHeroPos;
            void * FieldObjectMoveCodes;
            bool _lateUpdateMoveCode;
            bool _isCall_TrainerBtl;
            Dpr::EvScript::EvDataManager::EvCallData::Object _battleReturnData;
            void * _battleReturnHitObject;
            void * _eyeEncountTarget;
            void * _eyeEncountBallon;
            float _eyeEncountWait;
            int32_t _eyeEncountSeq;
            void * _talkTrinerIndex;
            int32_t Btl_TrainerType1;
            int32_t Btl_TrainerType2;
            int32_t _ugSeq;
            UnityEngine::Vector3::Object _ugEndPos;
            UnityEngine::Vector3::Object _ugHoleRot;
            float _ugFallSpdCurrent;
            int32_t _ugDiveSeq;
            float _ugDiveUpdateTime;
            float _ugDiveRotTime;
            float _ugDiveRotStart;
            float _ugDiveRotEnd;
            float _ugDiveJumpingTime;
            int32_t _ugFromSeq;
            float _ugFromJumpTime;
            float _ugFromRotSpdCurrent;
            void * _ugNextJumpPos;
            void * _toUgInvisibleObjects;
            int32_t _cacheZoneID_SceneChange;
            bool _pendingInitScripts;
            int32_t SorawotobuZoneId;
            int32_t SorawotobuLocatorIndex;
            void * _evCameraTable;
            void * _tairyouHasseiMane;
            bool _isFadeEventReturnInput;
            float _cloudSpeed;
            float _cloudTime;
            int32_t _switch_work_index;
            float _timeWait;
            void * _msgWindowParam;
            void * _msgWindowParamOther;
            Dpr::EvScript::EvDataManager::MsgOpenParam::Object _msgOpenParam;
            bool _isAutoMsg;
            int32_t _eMsgSpeed;
            float _autoTime;
            int32_t _boardState;
            void * _msgWindow;
            void * _msgWindowOther;
            void * _msgWindowCoroutine;
            TalkState _talkStart;
            int32_t _macroCmd;
            int32_t _procCmd;
            void * _talkOpenMsbt;
            void * _talkOpenLabel;
            float _turnEndFrame_Hero;
            float _turnEndFrame_Obj;
            void * _turnTime;
            void * _turnEndQuaternion;
            void * _turnDiffFlag;
            float _deltatime;
            void * _mapChangeZoneID;
            int32_t _mapChangeIndex;
            UnityEngine::Quaternion::Object _moveGridCenterStart;
            UnityEngine::Quaternion::Object _moveGridCenterEnd;
            bool _isOpenSubContentsMenu;
            bool _isWaitCheckOnlineAccount;
            System::String::Object* _custumWindow_msbt;
            System::Collections::Generic::List$$String::Object* _custumWindow_Labels;
            System::Collections::Generic::List$$int32_t::Object* _custumWindow_RetIndex;
            void * _selectDoorObject;
            float _fadeSpeed;
            int32_t _heroReqBit;
            int32_t _heroMoveGridCenterFrontDir;
            bool _heroMoveGridCenterFrontStat;
            void * _fieldObjectMove;
            void * _fieldObjectRotateYaw;
            void * _fieldFloatMove;
            int32_t _hidenSequence;
            UnityEngine::Vector3::Object _takiTargetPosition;
            UnityEngine::Vector3::Object _rockClimbingEndPos;
            UnityEngine::Vector3::Object _rockClimbingStandPos;
            bool _hidenEffectWait;
            bool _rideBicycleReserve;
            void * _runEventLiftEntity;
            int32_t _liftSequence;
            int32_t _gearSequence;
            int32_t _waterSequence;
            int32_t _kinomiSequence;
            float _kinomiSequenceTime;
            void * _kinomiEffect;
            int32_t _warpSequence;
            int32_t _warpSpeedSequence;
            float _warpSpeedSequenceTime;
            int32_t _scopeSequence;
            int32_t _azukariyaSequence;
            int32_t _pokelistSequence;
            int32_t _pokelistBox;
            int32_t _pokelistIndex;
            int32_t _trainSequence;
            int32_t _itemSequence;
            int32_t _btwrSequence;
            bool _isOpenSpecialWin;
            int32_t _openSpecialWinLabelSelected;
            int32_t _currentTvProgram;
            int32_t _cutinState;
            bool _isShopOpen;
            int32_t _bagSelectItemNo;
            void * _floorWindow;
            void * _moneyWindow;
            void * matchingPassword;
            int32_t _openTownMapSeq;
            bool _isOpenBtlTowerRecode;
            int32_t _softwareKeyboardSubState;
            int32_t _effSeq;
            bool _pc_window_close;
            int32_t _dendou;
            void * _umaAnimatorCtr;
            bool _isOpenCustomBallTrainer;
            int32_t _nicknamePlacementSequence;
            Effect::EffectInstance::Array * _scriptEffects;
            UnityEngine::Coroutine::Array * _scriptScaleCorutine;
            void * _scriptScaleVectol;
            void * _temp_PokePara;
            bool _isBattleTowerBtl;
            bool _isBattleTowerWin;
            UnityEngine::Vector2::Object _playerMoveGridCenterAngle;
            Dpr::EvScript::EvScriptData::Object * _evData;
            void * _evScript;
            void * _evCommand;
            EvData::Aregment::Array * _evArg;
            int32_t btlsearchSeq;
            void * btlserchAudio;
            bool _isOpenHallOfFame;
            void * _cmdReportSaveCoroutine;
            int32_t _seqRankingView;
            bool _isOpenCertificate;
            int32_t returnSequenceID;
            bool _boukennootoTipsOpen;
            int32_t _fashionLoadSeq;
            void * _oldfashionAssetReqUri;
            void * _fashionAssetReqOp;
            float _fashionYawAngle;
            UnityEngine::Vector3::Object _fashionWorldpos;
            bool _isTraining;
            bool _isOpenTraining;
            void * _setlight_timer;
            void * _setlight_timer_limit;
            void * _start_lightIntensity;
            void * _end_lightIntensity;
            void * _isRunningLight;
            bool isSpBtlAruseus;
            void * Debug_01_DebugLabel;
            void * DebugSceneEventLabel;
            void * _fieldObjectEntity;
            void * _FieldKinomiGrowEntity;
            void * _AssetReqOpeList;
            void * _loadObjectList;
            int32_t _loadObjectIndex;
            void * _loadObjectParent;
            bool _nowInstantiate;
            void * _poolLoadObjects;
        };

        static inline StaticILMethod<0x04c2afe0, int32_t, int32_t> PTR_Method$$EvDataManager_EvCmdCallWazaOmoidashiUi {};
        static inline StaticILMethod<0x04c2b028, int32_t, int32_t> PTR_Method$$EvDataManager_EvCmdCallWazaOshieUi {};

        static inline StaticILMethod<0x04c77ce0, int32_t, int32_t> Method$$EvDataManager_EvCmdNameInPoke_OnInputCheck {};
        static inline StaticILMethod<0x04c7cf70, int32_t, int32_t> Method$$EvDataManager_EvCmdNameInPoke_OnComplete {};
        static inline StaticILMethod<0x04c7cfd0>                   Method$$EvDataManager_CmdFirstPokeSelectProc {};

        static inline MethodInfo* Method$$EvCmdCallWazaOmoidashiUiParty = nullptr;
        static MethodInfo* getMethod$$EvCmdCallWazaOmoidashiUiParty(Il2CppMethodPointer method) {
            if (Method$$EvCmdCallWazaOmoidashiUiParty == nullptr)
                Method$$EvCmdCallWazaOmoidashiUiParty = (*PTR_Method$$EvDataManager_EvCmdCallWazaOmoidashiUi)->copyWith(method);
            return Method$$EvCmdCallWazaOmoidashiUiParty;
        };

        static inline MethodInfo* Method$$EvCmdCallWazaOshieUiParty = nullptr;
        static MethodInfo* getMethod$$EvCmdCallWazaOshieUiParty(Il2CppMethodPointer method) {
            if (Method$$EvCmdCallWazaOshieUiParty == nullptr)
                Method$$EvCmdCallWazaOshieUiParty = (*PTR_Method$$EvDataManager_EvCmdCallWazaOshieUi)->copyWith(method);
            return Method$$EvCmdCallWazaOshieUiParty;
        };

        inline bool RunEvCmd(int32_t index) {
            return external<bool>(0x02c5b290, this, index);
        }

        inline int32_t GetPokemonFormNo(Pml::PokePara::PokemonParam::Object* index) {
            return external<int32_t>(0x02c67ff0, this, index);
        }

        inline Pml::PokePara::PokemonParam::Object* GetPokemonParam(int32_t trayIndex, int32_t index) {
            return external<Pml::PokePara::PokemonParam::Object*>(0x02c67f20, this, trayIndex, index);
        }

        inline bool JumpLabel(System::String::Object* label, EventEndDelegate::Object* callback) {
            return external<bool>(0x02c423c0, this, label, callback);
        }

        inline bool Cmd_TalkMsg(System::String::Object* msbt, System::String::Object* label) {
            return external<bool>(0x02c567f0, this, msbt, label);
        }

        inline bool EvCmdOpenBoutiqueShopChange() {
            return external<bool>(0x02c8a810, this);
        }

        inline void LearnWaza(Pml::PokePara::PokemonParam::Object *param, int32_t learnWazaNo, int32_t unlearnWazaNo) {
            external<void>(0x02c92a70, this, param, learnWazaNo, unlearnWazaNo);
        }

        inline int32_t GetBadgeCount() {
            return external<int32_t>(0x02c4ea10, this);
        }

        inline bool IsRunningEvent() {
            return external<bool>(0x02c423a0, this);
        }

        inline void SetBattleReturn() {
            external<void>(0x02c45c90, this);
        }

        static inline Dpr::EvScript::EvDataManager::Object* get_Instanse() {
            return external<Dpr::EvScript::EvDataManager::Object*>(0x02c3d4d0);
        }
    };
}
