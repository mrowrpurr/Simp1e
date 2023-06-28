#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ECS.h"
#include "IECSManager.h"

namespace Simp1e {

    template <typename EntityManagerT>
    class ECSManager : public IECSManager {
        std::unordered_map<std::string, std::unique_ptr<IECS>> _ecsMap;

    public:
        IECS* CreateECS(const char* environmentName) override {
            auto* existingEnvironment = GetECS(environmentName);
            if (existingEnvironment) return existingEnvironment;
            auto* ecs                = new ECS<EntityManagerT>();
            _ecsMap[environmentName] = std::unique_ptr<IECS>(ecs);
            return ecs;
        }
        IECS* GetECS(const char* environmentName) override {
            if (!HasECS(environmentName)) return nullptr;
            auto found = _ecsMap.find(environmentName);
            if (found == _ecsMap.end()) return nullptr;
            return found->second.get();
        }
        bool HasECS(const char* environmentName) override { return _ecsMap.find(environmentName) != _ecsMap.end(); }
        void DestroyECS(const char* environmentName) override {
            if (!HasECS(environmentName)) return;
            _ecsMap.erase(environmentName);
        }
    };
}
