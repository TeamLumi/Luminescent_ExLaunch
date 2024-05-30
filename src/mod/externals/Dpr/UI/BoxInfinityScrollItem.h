#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct BoxInfinityScrollItem : ILClass<BoxInfinityScrollItem> {
        struct BaseParam : ILClass<BaseParam> {
            struct Fields {
                int32_t paramIndex;
                void* itemParams;
                int32_t displayMode;
            };
        };

        struct Fields : UnityEngine::MonoBehaviour::Fields {

        };
    };
}