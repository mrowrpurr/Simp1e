#pragma once

#include <Simp1e/Entity.h>
#include <Simp1e/IEngine.h>

namespace Simp1e {

    struct IQtComponentUpdateHandler {
        virtual ~IQtComponentUpdateHandler() = default;

        virtual void Update(IEngine* engine, Entity entity, void* component) = 0;
    };
}
