#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldPokemonEntity.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/XLSXContent/PokemonInfo.h"

namespace System::Collections::Generic {
    struct List$$UgMainProc_SimbolMons;
}

struct UgMainProc : ILClass<UgMainProc, 0x04c63d10> {
    struct DisplayClass9_0 : ILClass<DisplayClass9_0, 0x04c655d8> {
        struct Fields {
            void* sm; // UgMainProc_SimbolMons_o*
            Pml::PokePara::PokemonParam::Object* param;
            int32_t moveType;
            UgMainProc::Object* __4__this;
        };
    };

    struct SimbolMons : ILClass<SimbolMons> {
        struct Fields {
            Pml::PokePara::PokemonParam::Object* pokeParam;
            XLSXContent::PokemonInfo::SheetCatalog::Object* catalog;
            UnityEngine::Vector3::Object defaultPos;
            bool Active;
            bool isLoaded;
            int32_t moveType;
            UnityEngine::GameObject::Object* gameObject;
            UnityEngine::Transform::Object* transform;
            FieldPokemonEntity::Object* entity;
        };
    };

    struct Fields {
        System::Collections::Generic::List$$UgMainProc_SimbolMons* _ugMons;
        void* ugAIs; // System_Collections_Generic_List_AIUgModel__o*
        float NoHitTime;
    };
};

namespace System::Collections::Generic {
    struct List$$UgMainProc_SimbolMons : List<List$$UgMainProc_SimbolMons, UgMainProc::SimbolMons> {};
}