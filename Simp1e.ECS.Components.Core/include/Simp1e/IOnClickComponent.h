#pragma once

#include <Simp1e/DefineComponentType.h>
#include <function_pointer.h>

#include "DirtyTrackingComponentBase.h"

namespace Simp1e {

    struct IOnClickComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("OnClick")

        enum class Fields {
            FunctionPointer = 1 << 0,
        };

        virtual IFunctionPointerBase* GetFunctionPointer() const                                = 0;
        virtual void                  SetFunctionPointer(IFunctionPointerBase* functionPointer) = 0;
    };
}