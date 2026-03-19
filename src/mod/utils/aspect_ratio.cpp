#include "exlaunch.hpp"
#include "utils/aspect_ratio.h"
#include "data/aspect_ratios.h"
#include "data/settings.h"
#include "data/utils.h"
#include "externals/UnityEngine/Screen.h"
#include "externals/FieldCamera.h"
#include "logger/logger.h"

// Cached target aspect ratio — updated by applyAspectRatioSetting,
// read by FieldCameraLateUpdate hook every frame
static float s_targetAspect = 0.0f;

void applyAspectRatioSetting(int32_t configId, int32_t value) {
    if (configId != array_index(SETTINGS, "Aspect Ratio"))
        return;

    float ratio = aspectRatioEnumToFloat(value);
    s_targetAspect = ratio;

    if (ratio <= 0.0f) {
        // AUTO_16_9: reset to default
        Logger::log("Aspect Ratio: reset to default 16:9\n");
        UnityEngine::Screen::SetResolution(1920, 1080);
    } else {
        int32_t width = (int32_t)(ratio * 1080.0f);
        Logger::log("Aspect Ratio: setting to %dx1080 (ratio=%.4f)\n", width, ratio);
        UnityEngine::Screen::SetResolution(width, 1080);
    }
}

// Hook: FieldCamera.LateUpdate (RVA 0x1783AB0)
// After the game updates the camera each frame, force the aspect ratio
HOOK_DEFINE_TRAMPOLINE(FieldCameraLateUpdate) {
    static void Callback(FieldCamera::Object* thisObj) {
        Orig(thisObj);

        if (s_targetAspect <= 0.0f) return;

        auto* cameraObj = thisObj->fields._camera;
        if (!cameraObj) return;

        cameraObj->set_aspect(s_targetAspect);
    }
};

void installAspectRatioHooks() {
    FieldCameraLateUpdate::InstallAtOffset(0x1783AB0);
    Logger::log("Aspect Ratio: FieldCamera.LateUpdate hook installed\n");
}
