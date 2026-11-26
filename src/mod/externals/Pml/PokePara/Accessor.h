#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/Sex.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"

namespace Pml::PokePara {
    struct Accessor : ILClass<Accessor, game::Address::Pml_PokePara_Accessor_TypeInfo> {
        struct AccessState : ILStruct<AccessState> {
            bool isEncoded;
            bool isFastMode;
        };

        struct Fields {
            System::Byte_array* m_pCalcData;
            System::Byte_array* m_pCoreData;
            Pml::PokePara::Accessor::AccessState::Object m_accessState;
        };

        inline void ctor() {
            external<void>(game::Address::Pml_PokePara_Accessor_ctor, this);
        }

        inline void SetLangID(uint8_t langId) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetLangId, this, langId);
        }

        inline void SetOwnedOthersFlag(bool flag) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetOwnedOthersFlag, this, flag);
        }

        inline int32_t GetWazaNo(uint8_t wazaIndex) {
            return external<int32_t>(game::Address::Pml_PokePara_Accessor_GetWazaNo, this, wazaIndex);
        }

        inline void SetFormNo(uint16_t formno) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetFormNo, this, formno);
        }
        
        inline void SetGetBall(uint8_t ball) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetGetBall, this, ball);
        }

        inline bool HaveCalcData() {
            return external<bool>(game::Address::Pml_PokePara_Accessor_HaveCalcData, this);
        }

        inline uint32_t GetLevel() {
            return external<uint32_t>(game::Address::Pml_PokePara_Accessor_GetLevel, this);
        }

        inline uint8_t GetOyasex() {
            return external<uint8_t>(game::Address::Pml_PokePara_Accessor_GetOyasex, this);
        }

        inline uint32_t GetID() {
            return external<uint32_t>(game::Address::Pml_PokePara_Accessor_GetID, this);
        }

        inline bool CompareOyaName(System::String::Object* cmpName) {
            return external<bool>(game::Address::Pml_PokePara_Accessor_CompareOyaName, this, cmpName);
        }

        inline uint32_t GetFriendship() {
            return external<uint32_t>(game::Address::Pml_PokePara_Accessor_GetFriendship, this);
        }

        inline uint8_t GetOthersFriendship() {
            return external<uint8_t>(game::Address::Pml_PokePara_Accessor_GetOthersFriendship, this);
        }

        inline uint8_t GetStyle() {
            return external<uint8_t>(game::Address::Pml_PokePara_Accessor_GetStyle, this);
        }

        inline uint8_t GetBeautiful() {
            return external<uint8_t>(game::Address::Pml_PokePara_Accessor_GetBeautiful, this);
        }

        inline uint8_t GetCute() {
            return external<uint8_t>(game::Address::Pml_PokePara_Accessor_GetCute, this);
        }

        inline uint8_t GetClever() {
            return external<uint8_t>(game::Address::Pml_PokePara_Accessor_GetClever, this);
        }

        inline uint8_t GetStrong() {
            return external<uint8_t>(game::Address::Pml_PokePara_Accessor_GetStrong, this);
        }

        inline uint32_t GetItemNo() {
            return external<uint32_t>(game::Address::Pml_PokePara_Accessor_GetItemNo, this);
        }

        inline bool IsTamago() {
            return external<bool>(game::Address::Pml_PokePara_Accessor_IsTamago, this);
        }

        inline bool IsFuseiTamago() {
            return external<bool>(game::Address::Pml_PokePara_Accessor_IsFuseiTamago, this);
        }

        inline int32_t GetMonsNo() {
            return external<int32_t>(game::Address::Pml_PokePara_Accessor_GetMonsNo, this);
        }

        inline uint16_t GetFormNo() {
            return external<uint16_t>(game::Address::Pml_PokePara_Accessor_GetFormNo, this);
        }

        inline uint32_t GetMultiPurposeWork() {
            return external<uint32_t>(game::Address::Pml_PokePara_Accessor_GetMultiPurposeWork, this);
        }

        inline void SetMultiPurposeWork(uint32_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetMultiPurposeWork, this, value);
        }

        inline void AttachDecodedData(System::Byte_array* coreData, System::Byte_array* calcData) {
            external<void>(game::Address::Pml_PokePara_Accessor_AttachDecodedData, this, coreData, calcData);
        }

        inline void SetPersonalRnd(uint32_t rnd) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetPersonalRnd, this, rnd);
        }

        inline void SetID(uint32_t id) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetID, this, id);
        }

        inline void SetColorRnd(uint32_t rnd) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetColorRnd, this, rnd);
        }

        inline void SetMonsNo(uint32_t monsno) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetMonsNo, this, monsno);
        }

        inline void SetLevel(uint8_t level) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetLevel, this, level);
        }

        inline void SetSeikaku(uint32_t seikaku) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetSeikaku, this, seikaku);
        }

        inline void SetSeikakuHosei(uint32_t seikaku) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetSeikakuHosei, this, seikaku);
        }

        inline void SetSex(Pml::Sex sex) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetSex, this, sex);
        }

        inline void SetGetLevel(uint8_t level) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetGetLevel, this, level);
        }

        inline void SetCassetteVersion(uint32_t version) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetCassetteVersion, this, version);
        }

        inline void SetFriendship(uint8_t friendship) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetFriendship, this, friendship);
        }

        inline void SetEquipRibbonNo(uint8_t ribbonNo) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetEquipRibbonNo, this, ribbonNo);
        }

        inline void SetTalentHp(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentHp, this, value);
        }

        inline void SetTalentAtk(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentAtk, this, value);
        }

        inline void SetTalentDef(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentDef, this, value);
        }

        inline void SetTalentSpAtk(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentSpAtk, this, value);
        }

        inline void SetTalentSpDef(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentSpDef, this, value);
        }

        inline void SetTalentAgi(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentAgi, this, value);
        }

        inline void SetTokuseiNo(uint32_t tokusei) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTokuseiNo, this, tokusei);
        }

        inline void SetTokusei1Flag(bool flag) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTokusei1Flag, this, flag);
        }

        inline void SetTokusei2Flag(bool flag) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTokusei2Flag, this, flag);
        }

        inline void SetTokusei3Flag(bool flag) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTokusei3Flag, this, flag);
        }

        inline void SetFavoriteFlag(bool flag) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetFavoriteFlag, this, flag);
        }

        inline void SetExp(uint32_t exp) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetExp, this, exp);
        }

        inline void SetTalentHeight(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentHeight, this, value);
        }

        inline void SetTalentWeight(uint8_t value) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTalentWeight, this, value);
        }

        inline bool IsFastMode() {
            return external<bool>(game::Address::Pml_PokePara_Accessor_IsFastMode, this);
        }

        inline void StartFastMode() {
            external<void>(game::Address::Pml_PokePara_Accessor_StartFastMode, this);
        }

        inline void SetOyaName(System::String::Object* oyaName) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetOyaName, this, oyaName);
        }

        inline void SetTamagoWazaNo(uint8_t index, uint32_t wazano) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTamagoWazaNo, this, index, wazano);
        }

        inline void SetTamagoFlag(bool flag) {
            external<void>(game::Address::Pml_PokePara_Accessor_SetTamagoFlag, this, flag);
        }

        inline void EndFastMode() {
            external<void>(game::Address::Pml_PokePara_Accessor_EndFastMode, this);
        }

        inline uint32_t GetGetPlace() {
            return external<uint32_t>(game::Address::Pml_PokePara_Accessor_GetGetPlace, this);
        }

        inline uint32_t GetBirthPlace() {
            return external<uint32_t>(game::Address::Pml_PokePara_Accessor_GetBirthPlace, this);
        }
    };
}
