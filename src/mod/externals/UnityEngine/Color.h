#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"

namespace UnityEngine {
    struct Color : ILStruct<Color> {
        struct Fields {
            float r;
            float g;
            float b;
            float a;
        };

        /****************************
         * Everything below is custom code for HSV <-> RGB math
         ****************************/

        /*
         * Range is 0-360
         */
        float CalculateHue() {
            auto fields = *(Color::Fields*)this;
            float cmax = std::fmax(std::fmax(fields.r, fields.g), fields.b);
            float cmin = std::fmin(std::fmin(fields.r, fields.g), fields.b);

            float delta = cmax - cmin;

            float hue = 0;
            if (delta == 0) hue = 0;
            else if (cmax == fields.r) hue = std::fmod((fields.g - fields.b) / delta, 6.0f) * 60;
            else if (cmax == fields.g) hue =             ((fields.b - fields.r) / delta + 2)      * 60;
            else if (cmax == fields.b) hue =             ((fields.r - fields.g) / delta + 4)      * 60;

            return hue;
        }

        /*
         * Range is 0-1
         */
        float CalculateSaturation() {
            auto fields = *(Color::Fields*)this;
            float cmax = std::fmax(std::fmax(fields.r, fields.g), fields.b);
            float cmin = std::fmin(std::fmin(fields.r, fields.g), fields.b);

            float delta = cmax - cmin;

            return cmax == 0.0f ? 0.0f : (delta / cmax);
        }

        /*
         * Range is 0-1
         */
        float CalculateValue() {
            auto fields = *(Color::Fields*)this;
            return std::fmax(std::fmax(fields.r, fields.g), fields.b);
        }

        Color::Object static CreateFromHSV(float hue, float saturation, float value, float a) {
            Color::Fields newFields = {};
            newFields.a = a;

            Logger::log("[Color] Creating color from HSVA %f %f %f %f\n", hue, saturation, value, a);

            float chroma = value * saturation;
            float cmax = value;
            float cmin = cmax - chroma;

            float hueprime;
            if (hue < 300.0f)
                hueprime = hue / 60.0f;
            else
                hueprime = (hue - 360.0f) / 60.0f;

            if (-1.0f <= hueprime && hueprime < 0.0f) {
                newFields.r = cmax;
                newFields.g = cmin;
                newFields.b = cmin - hueprime * chroma;
            } else if (0.0f <= hueprime && hueprime < 1.0f) {
                newFields.r = cmax;
                newFields.g = cmin + hueprime * chroma;
                newFields.b = cmin;
            } else if (1.0f <= hueprime && hueprime < 2.0f) {
                newFields.r = cmin - (hueprime - 2.0f) * chroma;
                newFields.g = cmax;
                newFields.b = cmin;
            } else if (2.0f <= hueprime && hueprime < 3.0f) {
                newFields.r = cmin;
                newFields.g = cmax;
                newFields.b = cmin + (hueprime - 2.0f) * chroma;
            } else if (3.0f <= hueprime && hueprime < 4.0f) {
                newFields.r = cmin;
                newFields.g = cmin - (hueprime - 4.0f) * chroma;
                newFields.b = cmax;
            } else if (4.0f <= hueprime && hueprime < 5.0f) {
                newFields.r = cmin + (hueprime - 4.0f) * chroma;
                newFields.g = cmin;
                newFields.b = cmax;
            }

            if (newFields.r < 0.0f) newFields.r = 0.0f;
            else if (newFields.r > 1.0f) newFields.r = 1.0f;

            if (newFields.g < 0.0f) newFields.g = 0.0f;
            else if (newFields.g > 1.0f) newFields.g = 1.0f;

            if (newFields.b < 0.0f) newFields.b = 0.0f;
            else if (newFields.b > 1.0f) newFields.b = 1.0f;

            return *(Color::Object*)&newFields;
        }

        Color::Object static CreateFromHSV(float hue, float saturation, float value) {
            return CreateFromHSV(hue, saturation, value, 1.0f);
        }

        Color::Object static CreateFromRGB(int32_t r, int32_t g, int32_t b, int32_t a) {
            Color::Fields newFields = {};

            Logger::log("[Color] Creating color from RGBA %d %d %d %d\n", r, g, b, a);

            newFields.r = (float)r / 255.0f;
            newFields.g = (float)g / 255.0f;
            newFields.b = (float)b / 255.0f;
            newFields.a = (float)a / 255.0f;

            return *(Color::Object*)&newFields;
        }

        Color::Object static CreateFromRGB(int32_t r, int32_t g, int32_t b) {
            return CreateFromRGB(r, g, b, 255);
        }

        Color::Object static CreateFromRGB(int32_t rgb) {
            return CreateFromRGB((rgb & 0x00FF0000) >> 16,
                                 (rgb & 0x0000FF00) >> 8,
                                 (rgb & 0x000000FF),
                                 255);
        }

        int32_t ToHexCode() {
            auto fields = *(Color::Fields*)this;
            return ((int32_t)(fields.r * 255.0f) << 16) +
                   ((int32_t)(fields.g * 255.0f) << 8) +
                   (int32_t)(fields.b * 255.0f);
        }

        Color::Object HSVShift(float hueShift, float saturationShift, float valueShift) {
            auto fields = *(Color::Fields*)this;

            float hue = CalculateHue();
            float saturation = CalculateSaturation();
            float value = CalculateValue();

            Logger::log("[Color] Shifting HSV starting with %f %f %f, and shifting by %f, %f, %f\n", hue, saturation, value, hueShift, saturationShift, valueShift);

            hue += hueShift;

            if (hue >= 360.0f)
                hue = std::fmod(hue, 360.0f);
            else if (hue < 0.0f)
                hue = std::fmod(hue, 360.0f) + 360.0f;

            saturation += saturationShift;
            saturation = std::fmin(saturation, 100.0f);
            saturation = std::fmax(saturation, 0.0f);

            value += valueShift;
            value = std::fmin(value, 100.0f);
            value = std::fmax(value, 0.0f);

            return CreateFromHSV(hue, saturation, value, fields.a);
        }

        [[nodiscard]] nn::json ToJson() const {
            auto fields = *(Color::Fields*)this;

            return {
                {"r", fields.r},
                {"g", fields.g},
                {"b", fields.b},
                {"a", fields.a},
            };
        }

        static Color::Object FromJson(const nn::json& color) {
            Color::Fields newFields = {};
            newFields = {
                .r = color["r"].get<float>(),
                .g = color["g"].get<float>(),
                .b = color["b"].get<float>(),
                .a = color["a"].get<float>()
            };

            return *(Color::Object*)&newFields;
        }
    };
}
