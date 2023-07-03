#pragma once

#include "IOnClickComponent.h"

namespace Simp1e {

    class OnClickComponent : public IOnClickComponent {
        FunctionPointer   _functionPointer    = nullptr;
        IFunctionPointer* _rawFunctionPointer = nullptr;

    public:
        OnClickComponent() = default;
        OnClickComponent(FunctionPointer functionPointer) : _functionPointer(std::move(functionPointer)) {
            _rawFunctionPointer = _functionPointer.get();
        }
        OnClickComponent(IFunctionPointer* functionPointer) : _rawFunctionPointer(functionPointer) {}

        IFunctionPointer* GetFunctionPointer() const override { return _rawFunctionPointer; }
        void              SetFunctionPointer(IFunctionPointer* functionPointer) override {
            _rawFunctionPointer = functionPointer;
            _functionPointer.reset();
            SetDirtyFlag(Fields::FunctionPointer);
        }
    };
}