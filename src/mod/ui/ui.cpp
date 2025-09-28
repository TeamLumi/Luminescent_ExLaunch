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

int GeneratePoffinName(int spicy, int dry, int sweet, int bitter, int sour, int level) {
    if (spicy == 0 && dry == 0 && sweet == 0 && bitter == 0 && sour == 0) {
        return 27; // Foul
    }
    else {
        if (level >= 50 && level < 95) {
            return 28; // Mild
        }
        else if (level >= 95) {
            return 29; // Super Mild
        }
        else {
            int flavorCount = 0;
            if (spicy != 0) flavorCount++;
            if (dry != 0) flavorCount++;
            if (sweet != 0) flavorCount++;
            if (bitter != 0) flavorCount++;
            if (sour != 0) flavorCount++;

            if (flavorCount == 3)
                return 25; // Rich
            else if (flavorCount >= 4)
                return 26; // Overripe

            if (spicy >= dry && spicy >= sweet && spicy >= bitter && spicy >= sour) {
                if (dry == 0 && sweet == 0 && bitter == 0 && sour == 0)
                    return 0; // Spicy
                else if (dry >= sweet && dry >= bitter && dry >= sour)
                    return 1; // Spicy-Dry
                else if (sweet >= dry && sweet >= bitter && sweet >= sour)
                    return 2; // Spicy-Sweet
                else if (bitter >= dry && bitter >= sweet && bitter >= sour)
                    return 3; // Spicy-Bitter
                else if (sour >= dry && sour >= sweet && sour >= bitter)
                    return 4; // Spicy-Sour

                return 0; // Spicy
            }
            else if (dry >= spicy && dry >= sweet && dry >= bitter && dry >= sour) {
                if (spicy == 0 && sweet == 0 && bitter == 0 && sour == 0)
                    return 5; // Dry
                else if (spicy >= sweet && spicy >= bitter && spicy >= sour)
                    return 6; // Dry-Spicy
                else if (sweet >= spicy && sweet >= bitter && sweet >= sour)
                    return 7; // Dry-Sweet
                else if (bitter >= spicy && bitter >= sweet && bitter >= sour)
                    return 8; // Dry-Bitter
                else if (sour >= spicy && sour >= sweet && sour >= bitter)
                    return 9; // Dry-Sour

                return 5; // Dry
            }
            else if (sweet >= spicy && sweet >= dry && sweet >= bitter && sweet >= sour) {
                if (spicy == 0 && dry == 0 && bitter == 0 && sour == 0)
                    return 10; // Sweet
                else if (spicy >= dry && spicy >= bitter && spicy >= sour)
                    return 11; // Sweet-Spicy
                else if (dry >= spicy && dry >= bitter && dry >= sour)
                    return 12; // Sweet-Dry
                else if (bitter >= spicy && bitter >= dry && bitter >= sour)
                    return 13; // Sweet-Bitter
                else if (sour >= spicy && sour >= dry && sour >= bitter)
                    return 14; // Sweet-Sour

                return 10; // Sweet
            }
            else if (bitter >= spicy && bitter >= dry && bitter >= sweet && bitter >= sour) {
                if (spicy == 0 && dry == 0 && sweet == 0 && sour == 0)
                    return 15; // Bitter
                else if (spicy >= dry && spicy >= sweet && spicy >= sour)
                    return 16; // Bitter-Spicy
                else if (dry >= spicy && dry >= sweet && dry >= sour)
                    return 17; // Bitter-Dry
                else if (sweet >= spicy && sweet >= dry && sweet >= sour)
                    return 18; // Bitter-Sweet
                else if (sour >= spicy && sour >= dry && sour >= sweet)
                    return 19; // Bitter-Sour

                return 15; // Bitter
            }
            else if (sour >= spicy && sour >= dry && sour >= sweet && sour >= bitter) {
                if (spicy == 0 && dry == 0 && sweet == 0 && bitter == 0)
                    return 20; // Sour
                else if (spicy >= dry && spicy >= sweet && spicy >= bitter)
                    return 21; // Sour-Spicy
                else if (dry >= spicy && dry >= sweet && dry >= bitter)
                    return 22; // Sour-Dry
                else if (sweet >= spicy && sweet >= dry && sweet >= bitter)
                    return 23; // Sour-Sweet
                else if (bitter >= spicy && bitter >= dry && bitter >= sweet)
                    return 24; // Sour-Bitter

                return 20; // Sour
            }

            return 27; // Foul
        }
    }
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
