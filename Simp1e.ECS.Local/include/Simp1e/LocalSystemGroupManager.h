#pragma once

#include <Simp1e/SystemTypeHashKey.h>
#include <Simp1e/SystemTypeToHashKey.h>
#include <_Log_.h>

#include <"ILocalSystemGroupManager.hS">
#include <memory>
#include <unordered_map>

#include "LocalSystem.h"

namespace Simp1e {

    class LocalSystemGroupManager : public ILocalSystemGroupManager {
        std::unordered_map<SystemTypeHashKey, std::unique_ptr<ISystem>> _systems;
        std::unordered_map<SystemTypeHashKey, bool>                     _enabledSystems;
        std::vector<SystemTypeHashKey>                                  _insertionOrder;

    public:
        void Update(IEngine* environment, double deltaTime) override {
            _Log_("[LocalSystemManager] Update");
            for (auto& systemTypeHashKey : _insertionOrder) {
                if (!_enabledSystems[systemTypeHashKey]) continue;
                _systems[systemTypeHashKey]->Update(environment, deltaTime);
            }
        }

        virtual ISystem* GetSystemPointer(SystemType systemType) override {
            auto found = _systems.find(SystemTypeToHashKey(systemType));
            if (found == _systems.end()) return nullptr;
            return found->second.get();
        }

        virtual bool RemoveSystem(SystemType systemType) override {
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

        virtual bool HasSystem(SystemType systemType) override {
            return _systems.find(SystemTypeToHashKey(systemType)) != _systems.end();
        }

        virtual bool EnableSystem(SystemType systemType) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            _enabledSystems[key] = true;
            _Log_("[LocalSystemManager] Enabled system {}", systemType);
            return true;
        }

        virtual bool DisableSystem(SystemType systemType) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            _enabledSystems[key] = false;
            _Log_("[LocalSystemManager] Disabled system {}", systemType);
            return true;
        }

        virtual bool IsSystemEnabled(SystemType systemType) override {
            auto key   = SystemTypeToHashKey(systemType);
            auto found = _systems.find(key);
            if (found == _systems.end()) return false;
            return _enabledSystems[key];
        }

        void* AddSystemPointer(
            SystemType systemType, VoidPointer systemPointer, FunctionPointer systemUpdateFunctionPointer
        ) override {
            auto key = SystemTypeToHashKey(systemType);
            if (_systems.find(key) != _systems.end()) {
                _Log_("[LocalSystemManager] System {} already exists", systemType);
                return nullptr;
            }
            auto system          = new LocalSystem(std::move(systemPointer), std::move(systemUpdateFunctionPointer));
            _systems[key]        = std::unique_ptr<ISystem>(system);
            _enabledSystems[key] = true;
            _insertionOrder.push_back(key);
            _Log_("[LocalSystemManager] Added system {}", systemType);
            return system->GetSystemPointer();
        }
    };
}
