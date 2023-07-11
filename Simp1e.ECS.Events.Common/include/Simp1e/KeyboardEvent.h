#pragma once

#include <vector>

#include "IKeyboardEvent.h"

namespace Simp1e {

    class KeyboardEvent : public IKeyboardEvent {
        KeyboardKey                      _key;
        bool                             _pressed;
        bool                             _repeated;
        std::vector<KeyboardModifierKey> _modifiers;

    public:
        KeyboardEvent(
            KeyboardKey key = KeyboardKey::None, bool pressed = true, bool repeated = false,
            std::vector<KeyboardModifierKey> modifiers = {}
        )
            : _key(key), _pressed(pressed), _repeated(repeated), _modifiers(modifiers) {}

        KeyboardKey key() const override { return _key; }
        bool        pressed() const override { return _pressed; }
        bool        repeated() const override { return _repeated; }

        bool isModifierPressed(KeyboardModifierKey modifierKey) const override {
            for (auto modifier : _modifiers)
                if (modifier == modifierKey) return true;
            return false;
        }

        void ForEachModifier(IFunctionPointer<void(KeyboardModifierKey)>* callback) const override {
            for (auto modifier : _modifiers) callback->invoke(modifier);
        }
    };
}
