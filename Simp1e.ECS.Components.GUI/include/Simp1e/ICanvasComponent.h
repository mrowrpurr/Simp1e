#pragma once

#include <Simp1e/DefineComponentType.h>

namespace Simp1e {

    struct ICanvasComponent {
        DEFINE_COMPONENT_TYPE("Canvas");

        virtual ~ICanvasComponent() = default;
    };
}