#pragma once

namespace Simp1e {

    struct IEngine;

    struct ISystem {
        virtual ~ISystem()                                           = default;
        virtual void* GetSystemPointer() const                       = 0;
        virtual void  Update(IEngine* environment, double deltaTime) = 0;
    };
}
