#pragma once

#include "FunctionPointerComponentHelper.h"
#include "IOnClickComponent.h"

namespace Simp1e {

    class OnClickComponent : public IOnClickComponent, FunctionPointerComponentHelper {
    public:
        OnClickComponent() = default;
        OnClickComponent(IFunctionPointer* functionPointer) : IOnClickComponent(functionPointer) {}
        OnClickComponent(FunctionPointer functionPointer)
            : FunctionPointerComponentHelper(std::move(functionPointer)),
              IOnClickComponent(GetUniqueFunctionPointer()) {}
    };
}