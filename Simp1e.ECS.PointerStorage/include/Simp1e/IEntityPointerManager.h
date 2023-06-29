#pragma once

#include <Simp1e/IEntityManager.h>
#include <_Log_.h>

namespace Simp1e {

    struct IEntityPointerManager : public IEntityManager {
        virtual void AddComponentPointer(Entity entity, ComponentType componentType, void* component) = 0;
    };
}
