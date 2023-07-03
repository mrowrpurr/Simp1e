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

        virtual IFunctionPointer* GetFunctionPointer() const                            = 0;
        virtual void              SetFunctionPointer(IFunctionPointer* functionPointer) = 0;
    };
}