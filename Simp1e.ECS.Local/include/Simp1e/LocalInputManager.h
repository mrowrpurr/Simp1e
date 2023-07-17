#pragma once

#include <Simp1e/InputManager.h>

#include "LocalKeyboardInputManager.h"
#include "LocalSensorInputManager.h"

namespace Simp1e {

    class LocalInputManager : public InputManager<LocalKeyboardInputManager, LocalSensorInputManager> {};
}
