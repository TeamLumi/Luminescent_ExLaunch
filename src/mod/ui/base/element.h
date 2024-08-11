#pragma once

#include <string>
#include <functional>

#include "imgui.h"
#include "exlaunch.hpp"

#include "memory/vector.h"

#include "logger/logger.h"

namespace ui {
    class Drawable {
    protected:
        nn::vector<Drawable *> mChildren;
        virtual bool beginDraw() {
            return true;
        }

        virtual void draw() {
            for (auto child: mChildren) {
                child->render();
            }
        }

        virtual void endDraw() {}
    public:
        virtual ~Drawable() = default;

        virtual void render() {
            if (this->beginDraw()) {
                this->draw();
                this->endDraw();
            }
        }

        void addChild(Drawable *child) {
            mChildren.push_back(child);
        }

        void clearChildren() {
            mChildren.clear();
        }
    };

#define ELEMENT(TName, ...) struct TName : public ui::Factory<TName> __VA_OPT__(,) __VA_ARGS__
#define ELEMENT_SUPPORTS_CHILD(TName) struct TName* TName(std::function<void(struct TName&)> initializer) { \
    struct TName* instance = TName::create(initializer);                                                    \
    mChildren.push_back(instance);                                                                          \
    mOwnedChildren.push_back(instance);                                                                     \
    return instance;                                                                                        \
}                                                                                                           \
struct TName* TName() {                                                                                     \
    return TName([](struct TName&){});                                                                      \
}

#define COMMON_ELEMENTS()                   \
ELEMENT_SUPPORTS_CHILD(Button)              \
ELEMENT_SUPPORTS_CHILD(Checkbox)            \
ELEMENT_SUPPORTS_CHILD(ColorEdit3)          \
ELEMENT_SUPPORTS_CHILD(Combo)               \
ELEMENT_SUPPORTS_CHILD(ComboSimple)         \
ELEMENT_SUPPORTS_CHILD(FunctionElement)     \
ELEMENT_SUPPORTS_CHILD(InputInt)            \
ELEMENT_SUPPORTS_CHILD(InputFloat)          \
ELEMENT_SUPPORTS_CHILD(MenuBar)             \
ELEMENT_SUPPORTS_CHILD(SliderFloat)         \
ELEMENT_SUPPORTS_CHILD(SliderFloat2)        \
ELEMENT_SUPPORTS_CHILD(SliderFloat3)        \
ELEMENT_SUPPORTS_CHILD(SliderInt)           \
ELEMENT_SUPPORTS_CHILD(Spacing)             \
ELEMENT_SUPPORTS_CHILD(Text)                \
ELEMENT_SUPPORTS_CHILD(TextUnformatted)     \
ELEMENT_SUPPORTS_CHILD(FileSelector)        \
ELEMENT_SUPPORTS_CHILD(ElementInspector)    \
ELEMENT_SUPPORTS_CHILD(DataViewer)          \
ELEMENT_SUPPORTS_CHILD(StringView)

#define TOOL_ELEMENTS()                     \
ELEMENT_SUPPORTS_CHILD(ItemTool)            \
ELEMENT_SUPPORTS_CHILD(PokemonTool)         \
ELEMENT_SUPPORTS_CHILD(ArenaTool)           \
ELEMENT_SUPPORTS_CHILD(WarpTool)            \
ELEMENT_SUPPORTS_CHILD(VariablesTool)       \
ELEMENT_SUPPORTS_CHILD(ColorVariationTool)  \
ELEMENT_SUPPORTS_CHILD(SaveTool)            \
ELEMENT_SUPPORTS_CHILD(ScriptTool)          \
ELEMENT_SUPPORTS_CHILD(AnimationTool)       \
ELEMENT_SUPPORTS_CHILD(ModelTool)           \
ELEMENT_SUPPORTS_CHILD(MaterialTool)        \
ELEMENT_SUPPORTS_CHILD(PokemonInfoTool)     \
ELEMENT_SUPPORTS_CHILD(PoffinTool)          \
ELEMENT_SUPPORTS_CHILD(MiscTool)

    template<typename T>
    class Factory : public virtual ui::Drawable {
    protected:
        nn::vector<Drawable*> mOwnedChildren;

        virtual bool isValid() {
            return true;
        }

        ~Factory() override {
            for (auto owned : mOwnedChildren) {
                delete owned;
            }
        }

    public:
        static T* create(std::function<void(T &)> initializer) {
            T* instance = IM_NEW(T);
            initializer(*instance);
//            if (!instance->isValid()) {
//                EXL_ABORT(0, "Type reported invalid after initializing!");
//            }
            return instance;
        }

        static T single(std::function<void(T &)> initializer) {
            T instance{};
            initializer(instance);
//            if (!instance.isValid()) {
//                EXL_ABORT(0, "Type reported invalid after initializing!");
//            }
            return instance;
        }
    };
}
