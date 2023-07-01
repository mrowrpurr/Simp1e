#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct ITextComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("TextComponent")
        virtual const char* GetText() const           = 0;
        virtual void        SetText(const char* name) = 0;
    };
}