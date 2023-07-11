#pragma once

#include <Simp1e/DefineSystemType.h>
#include <Simp1e/IEngine.h>

namespace Simp1e {

    class KeyboardInputSystem {
    public:
        DEFINE_SYSTEM_TYPE("KeyboardInput")

        virtual void Update(IEngine* engine, float deltaTime) {
            // TODO : implement
        }
    };
}
