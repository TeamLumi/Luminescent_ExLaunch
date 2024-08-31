#include "externals/il2cpp.h"

#include "ui/base/root_element.h"
#include "ui/base/window.h"
#include "ui/components/data_viewer.h"
#include "ui/components/element_inspector.h"
#include "ui/ui.h"

#include "logger/logger.h"

using namespace ui;

static Window debugLogWindow = Window::single([](Window &_) {
    _.title = "Debug Log";
    auto *view = _.StringView();

    // WARNING: Turning this on is a ticking time bomb.
    // Eventually there are too many logs and the game hangs.
    /*Logger::addListener([view](const char *message) {
        view->append(message);
    });*/
});

static ElementInspector inspector = ElementInspector::single([](ElementInspector &_) {

});

static DataViewer dataViewer = DataViewer::single([](DataViewer &_) {

});

void ui_inspect(UnityEngine::Transform* transform) {
    inspector.root = transform;
}

static Window inspectorWindow = Window::single([](Window &_) {
    _.title = "Inspector";
    _.flags |= ImGuiWindowFlags_MenuBar;
    _.initialPos = ImVec2(550, 50);
    _.initialSize = ImVec2(600, 650);
    _.allow_static = true;

    _.MenuBar([](MenuBar &_) {
        _.Menu([](Menu &_) {
            _.label = "Options";
            _.MenuItem([](MenuItem &_) {
                _.label = "Show Children";
                _.checked = &inspector.showChildren;
            });
            _.MenuItem([](MenuItem &_) {
                _.label = "Show Components";
                _.checked = &inspector.showComponents;
            });
        });
    });
    _.addChild(&inspector);
});

static Window dataViewerWindow = Window::single([](Window &_) {
    _.title = "Data Viewer";
    _.flags |= ImGuiWindowFlags_MenuBar;
    _.initialPos = ImVec2(550, 50);
    _.initialSize = ImVec2(150, 150);
    _.allow_static = true;
    _.addChild(&dataViewer);
});

static Window mainWindow = Window::single([](Window &_) {
    _.title = "BDSP Toolbox";
    _.toggleable = false;
    _.flags |= ImGuiWindowFlags_MenuBar;
    _.initialPos = ImVec2(50, 50);
    _.initialSize = ImVec2(450, 650);

    _.Text([](Text &_) {
        _.content = "Press ZL+ZR to toggle all menus.\nHold L to walk through walls.";
    });

    _.Spacing();

    _.MenuBar([](MenuBar &_) {
        _.Menu([](Menu &_) {
            _.label = "Tools";
            _.MenuItem([](MenuItem &_) {
                _.label = "Debug Log";
                _.checked = &debugLogWindow.open;
            });
            _.MenuItem([](MenuItem &_) {
                _.label = "Inspector";
                _.checked = &inspectorWindow.open;
            });
            _.MenuItem([](MenuItem &_) {
                _.label = "Data Viewer";
                _.checked = &dataViewerWindow.open;
            });
        });
    });

    _.ItemTool();
    _.PokemonTool();
    _.ArenaTool();
    _.WarpTool();
    _.VariablesTool();
    _.MaterialTool();
    _.ColorVariationTool();
    _.SaveTool();
    _.ScriptTool();
    _.AnimationTool();
    _.ModelTool();
    _.PokemonInfoTool();
    _.PoffinTool();
    _.MiscTool();
});

void setup_ui() {
    ROOT->addChild(&mainWindow);
    ROOT->addChild(&debugLogWindow);
    ROOT->addChild(&inspectorWindow);
    ROOT->addChild(&dataViewerWindow);
}
