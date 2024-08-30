#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/Sex.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"

namespace Pml::PokePara {
    struct Accessor : ILClass<Accessor, 0x04c64ec8> {
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
            external<void>(0x024a3ed0, this);
        }

        inline void SetLangID(uint8_t langId) {
            external<void>(0x024a8bb0, this, langId);
        }

        inline void SetOwnedOthersFlag(bool flag) {
            external<void>(0x024ab2b0, this, flag);
        }

        inline int32_t GetWazaNo(uint8_t wazaIndex) {
            return external<int32_t>(0x024a5d70, this, wazaIndex);
        }

        inline void SetFormNo(uint16_t formno) {
            external<void>(0x024a9c30, this, formno);
        }
        
        inline void SetGetBall(uint8_t ball) {
            external<void>(0x024aaba0, this, ball);
        }

        inline bool HaveCalcData() {
            return external<bool>(0x024a40f0, this);
        }

        inline uint32_t GetLevel() {
            return external<uint32_t>(0x024a71a0, this);
        }

        inline uint8_t GetOyasex() {
            return external<uint8_t>(0x024a7120, this);
        }

        inline uint32_t GetID() {
            return external<uint32_t>(0x024a4f50, this);
        }

        inline bool CompareOyaName(System::String::Object* cmpName) {
            return external<bool>(0x024a7eb0, this, cmpName);
        }

        inline uint32_t GetFriendship() {
            return external<uint32_t>(0x024a5030, this);
        }

        inline uint8_t GetOthersFriendship() {
            return external<uint8_t>(0x024a7960, this);
        }

        inline uint8_t GetStyle() {
            return external<uint8_t>(0x024a58a0, this);
        }

        inline uint8_t GetBeautiful() {
            return external<uint8_t>(0x024a5910, this);
        }

        inline uint8_t GetCute() {
            return external<uint8_t>(0x024a5980, this);
        }

        inline uint8_t GetClever() {
            return external<uint8_t>(0x024a59f0, this);
        }

        inline uint8_t GetStrong() {
            return external<uint8_t>(0x024a5a60, this);
        }

        inline uint32_t GetItemNo() {
            return external<uint32_t>(0x024a4ee0, this);
        }

        inline bool IsTamago() {
            return external<bool>(0x024a68b0, this);
        }

        inline bool IsFuseiTamago() {
            return external<bool>(0x024a4bc0, this);
        }

        inline int32_t GetMonsNo() {
            return external<int32_t>(0x024a4e70, this);
        }

        inline uint16_t GetFormNo() {
            return external<uint16_t>(0x024a64e0, this);
        }

        inline uint32_t GetMultiPurposeWork() {
            return external<uint32_t>(0x024a7f40, this);
        }

        inline void SetMultiPurposeWork(uint32_t value) {
            external<void>(0x024abac0, this, value);
        }

        inline void AttachDecodedData(System::Byte_array* coreData, System::Byte_array* calcData) {
            external<void>(0x024a3ee0, this, coreData, calcData);
        }

        inline void SetPersonalRnd(uint32_t rnd) {
            external<void>(0x024a85a0, this, rnd);
        }

        inline void SetID(uint32_t id) {
            external<void>(0x024a8950, this, id);
        }

        inline void SetColorRnd(uint32_t rnd) {
            external<void>(0x024a8640, this, rnd);
        }

        inline void SetMonsNo(uint32_t monsno) {
            external<void>(0x024a8870, this, monsno);
        }

        inline void SetLevel(uint8_t level) {
            external<void>(0x024aad10, this, level);
        }

        inline void SetSeikaku(uint32_t seikaku) {
            external<void>(0x024a9ca0, this, seikaku);
        }

        inline void SetSeikakuHosei(uint32_t seikaku) {
            external<void>(0x024a9d10, this, seikaku);
        }

        inline void SetSex(Pml::Sex sex) {
            external<void>(0x024a9b90, this, sex);
        }

        inline void SetGetLevel(uint8_t level) {
            external<void>(0x024aac10, this, level);
        }

        inline void SetCassetteVersion(uint32_t version) {
            external<void>(0x024aa4e0, this, version);
        }

        inline void SetFriendship(uint8_t friendship) {
            external<void>(0x024a8a30, this, friendship);
        }

        inline void SetEquipRibbonNo(uint8_t ribbonNo) {
            external<void>(0x024a9410, this, ribbonNo);
        }

        inline void SetTalentHp(uint8_t value) {
            external<void>(0x024a9700, this, value);
        }

        inline void SetTalentAtk(uint8_t value) {
            external<void>(0x024a9780, this, value);
        }

        inline void SetTalentDef(uint8_t value) {
            external<void>(0x024a9800, this, value);
        }

        inline void SetTalentSpAtk(uint8_t value) {
            external<void>(0x024a9880, this, value);
        }

        inline void SetTalentSpDef(uint8_t value) {
            external<void>(0x024a9900, this, value);
        }

        inline void SetTalentAgi(uint8_t value) {
            external<void>(0x024a9980, this, value);
        }

        inline void SetTokuseiNo(uint32_t tokusei) {
            external<void>(0x024a8ad0, this, tokusei);
        }

        inline void SetTokusei1Flag(bool flag) {
            external<void>(0x024a9e20, this, flag);
        }

        inline void SetTokusei2Flag(bool flag) {
            external<void>(0x024a9ea0, this, flag);
        }

        inline void SetTokusei3Flag(bool flag) {
            external<void>(0x024a9f40, this, flag);
        }

        inline void SetFavoriteFlag(bool flag) {
            external<void>(0x024a9fe0, this, flag);
        }

        inline void SetExp(uint32_t exp) {
            external<void>(0x024a89c0, this, exp);
        }

        inline void SetTalentHeight(uint8_t value) {
            external<void>(0x024ac100, this, value);
        }

        inline void SetTalentWeight(uint8_t value) {
            external<void>(0x024ac170, this, value);
        }

        inline bool IsFastMode() {
            return external<bool>(0x024a4250, this);
        }

        inline void StartFastMode() {
            external<void>(0x024a4260, this);
        }

        inline void SetOyaName(System::String::Object* oyaName) {
            external<void>(0x024aa550, this, oyaName);
        }

        inline void SetTamagoWazaNo(uint8_t index, uint32_t wazano) {
            external<void>(0x024a9660, this, index, wazano);
        }

        inline void SetTamagoFlag(bool flag) {
            external<void>(0x024a9d80, this, flag);
        }

        inline void EndFastMode() {
            external<void>(0x024a42a0, this);
        }
    };
}
