#pragma once

#include <Simp1e/ISystemManager.h>
#include <Simp1e/SystemTypeFromType.h>
#include <function_pointer.h>
#include <void_pointer.h>

#include <utility>

namespace Simp1e {

    class ILocalSystemManager : public ISystemManager {
        virtual void* AddSystemPointer(
            SystemType systemType, VoidPointer systemPointer, FunctionPointer systemUpdateFunctionPointer
        ) = 0;

        template <typename T, typename... Args>
        T* Add(Args&&... args) {
            auto* system       = new T(std::forward<Args>(args)...);
            auto  systemUpdate = [system](IEngine* engine, double deltaTime) { system->Update(engine, deltaTime); };
            AddSystemPointer(SystemTypeFromType<T>(), void_pointer(system), function_pointer(systemUpdate));
            return static_cast<T*>(system);
        }
    };
}
