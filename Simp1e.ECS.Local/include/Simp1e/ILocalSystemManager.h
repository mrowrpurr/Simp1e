#pragma once

#include <Simp1e/ISystemManager.h>

#include <memory>

namespace Simp1e {

    struct ILocalSystemManager : public ISystemManager {
        virtual ISystemGroupManager* AddGroup(
            SystemGroupType systemGroupType, std::unique_ptr<ISystemGroupManager> systemGroupManager
        ) = 0;
    };

    template <typename T, typename... Args>
    T* Add(SystemGroupType systemGroupType, Args&&... args) {
        auto* systemGroupManager = new T(std::forward<Args>(args)...);
        AddSystemGroupManager(systemGroupType, std::unique_ptr<ISystemGroupManager>(systemGroupManager));
        return static_cast<T*>(systemGroupManager);
    }
}
