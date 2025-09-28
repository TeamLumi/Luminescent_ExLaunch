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
            return external<int32_t>(0x02045e00, this, index);
        }

        inline void SetWaza(uint32_t index, uint32_t wazaId) {
            external<void>(0x020464e0, this, index, wazaId);
        }

        inline uint32_t GetLangId() {
            return external<uint32_t>(0x0204b810, this);
        }

        inline bool IsNull() {
            return external<bool>(0x0204c9d0, this);
        }

        inline bool IsEgg(Pml::PokePara::EggCheckType type) {
            return external<bool>(0x02049370, this, type);
        }

        inline int32_t GetSeikaku() {
            return external<int32_t>(0x02048740, this);
        }

        inline bool IsHpZero() {
            return external<bool>(0x02043bd0, this);
        }

        inline uint32_t GetLevel() {
            return external<uint32_t>(0x02043e30, this);
        }

        inline int32_t GetMonsNo() {
            return external<int32_t>(0x02044240, this);
        }

        inline uint16_t GetFormNo() {
            return external<uint16_t>(0x02044250, this);
        }

        inline uint16_t GetItem() {
            return external<uint16_t>(0x02049660, this);
        }

        inline uint16_t GetTokuseiIndex() {
            return external<uint8_t>(0x02045ce0, this);
        }

        inline void SetTokuseiIndex(uint8_t tokuseiIndex) {
            external<void>(0x02048ac0, this, tokuseiIndex);
        }

        inline uint16_t GetTokuseiNo() {
            return external<uint16_t>(0x020488a0, this);
        }

        inline void SetItem(uint16_t itemno) {
            external<void>(0x02049680, this, itemno);
        }

        inline uint8_t GetMezapaType() {
            return external<uint8_t>(0x0204b950, this);
        }

        inline Sex GetSex() {
            return external<Sex>(0x02048540, this);
        }

        inline void SetNickName(System::String::Object* nickName) {
            external<void>(0x02048e90, this, nickName);
        }

        inline System::Collections::Generic::HashSet$$WazaNo::Object* CollectRemindableWaza() {
            return external<System::Collections::Generic::HashSet$$WazaNo::Object*>(0x02047aa0, this);
        }

        inline int32_t AddWazaIfEmptyExist(int32_t wazano) {
            return external<int32_t>(0x02047370, this, wazano);
        }

        inline bool CheckWazaMachine(uint32_t machineNo) {
            return external<bool>(0x02046b40, this, machineNo);
        }

        inline uint32_t GetPower(int32_t powerId) {
            return external<uint32_t>(0x020435c0, this, powerId);
        }

        inline bool IsTrainingDone(int32_t powerId) {
            return external<bool>(0x02044680, this, powerId);
        }

        inline uint32_t GetTalentPower(int32_t powerId) {
            return external<uint32_t>(0x02044610, this, powerId);
        }

        inline uint32_t GetEffortPower(int32_t powerId){
            return external<uint32_t>(0x02044c50, this, powerId);
        }

        inline uint32_t GetHp(){
            return external<uint32_t>(0x02043970, this);
        }

        inline uint32_t GetMaxHp() {
            return external<uint32_t>(0x02043620, this);
        }

        inline void SetGetBall(uint32_t value) {
            external<void>(0x0204b840, this, value);
        }

        inline void SetRareType(RareType type) {
            external<void>(0x0204a920, this, type);
        }

        inline uint8_t CalcLevel() {
            return external<uint8_t>(0x02043e80, this);
        }

        inline uint32_t GetAtk() {
            return external<uint32_t>(0x020436a0, this);
        }

        inline uint32_t GetDef() {
            return external<uint32_t>(0x02043720, this);
        }

        inline uint32_t GetPersonalRnd() {
            return external<uint32_t>(0x0204a690, this);
        }

        inline uint8_t GetType1() {
            return external<uint8_t>(0x0204a9f0, this);
        }

        inline uint8_t GetType2() {
            return external<uint8_t>(0x0204aaf0, this);
        }

        inline void SetFriendship(uint32_t value) {
            external<void>(0x02049070, this, value);
        }

        inline void ChangeMonsNo(int32_t newMonsno, uint16_t newFormno) {
            external<void>(0x02045b40, this, newMonsno, newFormno);
        }

        inline void ChangeFormNo(uint16_t newFormno, FormChangeResult::Object* pResult) {
            external<void>(0x0204a260, this, newFormno, pResult);
        }

        inline void RemoveItem() {
            external<void>(0x02049690, this);
        }

        inline RareType GetRareType() {
            return external<RareType>(0x0204a5a0, this);
        }

        inline void ChangeEffortPower(Pml::PokePara::PowerID powerId, uint32_t value) {
            external<void>(0x02044da0, this, powerId, value);
        }

        inline void SetParentName(System::String::Object* name) {
            external<void>(0x0204ac00, this, name);
        }

        inline void SetLangId(uint32_t langId) {
            external<void>(0x02049650, this, langId);
        }

        inline void SetCondition(Pml::PokePara::Condition cond, uint8_t value) {
            external<void>(0x0204b040, this, cond, value);
        }

        inline bool UpdateOwnerInfo(Pml::PokePara::OwnerInfo::Object* ownerInfo) {
            return external<bool>(0x02048c90, this, ownerInfo);
        }

        inline bool StartFastMode() {
            return external<bool>(0x0204c910, this);
        }

        inline bool EndFastMode(bool validFlag) {
            return external<bool>(0x0204c960, this, validFlag);
        }

        inline uint32_t GetMultiPurposeWork() {
            return this->instance()->fields.m_accessor->GetMultiPurposeWork();
        }

        inline void SetMultiPurposeWork(uint32_t value) {
            this->instance()->fields.m_accessor->SetMultiPurposeWork(value);
        }

        inline uint32_t GetExp() {
            return external<uint32_t>(0x02043f30, this);
        }

        inline uint32_t GetMemories(int32_t memoriesKind) {
            return external<uint32_t>(0x0204ac30, this, memoriesKind);
        }

        inline uint32_t GetGetBall() {
            return external<uint32_t>(0x0204b830, this);
        }

        inline uint8_t GetWazaCount() {
            return external<uint8_t>(0x02045e10, this);
        }

        inline bool HaveWaza(int32_t wazano) {
            return external<bool>(0x02045ea0, this, wazano);
        }

        inline void ChangeEgg() {
            external<void>(0x02049450, this);
        }
    };
}
