#pragma once

#include "EventTypeMacro.h"

namespace Simp1e::ECS {

    class KeyboardEvent {
    public:
        enum class Key {
            Unknown,
            Backspace,
            Tab,
            Enter,
            Shift,
            Ctrl,
            Alt,
            Pause,
            CapsLock,
            Escape,
            Space,
            PageUp,
            PageDown,
            End,
            Home,
            Left,
            Up,
            Right,
            Down,
            PrintScreen,
            Insert,
            Delete,
            Key0,
            Key1,
            Key2,
            Key3,
            Key4,
            Key5,
            Key6,
            Key7,
            Key8,
            Key9,
            KeyA,
            KeyB,
            KeyC,
            KeyD,
            KeyE,
            KeyF,
            KeyG,
            KeyH,
            KeyI,
            KeyJ,
            KeyK,
            KeyL,
            KeyM,
            KeyN,
            KeyO,
            KeyP,
            KeyQ,
            KeyR,
            KeyS,
            KeyT,
            KeyU,
            KeyV,
            KeyW,
            KeyX,
            KeyY,
            KeyZ,
            LeftSuper,
            RightSuper,
            Menu,
            Numpad0,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            NumpadMultiply,
            NumpadAdd,
            NumpadSubtract,
            NumpadDecimal,
            NumpadDivide,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            NumLock,
            ScrollLock,
            LeftShift,
            RightShift,
            LeftCtrl,
            RightCtrl,
            LeftAlt,
            RightAlt,
            Semicolon,
            Apostrophe,
            Hyphen,
            Equal,
            Comma,
            Minus,
            Period,
            Slash,
            Backquote,
            LeftBracket,
            Backslash,
            RightBracket
        };

    private:
        Key  _key;
        bool _pressed;
        bool _repeated;

    public:
        SIMP1E_ECS_EVENT("Keyboard")

        KeyboardEvent(Key key, bool pressed = true, bool repeated = false)
            : _key(key), _pressed(pressed), _repeated(repeated) {}

        virtual ~KeyboardEvent() = default;

        virtual Key  key() const { return _key; }
        virtual bool pressed() const { return _pressed; }
        virtual bool released() const { return !_pressed; }
        virtual bool repeated() const { return _repeated; }
    };
}
