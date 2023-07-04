#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/Entity.h>
#include <Simp1e/IHasParentEntity.h>

namespace Simp1e {

    struct ILabelComponent : public DirtyTrackingComponentBase, public IHasParentEntity {
        DEFINE_COMPONENT_TYPE("Label");

        enum class Fields : int {
            ParentEntity = 1 << 0,
            Text         = 1 << 1,
        };

        virtual const char* GetText() const           = 0;
        virtual void        SetText(const char* text) = 0;
    };
}