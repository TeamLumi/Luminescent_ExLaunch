#pragma once
#include "externals/il2cpp-api.h"

struct Layer : ILClass<Layer, 0x04c59fa0> {
    struct StaticFields {
        int32_t Nothing;
        int32_t Everything;
        int32_t DefaultLayer;
        int32_t JumpLayer;
        int32_t GroundLayer;
        int32_t Ground;
        int32_t ObstacleLayer;
        int32_t Obstacle;
        int32_t Event;
        int32_t Jump;
        int32_t Attribute;
        int32_t CharacterLayer;
        int32_t Character;
        int32_t FieldLayer;
        int32_t Field;
        int32_t EffectLayer;
        int32_t Effect;
        int32_t NoRendererLayer;
        int32_t NoRenderer;
        int32_t UI;
        int32_t UIModelLayer;
    };

    static_assert(offsetof(StaticFields, UIModelLayer) == 0x50);
};