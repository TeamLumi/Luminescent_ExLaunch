#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"
#include "externals/System/Action.h"

namespace Dpr::Battle::View {
    struct BattleViewAssetManager : ILClass<BattleViewAssetManager> {

        struct __c__DisplayClass24_0 : ILClass<__c__DisplayClass24_0> {
            struct Fields {
                Dpr::Battle::View::TrainerSimpleParam::Object param;
                System::Action::Object* onComplete;
            };

            static_assert(offsetof(Fields, onComplete) == 0x38);
        };




        struct StaticFields {
            void* _CachedAssetBundle_k__BackingField; // Systems_Collections_Generic_Dictionary_string__Object__o*
            void* _CachedEffectData_k__BackingField; // Systems_Collections_Generic_List_Tuple_string__EffectData__o*
            void* _CachedModelData_k__BackingField; // Systems_Collections_Generic_List_Tuple_string__ObjectEntity__o*
            void* _CachedCameraFilePlayables_k__BackingField; // Systems_Collections_Generic_Dictionary_string__CameraFilePlayable__o*
            void* NeedDestroyObjects; // Systems_Collections_Generic_List_Object__o*
        };

        static_assert(offsetof(StaticFields, NeedDestroyObjects) == 0x20);
    };
}