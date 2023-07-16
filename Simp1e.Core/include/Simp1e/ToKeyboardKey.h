#pragma once

#include "KeyboardKey.h"

namespace Simp1e {
    KeyboardKey ToKeyboardKey(int key) { return static_cast<KeyboardKey>(key); }
}
