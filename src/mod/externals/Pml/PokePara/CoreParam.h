#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/Accessor.h"
#include "externals/Pml/PokePara/Condition.h"
#include "externals/Pml/PokePara/EggCheckType.h"
#include "externals/Pml/PokePara/OwnerInfo.h"
#include "externals/Pml/PokePara/PowerID.h"
#include "externals/Pml/PokePara/RareType.h"
#include "externals/Pml/Sex.h"
#include "externals/Pml/WazaNo.h"

namespace Pml::PokePara {
    struct CoreParam : ILClass<CoreParam> {
        struct FormChangeResult : ILClass<FormChangeResult> {
            struct Fields {
                Pml::WazaNo_array* m_addedWaza;
                Pml::WazaNo_array* m_removedWaza;
                Pml::WazaNo_array* m_addFailedWaza;
            };
        };

        struct Fields {
            void* m_coreData;
            void* m_calcData;
            Pml::PokePara::Accessor::Object* m_accessor;
        };

        inline int32_t GetWazaNo(uint8_t index) {
            return external<int32_t>(game::Address::Pml_PokePara_CoreParam_GetWazaNo, this, index);
        }

        inline void SetWaza(uint32_t index, uint32_t wazaId) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetWaza, this, index, wazaId);
        }

        inline uint32_t GetLangId() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetLangId, this);
        }

        inline bool IsNull() {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_IsNull, this);
        }

        inline bool IsEgg(Pml::PokePara::EggCheckType type) {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_IsEgg, this, type);
        }

        inline int32_t GetSeikaku() {
            return external<int32_t>(game::Address::Pml_PokePara_CoreParam_GetSeikaku, this);
        }

        inline bool IsHpZero() {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_IsHpZero, this);
        }

        inline uint32_t GetLevel() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetLevel, this);
        }

        inline int32_t GetMonsNo() {
            return external<int32_t>(game::Address::Pml_PokePara_CoreParam_GetMonsNo, this);
        }

        inline uint16_t GetFormNo() {
            return external<uint16_t>(game::Address::Pml_PokePara_CoreParam_GetFormNo, this);
        }

        inline uint16_t GetItem() {
            return external<uint16_t>(game::Address::Pml_PokePara_CoreParam_GetItem, this);
        }

        inline uint16_t GetTokuseiIndex() {
            return external<uint8_t>(game::Address::Pml_PokePara_CoreParam_GetTokuseiIndex, this);
        }

        inline void SetTokuseiIndex(uint8_t tokuseiIndex) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetTokuseiIndex, this, tokuseiIndex);
        }

        inline uint16_t GetTokuseiNo() {
            return external<uint16_t>(game::Address::Pml_PokePara_CoreParam_GetTokuseiNo, this);
        }

        inline void SetItem(uint16_t itemno) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetItem, this, itemno);
        }

        inline uint8_t GetMezapaType() {
            return external<uint8_t>(game::Address::Pml_PokePara_CoreParam_GetMezapaType, this);
        }

        inline Sex GetSex() {
            return external<Sex>(game::Address::Pml_PokePara_CoreParam_GetSex, this);
        }

        inline void SetNickName(System::String::Object* nickName) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetNickName, this, nickName);
        }

        inline System::Collections::Generic::HashSet$$WazaNo::Object* CollectRemindableWaza() {
            return external<System::Collections::Generic::HashSet$$WazaNo::Object*>(game::Address::Pml_PokePara_CoreParam_CollectRemindableWaza, this);
        }

        inline int32_t AddWazaIfEmptyExist(int32_t wazano) {
            return external<int32_t>(game::Address::Pml_PokePara_CoreParam_AddWazaIfEmptyExist, this, wazano);
        }

        inline bool CheckWazaMachine(uint32_t machineNo) {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_CheckWazaMachine, this, machineNo);
        }

        inline uint32_t GetPower(int32_t powerId) {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetPower, this, powerId);
        }

        inline bool IsTrainingDone(int32_t powerId) {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_IsTrainingDone, this, powerId);
        }

        inline uint32_t GetTalentPower(int32_t powerId) {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetTalentPower, this, powerId);
        }

        inline uint32_t GetEffortPower(int32_t powerId){
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetEffortPower, this, powerId);
        }

        inline uint32_t GetHp(){
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetHp, this);
        }

        inline uint32_t GetMaxHp() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetMaxHp, this);
        }

        inline void SetGetBall(uint32_t value) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetGetBall, this, value);
        }

        inline void SetRareType(RareType type) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetRareType, this, type);
        }

        inline uint8_t CalcLevel() {
            return external<uint8_t>(game::Address::Pml_PokePara_CoreParam_CalcLevel, this);
        }

        inline uint32_t GetAtk() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetAtk, this);
        }

        inline uint32_t GetDef() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetDef, this);
        }

        inline uint32_t GetPersonalRnd() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetPersonalRnd, this);
        }

        inline uint8_t GetType1() {
            return external<uint8_t>(game::Address::Pml_PokePara_CoreParam_GetType1, this);
        }

        inline uint8_t GetType2() {
            return external<uint8_t>(game::Address::Pml_PokePara_CoreParam_GetType2, this);
        }

        inline void SetFriendship(uint32_t value) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetFriendship, this, value);
        }

        inline void ChangeMonsNo(int32_t newMonsno, uint16_t newFormno) {
            external<void>(game::Address::Pml_PokePara_CoreParam_ChangeMonsNo, this, newMonsno, newFormno);
        }

        inline void ChangeFormNo(uint16_t newFormno, FormChangeResult::Object* pResult) {
            external<void>(game::Address::Pml_PokePara_CoreParam_ChangeFormNo, this, newFormno, pResult);
        }

        inline void RemoveItem() {
            external<void>(game::Address::Pml_PokePara_CoreParam_RemoveItem, this);
        }

        inline RareType GetRareType() {
            return external<RareType>(game::Address::Pml_PokePara_CoreParam_GetRareType, this);
        }

        inline void ChangeEffortPower(Pml::PokePara::PowerID powerId, uint32_t value) {
            external<void>(game::Address::Pml_PokePara_CoreParam_ChangeEffortPower, this, powerId, value);
        }

        inline void ChangeTalentPower(Pml::PokePara::PowerID powerId, uint32_t value) {
            external<void>(game::Address::Pml_PokePara_CoreParam_ChangeTalentPower, this, powerId, value);
        }

        inline void SetParentName(System::String::Object* name) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetParentName, this, name);
        }

        inline void SetLangId(uint32_t langId) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetLangId, this, langId);
        }

        inline void SetCondition(Pml::PokePara::Condition cond, uint8_t value) {
            external<void>(game::Address::Pml_PokePara_CoreParam_SetCondition, this, cond, value);
        }

        inline bool UpdateOwnerInfo(Pml::PokePara::OwnerInfo::Object* ownerInfo) {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_UpdateOwnerInfo, this, ownerInfo);
        }

        inline bool StartFastMode() {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_StartFastMode, this);
        }

        inline bool EndFastMode(bool validFlag) {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_EndFastMode, this, validFlag);
        }

        inline uint32_t GetMultiPurposeWork() {
            return this->instance()->fields.m_accessor->GetMultiPurposeWork();
        }

        inline void SetMultiPurposeWork(uint32_t value) {
            this->instance()->fields.m_accessor->SetMultiPurposeWork(value);
        }

        inline uint32_t GetExp() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetExp, this);
        }

        inline uint32_t GetMemories(int32_t memoriesKind) {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetMemories, this, memoriesKind);
        }

        inline uint32_t GetGetBall() {
            return external<uint32_t>(game::Address::Pml_PokePara_CoreParam_GetGetBall, this);
        }

        inline uint8_t GetWazaCount() {
            return external<uint8_t>(game::Address::Pml_PokePara_CoreParam_GetWazaCount, this);
        }

        inline bool HaveWaza(int32_t wazano) {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_HaveWaza, this, wazano);
        }

        inline void ChangeEgg() {
            external<void>(game::Address::Pml_PokePara_CoreParam_ChangeEgg, this);
        }

        inline bool IsRare() {
            return external<bool>(game::Address::Pml_PokePara_CoreParam_IsRare, this);
        }
    };
}
