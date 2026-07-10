#include "ui.h"
#include "helpers.h"
#include "imgui_backend/imgui_impl_nvn.hpp"
#include "nvn/nvn.h"
#include "nvn/nvn_Cpp.h"
#include "exlaunch.hpp"
#include "nvn/nvn_CppFuncPtrImpl.h"
#include "checks.hpp"
#include "logger/logger.h"
#include "ui/base/root_element.h"
#include "memory/nn_allocator.h"

nvn::Device *nvnDevice;
nvn::Queue *nvnQueue;
nvn::CommandBuffer *nvnCmdBuf;

nvn::DeviceGetProcAddressFunc tempGetProcAddressFuncPtr;

nvn::CommandBufferInitializeFunc tempBufferInitFuncPtr;
nvn::DeviceInitializeFunc tempDeviceInitFuncPtr;
nvn::QueueInitializeFunc tempQueueInitFuncPtr;
nvn::QueuePresentTextureFunc tempPresentTexFunc;
nvn::TextureGetWidthFunc tempTexGetWidthFunc;
nvn::TextureGetHeightFunc tempTexGetHeightFunc;
nvn::WindowBuilderSetTexturesFunc tempWindowBuilderSetTexFunc;
nvn::WindowSetCropFunc tempWindowSetCropFunc;
nvn::CommandBufferSetTexturePoolFunc tempCommandSetTexturePoolFunc;
nvn::CommandBufferSetSamplerPoolFunc tempCommandSetSamplerPoolFunc;

bool hasInitImGui = false;

// Overlay system — volatile because procDraw runs on NVN render thread,
// while ui_showOverlay/ui_hideOverlay are called from game logic thread.
static volatile bool s_overlayActive = false;
static char s_overlayText[128] = {};

void ui_showOverlay(const char* text) {
    strncpy(s_overlayText, text, sizeof(s_overlayText) - 1);
    s_overlayText[sizeof(s_overlayText) - 1] = '\0';
    __sync_synchronize(); // memory barrier: ensure text is written before flag
    s_overlayActive = true;
    Logger::log("[Overlay] showOverlay: text='%s'\n", s_overlayText);
}

void ui_hideOverlay() {
    s_overlayActive = false;
    __sync_synchronize();
}

static ui::Root root = ui::Root::single([](ui::Root& it){});

void procDraw() {
    // Heartbeat: log every ~5 seconds at 60fps to confirm procDraw is running
    static int s_procDrawCount = 0;
    s_procDrawCount++;
    if (s_procDrawCount % 300 == 1) {
        Logger::log("[Overlay] procDraw heartbeat #%d active=%d text[0]=%d\n",
                    s_procDrawCount / 300, (int)s_overlayActive, (int)(uint8_t)s_overlayText[0]);
    }

    root.render();

    // Render overlay in its own standalone ImGui frame, after root finishes.
    // This runs on the NVN render thread, so we use volatile + barrier above.
    if (s_overlayActive && s_overlayText[0]) {
        ImguiNvnBackend::newFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        float windowWidth = 420.0f;
        ImGui::SetNextWindowPos(
            ImVec2((io.DisplaySize.x - windowWidth) * 0.5f, io.DisplaySize.y - 120.0f));
        ImGui::SetNextWindowSize(ImVec2(windowWidth, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 12));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.78f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

        ImGui::Begin("##Overlay", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing);

        float textWidth = ImGui::CalcTextSize(s_overlayText).x;
        float availWidth = ImGui::GetContentRegionAvail().x;
        if (textWidth < availWidth) {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (availWidth - textWidth) * 0.5f);
        }
        ImGui::TextWrapped("%s", s_overlayText);

        ImGui::End();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);

        ImGui::Render();
        ImguiNvnBackend::renderDrawData(ImGui::GetDrawData());
    }
}

ui::Root* getRootElement() {
    return &root;
}

bool InitImGui() {
    if (nvnDevice && nvnQueue && nvnCmdBuf) {
        Logger::log("Creating ImGui.\n");
        IMGUI_CHECKVERSION();

        ImGuiMemAllocFunc allocFunc = [](size_t size, void *user_data) {
            return nn_malloc(size);
        };

        ImGuiMemFreeFunc freeFunc = [](void *ptr, void *user_data) {
            nn_free(ptr);
        };

        Logger::log("Setting Allocator Functions.\n");
        ImGui::SetAllocatorFunctions(allocFunc, freeFunc, nullptr);
        Logger::log("Creating ImGui Context.\n");

        ImGui::CreateContext();
        Logger::log("Created ImGui Context.\n");
        ImGuiIO &io = ImGui::GetIO();
        (void) io;

        Logger::log("Setting Style to Dark.\n");
        ImGui::StyleColorsDark();

        ImguiNvnBackend::NvnBackendInitInfo initInfo = {
                .device = nvnDevice,
                .queue = nvnQueue,
                .cmdBuf = nvnCmdBuf,
        };

//        nvn::pfncpp_nvnCommandBufferInitialize(initInfo.cmdBuf, nvnDevice);
//        nvn::pfncpp_nvnCommandBufferAddControlMemory(initInfo.cmdBuf, IM_ALLOC(0x4000), 0x4000);
//
//        auto pool = IM_NEW(nvn::MemoryPool);
//        nvn::MemoryPoolBuilder memPoolBuilder{};
//        memPoolBuilder.SetDefaults()
//            .SetDevice(nvnDevice)
//            .SetFlags(nvn::MemoryPoolFlags::CPU_UNCACHED | nvn::MemoryPoolFlags::GPU_CACHED)
//            .SetStorage(IM_ALLOC(0x20000), 0x20000);
//        pool->Initialize(&memPoolBuilder);
//
//        auto offset = pool->GetBufferAddress();
//        nvn::pfncpp_nvnCommandBufferAddCommandMemory(initInfo.cmdBuf, pool, offset, 0x20000);

        Logger::log("Initializing Backend.\n");
        ImguiNvnBackend::InitBackend(initInfo);
        InputHelper::initKBM();
        InputHelper::setPort(0); // set input helpers default port to zero

        return true;

    } else {
        Logger::log("Unable to create ImGui Renderer!\n");

        return false;
    }
}

static nvn::CommandBuffer* __cmdBuf = nullptr;
static const nvn::TexturePool* __texturePool = nullptr;
static const nvn::SamplerPool* __samplerPool = nullptr;

void setTexturePool(nvn::CommandBuffer* cmdBuf, const nvn::TexturePool* pool) {
    __cmdBuf = cmdBuf;
    __texturePool = pool;

    tempCommandSetTexturePoolFunc(cmdBuf, pool);
}

void setSamplerPool(nvn::CommandBuffer* cmdBuf, const nvn::SamplerPool* pool) {
    __samplerPool = pool;

    tempCommandSetSamplerPoolFunc(cmdBuf, pool);
}

void presentTexture(nvn::Queue *queue, nvn::Window *window, int texIndex) {
    auto* buf = __cmdBuf;
    auto* pool = __texturePool;
    auto* samplerPool = __samplerPool;

    if (hasInitImGui) {
        procDraw();
    }

    buf->BeginRecording();
    setTexturePool(buf, pool);
    setSamplerPool(buf, samplerPool);
    auto handle = buf->EndRecording();
    queue->SubmitCommands(1, &handle);
    tempPresentTexFunc(queue, window, texIndex);
}

void windowBuilderSetTextures(nvn::WindowBuilder *builder, int count, nvn::Texture * const*textures) {
    tempWindowBuilderSetTexFunc(builder, count, textures);

    if (hasInitImGui) {
        int h = tempTexGetHeightFunc(textures[0]);
        int w = tempTexGetWidthFunc(textures[0]);

        ImguiNvnBackend::getBackendData()->viewportSize = ImVec2(w, h);
    }
}

void setCrop(nvn::Window *window, int x, int y, int w, int h) {
    tempWindowSetCropFunc(window, x, y, w, h);

    if (hasInitImGui) {
        ImguiNvnBackend::getBackendData()->viewportSize = ImVec2(w, h);
    }
}

NVNboolean deviceInit(nvn::Device *device, const nvn::DeviceBuilder *builder) {
    NVNboolean result = tempDeviceInitFuncPtr(device, builder);
    nvnDevice = device;
    nvn::nvnLoadCPPProcs(nvnDevice, tempGetProcAddressFuncPtr);
    return result;
}

NVNboolean queueInit(nvn::Queue *queue, const nvn::QueueBuilder *builder) {
    NVNboolean result = tempQueueInitFuncPtr(queue, builder);

    if (nvnQueue == nullptr) {
        nvnQueue = queue;
    }
    return result;
}

NVNboolean cmdBufInit(nvn::CommandBuffer *buffer, nvn::Device *device) {
    NVNboolean result = tempBufferInitFuncPtr(buffer, device);
    nvnCmdBuf = buffer;

    if (!hasInitImGui) {
        Logger::log("Initializing ImGui.\n");
        hasInitImGui = InitImGui();
        setup_ui();
    }

    return result;
}

nvn::GenericFuncPtrFunc getProc(nvn::Device *device, const char *procName) {
    nvn::GenericFuncPtrFunc ptr = tempGetProcAddressFuncPtr(nvnDevice, procName);

    if (strcmp(procName, "nvnQueueInitialize") == 0) {
        tempQueueInitFuncPtr = (nvn::QueueInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &queueInit;
    } else if (strcmp(procName, "nvnCommandBufferInitialize") == 0) {
        tempBufferInitFuncPtr = (nvn::CommandBufferInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &cmdBufInit;
    } else if (strcmp(procName, "nvnQueuePresentTexture") == 0) {
        tempPresentTexFunc = (nvn::QueuePresentTextureFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &presentTexture;
    } else if (strcmp(procName, "nvnWindowBuilderSetTextures") == 0) {
        tempWindowBuilderSetTexFunc = (nvn::WindowBuilderSetTexturesFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &windowBuilderSetTextures;
    } else if (strcmp(procName, "nvnTextureGetWidth") == 0) {
        tempTexGetWidthFunc = (nvn::TextureGetWidthFunc) ptr;
    } else if (strcmp(procName, "nvnTextureGetHeight") == 0) {
        tempTexGetHeightFunc = (nvn::TextureGetHeightFunc) ptr;
    } else if (strcmp(procName, "nvnWindowSetCrop") == 0) {
        tempWindowSetCropFunc = (nvn::WindowSetCropFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &setCrop;
    } else if (strcmp(procName, "nvnCommandBufferSetTexturePool") == 0) {
        tempCommandSetTexturePoolFunc = (nvn::CommandBufferSetTexturePoolFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &setTexturePool;
    } else if (strcmp(procName, "nvnCommandBufferSetSamplerPool") == 0) {
        tempCommandSetSamplerPoolFunc = (nvn::CommandBufferSetSamplerPoolFunc ) ptr;
        return (nvn::GenericFuncPtrFunc) &setSamplerPool;
    } else if (strcmp(procName, "nvnDeviceInitialize") == 0) {  // Duplicated from NvnBootstrapHook to make porting to e.g. BDSP easier
        tempDeviceInitFuncPtr = (nvn::DeviceInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &deviceInit;
    } else if (strcmp(procName, "nvnDeviceGetProcAddress") == 0) {
        tempGetProcAddressFuncPtr = (nvn::DeviceGetProcAddressFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &getProc;
    }

    return ptr;
}

HOOK_DEFINE_TRAMPOLINE(NvnBootstrapHook) {
    static void *Callback(const char *funcName) {
        void *result = Orig(funcName);

        Logger::log("Installing nvn Hooks.\n");

        Logger::log("Installing %s Hook.\n", funcName);

        if (strcmp(funcName, "nvnDeviceInitialize") == 0) {
            tempDeviceInitFuncPtr = (nvn::DeviceInitializeFunc) result;
            return (void *) &deviceInit;
        }
        if (strcmp(funcName, "nvnDeviceGetProcAddress") == 0) {
            tempGetProcAddressFuncPtr = (nvn::DeviceGetProcAddressFunc) result;
            return (void *) &getProc;
        }

        return result;
    }
};

#define INPUT_HOOK(type)                                                                        \
HOOK_DEFINE_TRAMPOLINE(Disable##type) {                                                         \
    static int Callback(int *unkInt, nn::hid::Npad##type *state, int count, uint const &port) { \
        int result = Orig(unkInt, state, count, port);                                          \
        if (!InputHelper::isReadInputs()) {                                                     \
            if(InputHelper::isInputToggled()) {                                                 \
                *state = nn::hid::Npad##type();                                                 \
            }                                                                                   \
        }                                                                                       \
        return result;                                                                          \
    }                                                                                           \
};

INPUT_HOOK(FullKeyState);
INPUT_HOOK(HandheldState);
INPUT_HOOK(JoyDualState);
INPUT_HOOK(JoyLeftState);
INPUT_HOOK(JoyRightState);

void exl_imgui_main() {
    NvnBootstrapHook::InstallAtSymbol("nvnBootstrapLoader");
    DisableFullKeyState::InstallAtSymbol("_ZN2nn3hid6detail13GetNpadStatesEPiPNS0_16NpadFullKeyStateEiRKj");
    DisableHandheldState::InstallAtSymbol("_ZN2nn3hid6detail13GetNpadStatesEPiPNS0_17NpadHandheldStateEiRKj");
    DisableJoyDualState::InstallAtSymbol("_ZN2nn3hid6detail13GetNpadStatesEPiPNS0_16NpadJoyDualStateEiRKj");
    DisableJoyLeftState::InstallAtSymbol("_ZN2nn3hid6detail13GetNpadStatesEPiPNS0_16NpadJoyLeftStateEiRKj");
    DisableJoyRightState::InstallAtSymbol("_ZN2nn3hid6detail13GetNpadStatesEPiPNS0_17NpadJoyRightStateEiRKj");
}
