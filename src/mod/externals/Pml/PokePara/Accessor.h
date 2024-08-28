#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"
#include "externals/System/String.h"

namespace Pml::PokePara {
    struct Accessor : ILClass<Accessor> {
        struct AccessState : ILStruct<AccessState> {
            bool isEncoded;
            bool isFastMode;
        };

        struct Fields {
            System::Byte_array* m_pCalcData;
            System::Byte_array* m_pCoreData;
            Pml::PokePara::Accessor::AccessState::Object m_accessState;
        };

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
    };
}
