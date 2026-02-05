#include "exlaunch.hpp"
#include "logger/logger.h"
#include "features/aspect_ratio.h"
#include "externals/il2cpp.h"
#include "externals/FieldCamera.h"
#include "oe.h"
#include <cmath>
#include <cstring>

static float s_targetAspect = 0.0f;

void setTargetAspectRatio(float ratio) {
    s_targetAspect = ratio;
}

float getTargetAspectRatio() {
    return s_targetAspect;
}

// ---- Camera.set_aspect resolved at runtime ----

typedef void (*CameraSetAspectFn)(void*, float, const void*);
static CameraSetAspectFn s_cameraSetAspect = nullptr;
static bool s_cameraMethodsResolved = false;

static void resolveCameraMethods(void* cameraObj) {
    if (s_cameraMethodsResolved) return;
    s_cameraMethodsResolved = true;

    auto* klass = reinterpret_cast<Il2CppClass*>(*(void**)cameraObj);
    auto** methods = reinterpret_cast<const MethodInfo**>(klass->_1.methods);
    uint16_t count = klass->_2.method_count;

    for (uint16_t i = 0; i < count; i++) {
        if (!methods[i] || !methods[i]->name) continue;
        if (strcmp(methods[i]->name, "set_aspect") == 0 && methods[i]->parameters_count == 1) {
            s_cameraSetAspect = reinterpret_cast<CameraSetAspectFn>(methods[i]->methodPointer);
            break;
        }
    }

    Logger::log("Aspect Ratio: Camera.set_aspect = %p\n", (void*)s_cameraSetAspect);
}

// ---- FMOV encoding helpers ----

static float decodeFmovImm8(uint8_t imm8) {
    uint32_t a = (imm8 >> 7) & 1;
    uint32_t b = (imm8 >> 6) & 1;
    uint32_t cd = (imm8 >> 4) & 3;
    uint32_t efgh = imm8 & 0xF;

    int exp = (int)(((b ^ 1) << 2) | cd) - 3;
    float val = (1.0f + efgh / 16.0f) * powf(2.0f, (float)exp);
    if (a) val = -val;
    return val;
}

static uint8_t findClosestFmovImm8(float target) {
    float bestDiff = 1e9f;
    uint8_t bestImm = 0;

    for (int i = 0; i < 256; i++) {
        float val = decodeFmovImm8((uint8_t)i);
        if (val <= 0.0f) continue;
        float diff = fabsf(val - target);
        if (diff < bestDiff) {
            bestDiff = diff;
            bestImm = (uint8_t)i;
        }
    }
    return bestImm;
}

static uint32_t encodeFmovSdImm(uint8_t imm8, uint8_t rd) {
    return 0x1E201000 | ((uint32_t)imm8 << 13) | rd;
}

static void applyFmovPatches(float aspect) {
    uint8_t imm8 = findClosestFmovImm8(aspect);
    Logger::log("Aspect Ratio: FMOV patches target=%.4f encoded=%.4f\n",
                aspect, decodeFmovImm8(imm8));

    exl::patch::CodePatcher p1(0x0050750c);
    p1.WriteInst(encodeFmovSdImm(imm8, 1));

    exl::patch::CodePatcher p2(0x005119cc);
    p2.WriteInst(encodeFmovSdImm(imm8, 0));
}

// ---- FieldCamera.LateUpdate hook (RVA 0x1783AB0) ----

HOOK_DEFINE_TRAMPOLINE(FieldCameraLateUpdate) {
    static void Callback(FieldCamera::Object* thisObj) {
        Orig(thisObj);

        if (s_targetAspect <= 0.0f) return;

        auto* cameraObj = thisObj->fields._camera;
        if (!cameraObj) return;

        resolveCameraMethods(cameraObj);

        if (s_cameraSetAspect) {
            s_cameraSetAspect(cameraObj, s_targetAspect, nullptr);
        }
    }
};

// ---- Public API ----

void reapplyAspectRatioPatches() {
    if (s_targetAspect > 0.0f) {
        applyFmovPatches(s_targetAspect);
    } else {
        applyFmovPatches(16.0f / 9.0f);
    }
}

void patchAspectRatioFromViewport(int width, int height) {
    if (height <= 0) return;

    float aspect = s_targetAspect;
    if (aspect <= 0.0f) {
        aspect = (float)width / (float)height;
        s_targetAspect = aspect;
        Logger::log("Aspect Ratio: set from viewport = %.4f (%dx%d)\n",
                    aspect, width, height);
    }

    static bool fmovPatched = false;
    if (!fmovPatched) {
        fmovPatched = true;
        applyFmovPatches(aspect);
    }
}

static float detectDisplayAspect() {
    u32 width = 0, height = 0;
    nn::oe::GetDefaultDisplayResolution(&width, &height);
    Logger::log("Aspect Ratio: display resolution = %ux%u\n", width, height);

    if (width == 0 || height == 0) return 0.0f;

    float aspect = (float)width / (float)height;
    if (fabsf(aspect - (16.0f / 9.0f)) < 0.01f) return 0.0f;

    return aspect;
}

void exl_aspect_ratio_main() {
    if (s_targetAspect <= 0.0f) {
        float detected = detectDisplayAspect();
        if (detected > 0.0f) {
            s_targetAspect = detected;
            Logger::log("Aspect Ratio: auto-detected %.4f\n", detected);
        }
    }

    Logger::log("Aspect Ratio: target=%.4f, installing hooks\n", s_targetAspect);
    FieldCameraLateUpdate::InstallAtOffset(0x1783AB0);
}
