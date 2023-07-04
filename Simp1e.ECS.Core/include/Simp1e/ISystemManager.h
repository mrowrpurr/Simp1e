#pragma once

#include <function_pointer.h>

#include "IMemoryManagedEngineContainerClass.h"
#include "ISystemGroupManager.h"
#include "SystemGroupType.h"

namespace Simp1e {

    struct IEngine;

    struct ISystemManager : public IMemoryManagedEngineContainerClass {
        virtual ~ISystemManager() = default;

        virtual void Update(IEngine* environment, double deltaTime) = 0;

        virtual bool OwnsSystemGroupMemoryManagement() const = 0;
        bool         ManagesEngineItemMemory() const override { return OwnsSystemGroupMemoryManagement(); }

        virtual VoidPointer* AddGroupPointer(SystemGroupType systemGroupType, VoidPointer systemGroupPointer) = 0;
        virtual ISystemGroupManager* GetGroup(SystemGroupType systemGroupType)                                = 0;
        virtual bool                 RemoveGroup(SystemGroupType systemGroupType)                             = 0;
        virtual bool                 HasGroup(SystemGroupType systemGroupType)                                = 0;

        virtual bool MoveGroupAfterGroup(SystemGroupType systemGroupType, SystemGroupType afterSystemGroupType)   = 0;
        virtual bool MoveGroupBeforeGroup(SystemGroupType systemGroupType, SystemGroupType beforeSystemGroupType) = 0;

        virtual bool SetGroupEnabled(SystemGroupType systemGroupType, bool enabled) = 0;
        virtual bool EnableGroup(SystemGroupType systemGroupType)                   = 0;
        virtual bool DisableGroup(SystemGroupType systemGroupType)                  = 0;
        virtual bool IsSystemGrouEnabled(SystemGroupType systemGroupType)           = 0;

        virtual void ForEachSystemGroupManager(IFunctionPointer* function) = 0;

        template <typename T, typename... Args>
        T* Add(SystemGroupType systemGroupType, Args&&... args) {
            auto* systemGroupManager = new T(std::forward<Args>(args)...);
            auto* voidPointer        = AddGroupPointer(systemGroupType, void_pointer(systemGroupManager));
            if (!OwnsSystemGroupMemoryManagement()) voidPointer->get()->disable_destruct_on_delete();
            return static_cast<T*>(systemGroupManager);
        }
    };
}
