#pragma once

#include <string>

#include "IWindowMenuItemComponent.h"

namespace Simp1e {

    class WindowMenuItemComponent : public IWindowMenuItemComponent {
        Entity            _windowMenuEntity;
        std::string       _text;
        bool              _visible          = true;
        bool              _enabled          = true;
        FunctionPointer   _clickFunction    = nullptr;
        IFunctionPointer* _rawClickFunction = nullptr;

    public:
        WindowMenuItemComponent(Entity windowMenuEntity, const char* text, FunctionPointer clickFunction = nullptr)
            : _windowMenuEntity(windowMenuEntity), _text(text), _clickFunction(std::move(clickFunction)) {
            if (_clickFunction) _rawClickFunction = _clickFunction.get();
        }

        Entity      GetWindowMenuEntity() const override { return _windowMenuEntity; }
        const char* GetText() const override { return _text.c_str(); }
        void        SetText(const char* text) override {
            _text = text;
            SetDirtyFlag(Fields::Text);
        }
        bool IsVisible() const override { return _visible; }
        void SetVisible(bool visible) override {
            _visible = visible;
            SetDirtyFlag(Fields::Visible);
        }
        bool IsEnabled() const override { return _enabled; }
        void SetEnabled(bool enabled) override {
            _enabled = enabled;
            SetDirtyFlag(Fields::Enabled);
        }
        void SetClickFunction(FunctionPointer functionPointer) {
            _clickFunction    = std::move(functionPointer);
            _rawClickFunction = _clickFunction.get();
            SetDirtyFlag(Fields::ClickFunction);
        }
        void SetClickFunction(IFunctionPointer* functionPointer) override {
            _rawClickFunction = functionPointer;
            _clickFunction.reset();
            SetDirtyFlag(Fields::ClickFunction);
        }
        IFunctionPointer* GetClickFunction() const override { return _rawClickFunction; }
    };
}
