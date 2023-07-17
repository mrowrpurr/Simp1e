#pragma once

#include "IKeyboardInputManager.h"
#include "ISensorInputManager.h"

namespace Simp1e {

    struct IInputManager {
        virtual ~IInputManager() = default;

        virtual IKeyboardInputManager* GetKeyboard() = 0;
        virtual ISensorInputManager*   GetSensors()  = 0;

        // Do something super simple to start with.

        // This should provide 2x functions:
        // 1. Get the current state of some key, key group, control group, etc
        // 2. Register/Unregister events for those states when triggered

        // For the first version, just make it possible
        // to get the status of individual keys

        // And then the status of touch points
    };
}
