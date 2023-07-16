#pragma once

#include <Simp1e/IInputManager.h>

#include "LocalKeyboardInputManager.h"

namespace Simp1e {

    class LocalInputManager : public IInputManager {
        LocalKeyboardInputManager _keyboardInputManager;

    public:
        IKeyboardInputManager* GetKeyboardInputManager() override { return &_keyboardInputManager; }
    };
}
