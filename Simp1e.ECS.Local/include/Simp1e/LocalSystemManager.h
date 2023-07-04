#pragma once

#include <Simp1e/ISystemManager.h>

#include <unordered_map>
#include <vector>

namespace Simp1e {

    class LocalSystemManager : public ISystemManager {
        std::unordered_map<SystemGroupType, VoidPointer> _systemGroups;
        std::unordered_map<SystemGroupType, bool>        _enabledSystemGroups;
        std::vector<SystemGroupType>                     _systemGroupOrder;

    public:
        bool OwnsSystemGroupMemoryManagement() const override { return true; }

        VoidPointer* AddGroupPointer(SystemGroupType systemGroupType, VoidPointer systemGroupPointer) override {
            auto insertResult = _systemGroups.insert({systemGroupType, std::move(systemGroupPointer)});
            if (!insertResult.second) return nullptr;
            if (!OwnsSystemGroupMemoryManagement()) insertResult.first->second.get()->disable_destruct_on_delete();
            _enabledSystemGroups[systemGroupType] = true;
            _systemGroupOrder.push_back(systemGroupType);
            return &insertResult.first->second;
        }

        void Update(IEngine* environment, double deltaTime) override {
            for (auto& systemGroupType : _systemGroupOrder)
                if (_enabledSystemGroups[systemGroupType])
                    _systemGroups[systemGroupType]->as<ISystemGroupManager>()->Update(environment, deltaTime);
        }

        ISystemGroupManager* GetGroup(SystemGroupType systemGroupType) override {
            auto found = _systemGroups.find(systemGroupType);
            if (found == _systemGroups.end()) return nullptr;
            return found->second.get()->as<ISystemGroupManager>();
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

        void ForEachSystemGroupManager(IFunctionPointer* function) override {
            for (auto& systemGroupType : _systemGroupOrder)
                function_pointer::invoke(function, _systemGroups[systemGroupType].get());
        }
    };
}
