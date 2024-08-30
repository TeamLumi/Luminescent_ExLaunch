#pragma once

#include "externals/Pml/Personal/EvolveCond.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/WazaNo.h"
#include "externals/XLSXContent/GrowTable.h"
#include "externals/XLSXContent/EvolveTable.h"
#include "externals/XLSXContent/PersonalTable.h"
#include "externals/il2cpp-api.h"

namespace Pml::Personal {
    struct PersonalSystem : ILClass<PersonalSystem, 0x04c5ad18> {
        struct Fields {};

        struct StaticFields {
            XLSXContent::PersonalTable::Object* m_alldata;
            void* m_allGrowData; // XLSXContent_GrowTable_o*
            XLSXContent::EvolveTable::Object* m_allEvolveData;
            void* m_allWazaOboeData; // XLSXContent::WazaOboeTable::Object*
            XLSXContent::PersonalTable::SheetPersonal::Object* s_PersonalData;
            void* s_GrowTable; // XLSXContent::GrowTable::SheetData::Object*
            XLSXContent::EvolveTable::SheetEvolve::Object* s_EvolutionTable;
            void* s_WazaOboeData; // XLSXContent::WazaOboeTable::SheetWazaOboe::Object*
            Pml::WazaNo_array* OSHIE_WAZA;
        };

        static inline XLSXContent::PersonalTable::SheetPersonal::Object* GetPersonalData(int32_t monsno, uint16_t formno) {
            return external<XLSXContent::PersonalTable::SheetPersonal::Object*>(0x024a0ca0, monsno, formno);
        }

        static inline void LoadPersonalData(int32_t monsno, uint16_t formno) {
            external<void>(0x024a0d50, monsno, formno);
        }

        static inline int32_t GetTokuseiPattern() {
            return external<int32_t>(0x024a15d0);
        }

        static inline void LoadEvolutionTable(int32_t monsno, uint16_t formno) {
            external<void>(0x024a2240, monsno, formno);
        }

        static inline Pml::Personal::EvolveCond GetEvolutionCondition(uint8_t route_index) {
            return external<Pml::Personal::EvolveCond>(0x024a2500, route_index);
        }

        static inline int32_t GetEvolvedMonsNo(uint8_t route_index) {
            return external<int32_t>(0x024a26d0, route_index);
        }

        static inline bool IsEvolvedFormNoSpecified(uint8_t route_index) {
            return external<bool>(0x024a2830, route_index);
        }

        static inline uint16_t GetEvolvedFormNo(uint8_t route_index) {
            return external<uint16_t>(0x024a2780, route_index);
        }

        static inline uint16_t GetEvolutionParam(uint8_t route_index) {
            return external<uint16_t>(0x024a2620, route_index);
        }

        static inline XLSXContent::GrowTable::SheetData::Object* GetGrowTable(int32_t monsno, uint16_t formno) {
            return external<XLSXContent::GrowTable::SheetData::Object*>(0x024a1790, monsno, formno);
        }

        static inline System::String::Object* GetMonsName(int32_t monsNo, int32_t langId) {
            return external<System::String::Object*>(0x024a0a90, monsNo, langId);
        }
    };
}