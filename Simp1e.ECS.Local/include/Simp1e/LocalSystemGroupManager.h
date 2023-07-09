#pragma once

#include <Simp1e/ISystemGroupManager.h>
#include <_Log_.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "LocalSystemGroup.h"

namespace Simp1e {

    class LocalSystemGroupManager : public ISystemGroupManager {
        std::unordered_map<SystemGroupType, std::unique_ptr<ISystemGroup>> _systemGroups;
        std::unordered_map<SystemGroupType, bool>                          _enabledSystemGroups;
        std::vector<SystemGroupType>                                       _systemGroupOrder;

    public:
        ISystemGroup* AddGroup(SystemGroupType groupName) override {
            auto* systemGroup               = new LocalSystemGroup();
            _systemGroups[groupName]        = std::unique_ptr<ISystemGroup>(systemGroup);
            _enabledSystemGroups[groupName] = true;
            _systemGroupOrder.push_back(groupName);
            return systemGroup;
        }

        void Update(IEngine* environment, double deltaTime) override {
            for (auto& systemGroupType : _systemGroupOrder)
                if (_enabledSystemGroups[systemGroupType])
                    _systemGroups[systemGroupType]->Update(environment, deltaTime);
        }

        ISystemGroup* GetGroup(SystemGroupType systemGroupType) override {
            auto found = _systemGroups.find(systemGroupType);
            if (found == _systemGroups.end()) return nullptr;
            return found->second.get();
        }
        bool RemoveGroup(SystemGroupType systemGroupType) override { return _systemGroups.erase(systemGroupType) > 0; }
        bool HasGroup(SystemGroupType systemGroupType) override {
            return _systemGroups.find(systemGroupType) != _systemGroups.end();
        }

        bool MoveGroupAfterGroup(SystemGroupType systemGroupType, SystemGroupType afterSystemGroupType) override {
            auto it = std::find(_systemGroupOrder.begin(), _systemGroupOrder.end(), systemGroupType);
            if (it == _systemGroupOrder.end()) return false;
            auto itAfter = std::find(_systemGroupOrder.begin(), _systemGroupOrder.end(), afterSystemGroupType);
            if (itAfter == _systemGroupOrder.end()) return false;
            _systemGroupOrder.erase(it);
            _systemGroupOrder.insert(itAfter + 1, systemGroupType);
            return true;
        }
        bool MoveGroupBeforeGroup(SystemGroupType systemGroupType, SystemGroupType beforeSystemGroupType) override {
            auto it = std::find(_systemGroupOrder.begin(), _systemGroupOrder.end(), systemGroupType);
            if (it == _systemGroupOrder.end()) return false;
            auto itBefore = std::find(_systemGroupOrder.begin(), _systemGroupOrder.end(), beforeSystemGroupType);
            if (itBefore == _systemGroupOrder.end()) return false;
            _systemGroupOrder.erase(it);
            _systemGroupOrder.insert(itBefore, systemGroupType);
            return true;
        }

        bool SetGroupEnabled(SystemGroupType systemGroupType, bool enabled) override {
            _enabledSystemGroups[systemGroupType] = enabled;
            return true;
        }
        bool EnableGroup(SystemGroupType systemGroupType) override {
            _enabledSystemGroups[systemGroupType] = true;
            return true;
        }
        bool DisableGroup(SystemGroupType systemGroupType) override {
            _enabledSystemGroups[systemGroupType] = false;
            return true;
        }
        bool IsSystemGrouEnabled(SystemGroupType systemGroupType) override {
            return _enabledSystemGroups[systemGroupType];
        }

        void ForEachGroup(IFunctionPointer<void(ISystemGroup*)>* function) override {
            for (auto& systemGroupType : _systemGroupOrder) function->invoke(_systemGroups[systemGroupType].get());
        }
    };
}
