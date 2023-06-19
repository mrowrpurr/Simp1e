#pragma once

#include <_Log_.h>

#include <vector>

#include "SystemExecutor.h"

namespace Simp1e::ECS {

    class SystemManager {
        std::vector<std::unique_ptr<SystemExecutor>> _systems;
        std::unordered_map<SystemType, size_t>       _systemIndices;

    public:
        template <typename T>
        T* AddSystem(const SystemType& systemType, T* system) {
            _systems.emplace_back(std::make_unique<SystemExecutor>(
                systemType, system,
                [](SystemPointer& system) { static_cast<T*>(system.get())->Update(); }
            ));
            _systemIndices[systemType] = _systems.size() - 1;
            return system;
        }

        template <typename T>
        T* AddSystem(const SystemType& systemType) {
            auto* system = new T();
            AddSystem(systemType, system);
            return system;
        }

        template <typename T>
        T* AddSystem(T* system) {
            AddSystem(T::GetSystemType(), system);
            return system;
        }

        template <typename T>
        T* AddSystem(const SystemType& systemType, T&& systemValue) {
            auto* system = new T(std::forward<T>(systemValue));
            AddSystem(systemType, system);
            return system;
        }

        template <typename T>
        T* AddSystem(T&& system) {
            return AddSystem(T::GetSystemType(), std::forward<T>(system));
        }

        template <typename T>
        T* AddSystem() {
            return AddSystem(T::GetSystemType(), new T());
        }

        template <typename T>
        void RemoveSystem(T* system) {
            RemoveSystem(T::GetSystemType());
        }

        bool HasSystem(const SystemType& systemType) const {
            return _systemIndices.find(systemType) != _systemIndices.end();
        }

        template <typename T>
        bool HasSystem() const {
            return HasSystem(T::GetSystemType());
        }

        SystemExecutor* GetSystem(const SystemType& systemType) {
            auto it = _systemIndices.find(systemType);
            if (it != _systemIndices.end()) return _systems[it->second].get();
            return nullptr;
        }

        template <typename T>
        T* GetSystem() {
            auto* systemExecutor = GetSystem(T::GetSystemType());
            if (systemExecutor) return systemExecutor->template GetSystem<T>();
            return nullptr;
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
                try {
                    system->Update();
                } catch (const std::exception& e) {
                    _Log_(
                        "SystemManager::Update() system: '{}' exception: {}",
                        system->GetSystemType(), e.what()
                    );
                } catch (...) {
                    _Log_(
                        "SystemManager::Update() system: '{}' unknown exception",
                        system->GetSystemType()
                    );
                }
            }
        }

        size_t GetSystemCount() const { return _systems.size(); }

        void operator()() { Update(); }
    };
}
