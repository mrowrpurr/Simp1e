#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/Entity.h>

namespace Simp1e {

    struct IWindowMenuComponent {
        DEFINE_COMPONENT_TYPE("WindowMenu");

        virtual Entity      GetWindowEntity() const = 0;
        virtual const char* GetText() const         = 0;
    };
}
