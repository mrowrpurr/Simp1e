#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "IEnvironmentManager.h"

namespace Simp1e {

    class EnvironmentManager : public IEnvironmentManager {
        std::unordered_map<std::string, IEnvironment*> _environments;

    public:
        bool RegisterEnvironment(const char* name, IEnvironment* environment) override {
            auto* existing = GetEnvironment(name);
            if (existing) return false;
            _environments[name] = environment;
            return true;
        }

        IEnvironment* GetEnvironment(const char* name) override {
            auto found = _environments.find(name);
            if (found == _environments.end()) return nullptr;
            return found->second;
        }

        bool UnregisterEnvironment(const char* name) override {
            auto found = _environments.find(name);
            if (found == _environments.end()) return false;
            _environments.erase(found);
            return true;
        }

        bool IsEnvironmentRegistered(const char* name) override {
            return _environments.find(name) != _environments.end();
        }
    };
}
