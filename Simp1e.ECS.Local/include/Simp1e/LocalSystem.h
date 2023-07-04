#pragma once

#include <Simp1e/IEngine.h>
#include <Simp1e/ISystem.h>
#include <void_pointer.h>

#include <memory>

namespace Simp1e {

    class LocalSystem : public ISystem {
        VoidPointer     _system;
        FunctionPointer _updateFunction;

        LocalSystem(const LocalSystem&)            = delete;
        LocalSystem& operator=(const LocalSystem&) = delete;

    public:
        LocalSystem(VoidPointer system, FunctionPointer updateFunction)
            : _system(std::move(system)), _updateFunction(std::move(updateFunction)) {}

        void* GetSystemPointer() const override { return _system->void_ptr(); }
        void  Update(IEngine* engine, double deltaTime) override {
            function_pointer::invoke(_updateFunction, engine, deltaTime);
        }
    };
}
