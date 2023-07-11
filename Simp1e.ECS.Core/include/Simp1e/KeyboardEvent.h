#pragma once

#include <vector>

#include "IKeyboardEvent.h"

namespace Simp1e {

    class KeyboardEvent : public IKeyboardEvent {
        KeyboardKey _key;
        bool        _pressed;
        bool        _repeated;

    public:
        KeyboardEvent(KeyboardKey key = KeyboardKey::None, bool pressed = true, bool repeated = false)
            : _key(key), _pressed(pressed), _repeated(repeated) {}

        KeyboardKey key() const override { return _key; }
        bool        pressed() const override { return _pressed; }
        bool        repeated() const override { return _repeated; }
    };
}
