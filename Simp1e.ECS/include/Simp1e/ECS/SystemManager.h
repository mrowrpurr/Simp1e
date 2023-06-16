#pragma once

#include <vector>

#include "SystemExecutor.h"

namespace Simp1e::ECS {

    class SystemManager {
        std::vector<std::unique_ptr<SystemExecutor>> _systems;
        std::unordered_map<SystemType, size_t>       _systemIndices;

    public:
        template <typename T>
        void AddSystem(const SystemType& systemType, T* system) {
            _systems.emplace_back(std::make_unique<SystemExecutor>(
                systemType, system,
                [](SystemPointer& system) { static_cast<T*>(system.get())->Update(); }
            ));
            _systemIndices[systemType] = _systems.size() - 1;
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
        void AddSystem() {
            AddSystem(T::GetSystemType(), new T());
        }

        template <typename T>
        void RemoveSystem(T* system) {
            RemoveSystem(T::GetSystemType());
        }

        void RemoveSystem(const SystemType& systemType) {
            auto it = _systemIndices.find(systemType);
            if (it != _systemIndices.end()) {
                _systems.erase(_systems.begin() + it->second);
                _systemIndices.erase(it);
            }
        }

        template <typename T>
        void RemoveSystem() {
            RemoveSystem(T::GetSystemType());
        }

        void EnableSystem(const SystemType& systemType) {
            auto it = _systemIndices.find(systemType);
            if (it != _systemIndices.end()) {
                _systems[it->second]->Enable();
            }
        }

        template <typename T>
        void EnableSystem() {
            EnableSystem(T::GetSystemType());
        }

        void DisableSystem(const SystemType& systemType) {
            auto it = _systemIndices.find(systemType);
            if (it != _systemIndices.end()) {
                _systems[it->second]->Disable();
            }
        }

        template <typename T>
        void DisableSystem() {
            DisableSystem(T::GetSystemType());
        }

        void ToggleSystem(const SystemType& systemType) {
            auto it = _systemIndices.find(systemType);
            if (it != _systemIndices.end()) {
                _systems[it->second]->Toggle();
            }
        }

        template <typename T>
        void ToggleSystem() {
            ToggleSystem(T::GetSystemType());
        }

        void Clear() { _systems.clear(); }

        void Update() {
            for (auto& system : _systems) {
                system->Update();
            }
        }

        size_t GetSystemCount() const { return _systems.size(); }

        void operator()() { Update(); }
    };
}
