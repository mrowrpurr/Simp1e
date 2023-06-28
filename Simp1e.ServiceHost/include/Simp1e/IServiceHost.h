#pragma once

namespace Simp1e {

    struct IServiceHost {
        virtual ~IServiceHost()                                                     = default;
        virtual bool        HasService(const char* serviceName)                     = 0;
        virtual void*       GetService(const char* serviceName)                     = 0;
        virtual bool        RegisterService(const char* serviceName, void* service) = 0;
        virtual bool        UnregisterService(const char* serviceName)              = 0;
        virtual size_t      GetServiceCount()                                       = 0;
        virtual const char* GetServiceNameByIndex(size_t index)                     = 0;
        virtual void*       GetServiceByIndex(size_t index)                         = 0;

        template <typename T>
        bool HasService() {
            return HasService(T::GetServiceName());
        }

        template <typename T>
        T* GetService() {
            auto* service = GetService(T::GetServiceName());
            if (!service) return nullptr;
            return static_cast<T*>(service);
        }

        template <typename T>
        bool RegisterService(T* service) {
            return RegisterService(T::GetServiceName(), service);
        }

        template <typename T>
        bool UnregisterService() {
            return UnregisterService(T::GetServiceName());
        }
    };
}
