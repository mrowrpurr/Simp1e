#pragma once

#include <Simp1e/IEnvironment.h>
#include <Simp1e/SystemTypeToHashKey.h>
#include <_Log_.h>

#include <unordered_map>
#include <vector>

#include "ISystemPointerManager.h"

namespace Simp1e {

    class SystemPointerManager : public ISystemPointerManager {
        std::unordered_map<SystemTypeHashKey, IExecutable*> _systemPointers;
        std::unordered_map<SystemTypeHashKey, bool>         _enabledSystems;
        std::vector<SystemTypeHashKey>                      _insertionOrder;

    public:
        void Update(IEnvironment* environment, double deltaTime) override {
            _Log_("SystemPointerManager::Update");
            for (auto& systemTypeHashKey : _insertionOrder) {
                if (!_enabledSystems[systemTypeHashKey]) continue;
                _Log_("EXECUTE SYSTEM {}", systemTypeHashKey);
                std::pair<void*, double> executeParams(environment, deltaTime);
                _systemPointers[systemTypeHashKey]->Execute(&executeParams);
            }
        }

        bool AddSystemPointer(SystemType systemType, IExecutable* system) override {
            if (_systemPointers.find(SystemTypeToHashKey(systemType)) != _systemPointers.end()) return false;
            _systemPointers[SystemTypeToHashKey(systemType)] = system;
            _enabledSystems[SystemTypeToHashKey(systemType)] = true;
            _insertionOrder.push_back(SystemTypeToHashKey(systemType));
            _Log_("[SystemPointerManager] Added system {}", systemType);
            return true;
        }

        IExecutable* GetSystemPointer(SystemType systemType) override {
            if (_systemPointers.find(SystemTypeToHashKey(systemType)) == _systemPointers.end()) return nullptr;
            return _systemPointers[SystemTypeToHashKey(systemType)];
        }

        bool RemoveSystem(SystemType systemType) override {
            if (_systemPointers.find(SystemTypeToHashKey(systemType)) == _systemPointers.end()) return false;
            _systemPointers.erase(SystemTypeToHashKey(systemType));
            _enabledSystems.erase(SystemTypeToHashKey(systemType));
            _insertionOrder.erase(
                std::remove(_insertionOrder.begin(), _insertionOrder.end(), SystemTypeToHashKey(systemType)),
                _insertionOrder.end()
            );
            _Log_("[SystemPointerManager] Removed system {}", systemType);
            return true;
        }

        bool HasSystem(SystemType systemType) override {
            return _systemPointers.find(SystemTypeToHashKey(systemType)) != _systemPointers.end();
        }

        bool EnableSystem(SystemType systemType) override {
            if (_enabledSystems.find(SystemTypeToHashKey(systemType)) == _enabledSystems.end()) return false;
            _enabledSystems[SystemTypeToHashKey(systemType)] = true;
            _Log_("[SystemPointerManager] Enabled system {}", systemType);
            return true;
        }

        bool DisableSystem(SystemType systemType) override {
            if (_enabledSystems.find(SystemTypeToHashKey(systemType)) == _enabledSystems.end()) return false;
            _enabledSystems[SystemTypeToHashKey(systemType)] = false;
            _Log_("[SystemPointerManager] Disabled system {}", systemType);
            return true;
        }

        bool IsSystemEnabled(SystemType systemType) override {
            if (_enabledSystems.find(SystemTypeToHashKey(systemType)) == _enabledSystems.end()) return false;
            return _enabledSystems[SystemTypeToHashKey(systemType)];
        }
    };
}
