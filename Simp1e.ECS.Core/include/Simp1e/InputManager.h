#pragma once

#include "IInputManager.h"
#include "IKeyboardInputManager.h"
#include "ISensorInputManager.h"

namespace Simp1e {

    template <typename KeyboardInputManagerT, typename SensorInputManagerT>
    class InputManager : public IInputManager {
        KeyboardInputManagerT _keyboardInputManager;
        SensorInputManagerT   _sensorInputManager;

    public:
        IKeyboardInputManager* GetKeyboard() override { return &_keyboardInputManager; }
        ISensorInputManager*   GetSensors() override { return &_sensorInputManager; }

        KeyboardInputManagerT& Keyboard() { return _keyboardInputManager; }
        SensorInputManagerT&   Sensors() { return _sensorInputManager; }
    };
}
