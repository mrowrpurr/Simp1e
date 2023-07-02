#pragma once

#include <Simp1e/Executable.h>
#include <Simp1e/ISystemPointerManager.h>
#include <Simp1e/SystemTypeHashKey.h>
#include <_Log_.h>
#include <void_pointer.h>

#include <memory>
#include <unordered_map>
#include <utility>

namespace Simp1e {

    class SystemPointerManagerClient : public ISystemManager {
        ISystemPointerManager*                                              _systemManager;
        std::unordered_map<SystemTypeHashKey, VoidPointer>                  _systems;
        std::unordered_map<SystemTypeHashKey, std::unique_ptr<IExecutable>> _systemExecutables;

    public:
        SystemPointerManagerClient(ISystemPointerManager* systemManager) : _systemManager(systemManager) {}
        SystemPointerManagerClient(ISystemManager* systemManager)
            : _systemManager(dynamic_cast<ISystemPointerManager*>(systemManager)) {}

        void Update(IEnvironment* environment) override { _systemManager->Update(environment); }

        template <typename T>
        T* AddSystemPointer(SystemType systemType, T* system) {
            auto* systemExecutable         = new Executable([system](void* environmentPointer) {
                auto* environment = static_cast<IEnvironment*>(environmentPointer);
                system->Update(environment);
            });
            _systems[systemType]           = void_pointer(system);
            _systemExecutables[systemType] = std::unique_ptr<IExecutable>(systemExecutable);
            _systemManager->AddSystemPointer(systemType, systemExecutable);
            _Log_("[SystemPointerManagerClient] Added System {}", systemType);
            return system;
        }

        template <typename T, typename... Args>
        T* Add(Args&&... args) {
            auto* system = new T(std::forward<Args>(args)...);
            AddSystemPointer(T::GetSystemType(), system);
            return system;
        }

        IExecutable* GetSystemPointer(SystemType systemType) override { return _systemExecutables[systemType].get(); }

        template <typename T>
        T* GetSystem(SystemType systemType) {
            return static_cast<T*>(_systems[systemType].get());
        }

        template <typename T>
        T* GetSystem() {
            return GetSystem<T>(T::GetSystemType());
        }

        bool RemoveSystem(SystemType systemType) override {
            auto systemIterator = _systems.find(systemType);
            if (systemIterator != _systems.end()) _systems.erase(systemIterator);
            auto executableIterator = _systemExecutables.find(systemType);
            if (executableIterator != _systemExecutables.end()) _systemExecutables.erase(executableIterator);
            return _systemManager->RemoveSystem(systemType);
        }

        template <typename T>
        bool RemoveSystem() {
            return RemoveSystem(T::GetSystemType());
        }

        bool HasSystem(SystemType systemType) override { return _systemManager->HasSystem(systemType); }

        template <typename T>
        bool HasSystem() {
            return HasSystem(T::GetSystemType());
        }

        bool EnableSystem(SystemType systemType) override { return _systemManager->EnableSystem(systemType); }

        template <typename T>
        bool EnableSystem() {
            return EnableSystem(T::GetSystemType());
        }

        bool DisableSystem(SystemType systemType) override { return _systemManager->DisableSystem(systemType); }

        template <typename T>
        bool DisableSystem() {
            return DisableSystem(T::GetSystemType());
        }

        bool IsSystemEnabled(SystemType systemType) override { return _systemManager->IsSystemEnabled(systemType); }

        template <typename T>
        bool IsSystemEnabled() {
            return IsSystemEnabled(T::GetSystemType());
        }
    };
}
