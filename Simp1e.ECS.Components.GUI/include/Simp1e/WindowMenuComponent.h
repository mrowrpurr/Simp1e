#pragma once

#include <string>

#include "IWindowMenuComponent.h"

namespace Simp1e {

    class WindowMenuComponent : public IWindowMenuComponent {
        Entity      _windowEntity;
        std::string _text;

    public:
        WindowMenuComponent(Entity windowEntity, const char* text) : _windowEntity(windowEntity), _text(text) {}

        Entity      GetWindowEntity() const override { return _windowEntity; }
        const char* GetText() const override { return _text.c_str(); }
    };
}
