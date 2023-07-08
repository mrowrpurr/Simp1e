#pragma once

#include <Simp1e/IEngine.h>
#include <Simp1e/ISystem.h>
#include <Simp1e/SystemPointer.h>
#include <function_pointer.h>
#include <void_pointer.h>

#include <memory>

namespace Simp1e {

    template <typename T>
    class LocalSystem : public ISystem {
        std::unique_ptr<T>                                        _system;
        std::unique_ptr<IFunctionPointer<void(IEngine*, double)>> _updateFunction;

        LocalSystem(const LocalSystem&)            = delete;
        LocalSystem& operator=(const LocalSystem&) = delete;

    public:
        LocalSystem(T* system) : _system(std::unique_ptr<T>(system)) {
            _updateFunction = unique_function_pointer([this](IEngine* engine, double deltaTime) {
                _system->Update(engine, deltaTime);
            });
        }

        SystemPointer GetSystemPointer() const override { return _system.get(); }
        void          Update(IEngine* engine, double deltaTime) override { _updateFunction->invoke(engine, deltaTime); }
    };
}
