#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/Entity.h>

#include <string>

namespace Simp1e {

    class WindowMenuComponent {
        Entity      _windowEntity;
        std::string _text;

    public:
        DEFINE_COMPONENT_TYPE("Menu");

        WindowMenuComponent(Entity windowEntity, const char* text) : _windowEntity(windowEntity), _text(text) {}

        Entity      GetWindowEntity() const { return _windowEntity; }
        const char* GetText() const { return _text.c_str(); }
    };
}
