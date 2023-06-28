#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Environment.h"
#include "IEnvironmentManager.h"

namespace Simp1e {

    class EnvironmentManager : public IEnvironmentManager {
        std::unordered_map<std::string, std::unique_ptr<Environment>> _environments;

    public:
        IEnvironment* CreateEnvironment(const char* name) override {
            auto* existing = GetEnvironment(name);
            if (existing) return existing;
            auto* environment   = new Environment();
            _environments[name] = std::unique_ptr<Environment>(environment);
            return environment;
        }

        IEnvironment* GetEnvironment(const char* name) override {
            auto found = _environments.find(name);
            if (found == _environments.end()) return nullptr;
            return found->second.get();
        }

        void DestroyEnvironment(const char* name) override {
            auto found = _environments.find(name);
            if (found == _environments.end()) return;
            _environments.erase(found);
        }

        bool EnvironmentExists(const char* name) override { return _environments.find(name) != _environments.end(); }
    };
}
