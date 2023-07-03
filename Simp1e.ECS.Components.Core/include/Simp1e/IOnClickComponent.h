#pragma once

#include <Simp1e/DefineComponentType.h>

#include "FunctionPointerComponentBase.h"

namespace Simp1e {

    struct IOnClickComponent : public FunctionPointerComponentBase {
        DEFINE_COMPONENT_TYPE("OnClick")

        IOnClickComponent() = default;
        IOnClickComponent(IFunctionPointer* functionPointer) : FunctionPointerComponentBase(functionPointer) {}
    };
}