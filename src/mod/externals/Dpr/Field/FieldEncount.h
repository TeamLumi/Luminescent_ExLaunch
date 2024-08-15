#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Field/EncountResult.h"
#include "externals/Dpr/Field/FieldEncount.h"
#include "externals/MonsLv.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Primitives.h"
#include "externals/XLSXContent/FieldEncountTable.h"

namespace Dpr::Field {
    struct FieldEncount : ILClass<FieldEncount> {
        struct ENC_FLD_SPA : ILStruct<ENC_FLD_SPA> {
            struct Fields {
                int32_t TrainerID;
                bool SprayCheck;
                bool EncCancelSpInvalid;
                int32_t SpMyLv;
                bool Egg;
                int32_t Spa;
                System::Int32_array* FormProb;
                int32_t AnnoonTblType;
            };
        };

        struct SWAY_ENC_INFO : ILStruct<SWAY_ENC_INFO> {
            struct Fields {
                int32_t Table;
                bool Decide;
                bool Enc;
            };
        };

        static inline void LastProc(Dpr::Field::EncountResult::Object **result, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spa) {
            external<void>(0x019b4990, result, spa);
        }
        
        static inline int32_t CheckMovePokeEnc() {
            return external<int32_t>(0x019b4500);
        }
        
        static inline void EncountParamSet(int32_t poke, int32_t lv, int32_t inTarget, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object inFldSpa, Pml::PokePara::PokemonParam::Object *inPokeParam, Dpr::Field::EncountResult::Object **outBattleParam) {
            external<void>(0x019b47f0, poke, lv, inTarget, inFldSpa, inPokeParam, outBattleParam);
        }

        static inline bool SetEncountData(Pml::PokePara::PokemonParam::Object *param, int32_t inRodType, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object inFldSpa, MonsLv::Array *inData, int32_t location, int32_t inTarget, Dpr::Field::EncountResult::Object **outBattleParam) {
            return external<bool>(0x019b6e30, param, inRodType, inFldSpa, inData, location, inTarget, outBattleParam);
        }
        
        static inline void SetSpaStruct(Pml::PokePara::PokemonParam::Object *inPokeParam, XLSXContent::FieldEncountTable::Sheettable::Object *inData, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *outSpa) {
            external<void>(0x019b3eb0, inPokeParam, inData, outSpa);
        }

        static inline bool MapEncountCheck(uint32_t per, int32_t attr, bool inGridmove) {
            return external<bool>(0x019b4100, per, attr, inGridmove);
        }
        
        static inline bool WildEncSingle(Pml::PokePara::PokemonParam::Object *poke_param, Dpr::Field::EncountResult::Object **param, XLSXContent::FieldEncountTable::Sheettable::Object *data, MonsLv::Array *enc_data, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object inFldSpa, Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object inSwayEncInfo) {
            return external<bool>(0x019b4c00, poke_param, param, data, enc_data, inFldSpa, inSwayEncInfo);
        }

        static inline bool WildEncDouble(Pml::PokePara::PokemonParam::Object *poke_param, Dpr::Field::EncountResult::Object **param, MonsLv::Array *enc_data, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object* inFldSpa) {
            return external<bool>(0x019b4e80, poke_param, param, enc_data, inFldSpa);
        }
    };
}
