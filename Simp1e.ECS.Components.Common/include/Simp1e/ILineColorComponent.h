#pragma once

#include <Simp1e/Color.h>
#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct ILineColorComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("LineColor")

        enum class Fields : int {
            Color = 1 << 0,
        };

        virtual Color GetColor() const             = 0;
        virtual void  SetColor(const Color& color) = 0;
    };
}