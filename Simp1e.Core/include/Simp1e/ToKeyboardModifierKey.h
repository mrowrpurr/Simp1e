#pragma once

#include "KeyboardModifierKey.h"

namespace Simp1e {
    KeyboardModifierKey ToKeyboardKey(int key) { return static_cast<KeyboardModifierKey>(key); }
}
