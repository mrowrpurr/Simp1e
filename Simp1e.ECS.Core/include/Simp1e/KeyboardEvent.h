#pragma once

#include <vector>

#include "IKeyboardEvent.h"

namespace Simp1e {

    class KeyboardEvent : public IKeyboardEvent {
        int  _key;
        bool _pressed;
        bool _repeated;

    public:
        KeyboardEvent(int key, bool pressed = true, bool repeated = false)
            : _key(key), _pressed(pressed), _repeated(repeated) {}

        int  key() const override { return _key; }
        bool pressed() const override { return _pressed; }
        bool repeated() const override { return _repeated; }
    };
}
