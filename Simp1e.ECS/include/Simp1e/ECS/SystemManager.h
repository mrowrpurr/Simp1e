#pragma once

#include "SystemExecutor.h"

namespace Simp1e::ECS {

    class SystemManager {
        std::unordered_map<SystemType, std::unique_ptr<SystemExecutor>> _systems;

    public:
        template <typename T>
        void AddSystem(const SystemType& systemType, T* system) {
            _systems[systemType] = std::make_unique<SystemExecutor>(
                systemType, system, [](SystemPointer system) { static_cast<T*>(system)->Update(); }
            );
        }

        template <typename T>
        void AddSystem(const SystemType& systemType) {
            AddSystem(systemType, new T());
        }

        template <typename T>
        void AddSystem(T* system) {
            AddSystem(T::GetSystemType(), system);
        }

        template <typename T>
        void AddSystem(const SystemType& systemType, T&& system) {
            AddSystem(systemType, new T(std::forward<T>(system)));
        }

        template <typename T>
        void AddSystem(T&& system) {
            AddSystem(T::GetSystemType(), std::forward<T>(system));
        }

        template <typename T>
        void RemoveSystem(const SystemType& systemType) {
            _systems.erase(systemType);
        }

        void RemoveSystem(const SystemType& systemType) { _systems.erase(systemType); }

        void Clear() { _systems.clear(); }

        void Update() {
            for (auto& system : _systems) system.second->Update();
        }

        void operator()() { Update(); }
    };
}
