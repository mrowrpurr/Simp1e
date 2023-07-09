#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/Entity.h>
#include <Simp1e/IHasParentEntity.h>
#include <Simp1e/Size.h>

namespace Simp1e {

    struct ICanvasComponent : public DirtyTrackingComponentBase, public IHasParentEntity {
        DEFINE_COMPONENT_TYPE("Canvas");

        enum class Fields : int {
            ParentEntity = 1 << 0,
            Width        = 1 << 1,
            Height       = 1 << 2,
        };

        // TODO: get rid of size! This should come from the SizeComponent
        virtual Size GetSize() const = 0;

        virtual int  GetWidth() const    = 0;
        virtual void SetWidth(int width) = 0;

        virtual int  GetHeight() const     = 0;
        virtual void SetHeight(int height) = 0;
    };
}