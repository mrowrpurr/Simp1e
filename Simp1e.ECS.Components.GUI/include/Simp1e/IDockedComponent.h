#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IDockedComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Docked");

        enum class DockedLocation {
            Left,
            Right,
            Top,
            Bottom,
        };

        enum class Fields : int {
            Title                 = 1 << 0,
            DefaultDockedLocation = 1 << 1,
        };

        virtual const char* GetTitle() const            = 0;
        virtual void        SetTitle(const char* title) = 0;

        virtual DockedLocation GetDefaultDockedLocation() const                  = 0;
        virtual void           SetDefaultDockedLocation(DockedLocation location) = 0;
    };
}
