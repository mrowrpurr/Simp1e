#pragma once

#include <Simp1e/ISystemGroupManager.h>
#include <Simp1e/SystemTypeFromHashKey.h>
#include <Simp1e/SystemTypeHashKey.h>
#include <Simp1e/SystemTypeToHashKey.h>
#include <_Log_.h>

#include <memory>
#include <unordered_map>

#include "LocalSystem.h"

namespace Simp1e {

    class LocalSystemGroupManager : ISystemGroupManager {
        std::unordered_map<SystemTypeHashKey, std::unique_ptr<ISystem>> _systems;
        std::unordered_map<SystemTypeHashKey, bool>                     _enabledSystems;
        std::vector<SystemTypeHashKey>                                  _insertionOrder;

    public:
        template <typename T, typename... Args>
        T* AddSystem(Args&&... args) {
            auto systemType = SystemTypeFromType<T>();
            if (_systems.find(systemType) != _systems.end()) {
                _Log_("[LocalSystemManager] System {} already exists", systemType);
                return nullptr;
            }
            auto system                 = new T(std::forward<Args>(args)...);
            auto iSystem                = new LocalSystem<T>(system);
            _systems[systemType]        = std::unique_ptr<ISystem>(iSystem);
            _enabledSystems[systemType] = true;
            _insertionOrder.push_back(systemType);
            _Log_("[LocalSystemManager] Added system {}", systemType);
            return system;
        }

        void Update(IEngine* environment, double deltaTime) override {
            for (auto& systemTypeHashKey : _insertionOrder) {
                if (!_enabledSystems[systemTypeHashKey]) continue;
                _systems[systemTypeHashKey]->Update(environment, deltaTime);
            }
        }

        ISystem* GetSystemPointer(SystemType systemType) override {
            auto found = _systems.find(SystemTypeToHashKey(systemType));
            if (found == _systems.end()) return nullptr;
            return found->second.get();
        }

        bool RemoveSystem(SystemType systemType) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            _systems.erase(key);
            _enabledSystems.erase(key);
            _insertionOrder.erase(
                std::remove(_insertionOrder.begin(), _insertionOrder.end(), key), _insertionOrder.end()
            );
            _Log_("[LocalSystemManager] Removed system {}", systemType);
            return true;
        }

        bool HasSystem(SystemType systemType) override {
            return _systems.find(SystemTypeToHashKey(systemType)) != _systems.end();
        }

        bool SetSystemEnabled(SystemType systemType, bool enabled) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            _enabledSystems[key] = enabled;
            _Log_("[LocalSystemManager] Set system {} enabled: {}", systemType, enabled);
            return true;
        }

        bool EnableSystem(SystemType systemType) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            _enabledSystems[key] = true;
            _Log_("[LocalSystemManager] Enabled system {}", systemType);
            return true;
        }

        bool DisableSystem(SystemType systemType) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            _enabledSystems[key] = false;
            _Log_("[LocalSystemManager] Disabled system {}", systemType);
            return true;
        }

        bool IsSystemEnabled(SystemType systemType) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            return _enabledSystems[key];
        }

        void ForEachSystem(IFunctionPointer<void(SystemType, ISystem*)>* callback) override {
            for (auto& systemTypeHashKey : _insertionOrder) {
                if (!_enabledSystems[systemTypeHashKey]) continue;
                callback->invoke(SystemTypeFromHashKey(systemTypeHashKey), _systems[systemTypeHashKey].get());
            }
        }

        void ForEachSystem(FunctionPointer<void(SystemType, ISystem*)> callback) {
            for (auto& systemTypeHashKey : _insertionOrder) {
                if (!_enabledSystems[systemTypeHashKey]) continue;
                callback.invoke(SystemTypeFromHashKey(systemTypeHashKey), _systems[systemTypeHashKey].get());
            }
        }
    };
}
