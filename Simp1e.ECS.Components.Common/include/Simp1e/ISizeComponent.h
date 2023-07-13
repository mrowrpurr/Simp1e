#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/Size.h>
#include <Simp1e/sreal.h>

namespace Simp1e {

    struct ISizeComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Size")

        enum class Fields : int {
            Width  = 1 << 0,
            Height = 1 << 1,
        };

        virtual Size GetSize() const           = 0;
        virtual void SetSize(const Size& size) = 0;

        virtual sreal GetWidth() const  = 0;
        virtual sreal GetHeight() const = 0;

        virtual void SetWidth(sreal width)   = 0;
        virtual void SetHeight(sreal height) = 0;
    };
}