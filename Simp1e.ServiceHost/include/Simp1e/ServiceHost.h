#pragma once

#include <_Log_.h>

#include <string>
#include <unordered_map>

#include "IServiceHost.h"
#include "ServiceLog.h"

namespace Simp1e {

    class ServiceHost : public IServiceHost {
        std::unordered_map<std::string, void*> _services;

    public:
        bool  HasService(const char* serviceName) override { return _services.find(serviceName) != _services.end(); }
        void* GetService(const char* serviceName) override {
            auto it = _services.find(serviceName);
            if (it == _services.end()) return nullptr;
            return it->second;
        }
        bool RegisterService(const char* serviceName, void* service) override {
            if (HasService(serviceName)) return false;
            ServiceLog("ServiceHost", "Registering service: {}", serviceName);
            _services[serviceName] = service;
            return true;
        }
        bool UnregisterService(const char* serviceName) override {
            if (!HasService(serviceName)) return false;
            ServiceLog("ServiceHost", "Unregistering service: {}", serviceName);
            _services.erase(serviceName);
            return true;
        }
        size_t      GetServiceCount() override { return _services.size(); }
        const char* GetServiceNameByIndex(size_t index) override {
            if (index >= _services.size()) return nullptr;
            auto it = _services.begin();
            std::advance(it, index);
            return it->first.c_str();
        }
        void* GetServiceByIndex(size_t index) override {
            if (index >= _services.size()) return nullptr;
            auto it = _services.begin();
            std::advance(it, index);
            return it->second;
        }
    };
}
