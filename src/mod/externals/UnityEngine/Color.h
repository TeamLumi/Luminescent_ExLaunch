#pragma once

#include <cmath>

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Color : ILStruct<Color> {
        struct Fields {
            float r;
            float g;
            float b;
            float a;
        };

        Color::Object HueShift(float degrees) {
            auto fields = *(Color::Fields*)this;
            float cmax = fmaxf(std::fmax(fields.r, fields.g), fields.b);
            float cmin = fminf(std::fmin(fields.r, fields.g), fields.b);

            float delta = cmax - cmin;

            float hue = 0;
            if (delta == 0) hue = 0;
            else if (cmax == fields.r) hue = fmodf((fields.g - fields.b) / delta, 6.0f) * 60;
            else if (cmax == fields.g) hue = fields.b - fields.r + 2 * 60;
            else if (cmax == fields.b) hue = fields.r - fields.g + 4 * 60;

            float saturation = 0;
            if (cmax == 0) saturation = 0;
            else saturation = delta / cmax;

            float value = cmax;

            hue += degrees;
            if (hue < 0) hue += 360;
            else if (hue >= 360) hue -= 360;

            Color::Fields newFields = {};
            newFields.a = 1;

            float c = value * saturation;
            float x = c * (1 - fabsf(fmodf((hue / 60), 2) - 1));
            float m = value - c;

            if (0 <= hue && hue < 60) {
                newFields.r = c+m;
                newFields.g = x+m;
                newFields.b = 0+m;
            } else if (60 <= hue && hue < 120) {
                newFields.r = x+m;
                newFields.g = c+m;
                newFields.b = 0+m;
            } else if (120 <= hue && hue < 180) {
                newFields.r = 0+m;
                newFields.g = c+m;
                newFields.b = x+m;
            } else if (180 <= hue && hue < 240) {
                newFields.r = 0+m;
                newFields.g = x+m;
                newFields.b = c+m;
            } else if (240 <= hue && hue < 300) {
                newFields.r = x+m;
                newFields.g = 0+m;
                newFields.b = c+m;
            } else if (300 <= hue && hue < 360) {
                newFields.r = c+m;
                newFields.g = 0+m;
                newFields.b = x+m;
            }

            return *(Color::Object*)&newFields;
        }
    };
}
