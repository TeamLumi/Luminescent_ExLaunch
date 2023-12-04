#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UINavigator.h"

namespace Dpr::UI {
    struct BoxMultiNavigator : ILClass<BoxMultiNavigator> {
        struct Fields : UINavigator::Fields {
            void* _lefts; // System_Collections_Generic_List_UINavigator__o*
            void* _rights; // System_Collections_Generic_List_UINavigator__o*
            void* _tops; // System_Collections_Generic_List_UINavigator__o*
            void* _bottoms; // System_Collections_Generic_List_UINavigator__o*
            void* _multiNavigates; // System_Collections_Generic_List_List_UINavigator___o*
            bool isMultiInitialized;
        };
    };
}