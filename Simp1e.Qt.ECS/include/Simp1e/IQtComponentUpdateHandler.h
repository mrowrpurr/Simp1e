#pragma once

#include <Simp1e/Entity.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/IEntityComponentCollection.h>

namespace Simp1e {

    struct IQtComponentUpdateHandler {
        virtual ~IQtComponentUpdateHandler() = default;

        virtual void Update(
            IEngine* engine, Entity entity, void* component, IEntityComponentCollection* components
        ) = 0;
    };
}
