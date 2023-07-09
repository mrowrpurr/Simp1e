#pragma once

#include <Simp1e/ISystemGroup.h>
#include <Simp1e/SystemTypeFromHashKey.h>
#include <Simp1e/SystemTypeHashKey.h>
#include <Simp1e/SystemTypeToHashKey.h>
#include <_Log_.h>

#include <memory>
#include <unordered_map>

namespace Simp1e {

    class LocalSystemGroup : public ISystemGroup {
        std::unordered_map<SystemTypeHashKey, std::unique_ptr<IVoidPointer>> _systems;
        std::unordered_map<SystemTypeHashKey, std::unique_ptr<IFunctionPointer<void(IEngine*, double)>>>
                                                    _systemUpdateFunctions;
        std::unordered_map<SystemTypeHashKey, bool> _enabledSystems;
        std::vector<SystemTypeHashKey>              _insertionOrder;

    public:
        SystemPointer AddSystemPointer(
            SystemType systemType, IVoidPointer* systemPointer,
            IFunctionPointer<void(IEngine*, double)>* systemUpdateFunction
        ) override {
            auto systemTypeHashKey = SystemTypeToHashKey(systemType);
            if (_systems.find(systemTypeHashKey) != _systems.end()) {
                _Log_("[LocalSystemManager] System {} already exists", systemTypeHashKey);
                return nullptr;
            }
            _systems[systemTypeHashKey] = std::unique_ptr<IVoidPointer>(systemPointer);
            _systemUpdateFunctions[systemTypeHashKey] =
                std::unique_ptr<IFunctionPointer<void(IEngine*, double)>>(systemUpdateFunction);
            _enabledSystems[systemTypeHashKey] = true;
            _insertionOrder.push_back(systemTypeHashKey);
            _Log_("[LocalSystemManager] Added system {}", systemTypeHashKey);
            return systemPointer->void_ptr();
        }

        void Update(IEngine* environment, double deltaTime) override {
            for (auto& systemTypeHashKey : _insertionOrder) {
                if (!_enabledSystems[systemTypeHashKey]) continue;
                _systemUpdateFunctions[systemTypeHashKey]->invoke(environment, deltaTime);
            }
        }

        SystemPointer GetSystemPointer(SystemType systemType) override {
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
            _systemUpdateFunctions.erase(key);
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

        void ForEachSystem(
            IFunctionPointer<void(SystemType, SystemPointer, IFunctionPointer<void(IEngine*, double)>*)>* callback
        ) override {
            for (auto& systemTypeHashKey : _insertionOrder) {
                if (!_enabledSystems[systemTypeHashKey]) continue;
                auto foundUpdateFunction = _systemUpdateFunctions.find(systemTypeHashKey);
                if (foundUpdateFunction == _systemUpdateFunctions.end()) continue;
                callback->invoke(
                    SystemTypeFromHashKey(systemTypeHashKey), _systems[systemTypeHashKey].get(),
                    foundUpdateFunction->second.get()
                );
            }
        }
    };
}
