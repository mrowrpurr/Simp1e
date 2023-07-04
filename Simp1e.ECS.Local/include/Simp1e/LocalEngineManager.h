#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "IEngineManager.h"

namespace Simp1e {

    class LocalEngineManager : public IEngineManager {
        std::unordered_map<std::string, IEngine*> _environments;

    public:
        bool RegisterEngine(const char* name, IEngine* environment) override {
            auto* existing = GetEngine(name);
            if (existing) return false;
            _environments[name] = environment;
            return true;
        }

        IEngine* GetEngine(const char* name) override {
            auto found = _environments.find(name);
            if (found == _environments.end()) return nullptr;
            return found->second;
        }

        bool UnregisterEngine(const char* name) override {
            auto found = _environments.find(name);
            if (found == _environments.end()) return false;
            _environments.erase(found);
            return true;
        }

        bool IsEngineRegistered(const char* name) override { return _environments.find(name) != _environments.end(); }
    };
}
