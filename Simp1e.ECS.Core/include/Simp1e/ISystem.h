#pragma once

#include <void_pointer.h>

namespace Simp1e {

    struct IEngine;

    struct ISystem {
        virtual ~ISystem()                                                  = default;
        virtual VoidPointer* GetSystemPointer() const                       = 0;
        virtual void         Update(IEngine* environment, double deltaTime) = 0;
    };
}
