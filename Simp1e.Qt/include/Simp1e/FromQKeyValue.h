#pragma once

#include <Simp1e/KeyboardKey.h>

#include <QKeyCombination>
#include <QKeyEvent>

namespace Simp1e {

    KeyboardKey FromQKeyValue(
        int key, Qt::KeyboardModifiers modifiers = {}, int nativeScanCode = 0, int rightCtrl = 285, int rightShift = 54,
        int rightAlt = 312
    ) {
        // Number keys
        if (modifiers & Qt::KeypadModifier) {
            if (key == Qt::Key_0) return KeyboardKey::NumPad0;
            if (key == Qt::Key_1) return KeyboardKey::NumPad1;
            if (key == Qt::Key_2) return KeyboardKey::NumPad2;
            if (key == Qt::Key_3) return KeyboardKey::NumPad3;
            if (key == Qt::Key_4) return KeyboardKey::NumPad4;
            if (key == Qt::Key_5) return KeyboardKey::NumPad5;
            if (key == Qt::Key_6) return KeyboardKey::NumPad6;
            if (key == Qt::Key_7) return KeyboardKey::NumPad7;
            if (key == Qt::Key_8) return KeyboardKey::NumPad8;
            if (key == Qt::Key_9) return KeyboardKey::NumPad9;
            if (key == Qt::Key_Asterisk) return KeyboardKey::NumPadMultiply;
            if (key == Qt::Key_Plus) return KeyboardKey::NumPadAdd;
            if (key == Qt::Key_Minus) return KeyboardKey::NumPadSubtract;
            if (key == Qt::Key_Slash) return KeyboardKey::NumPadDivide;
        }
        if (key == Qt::Key_0) return KeyboardKey::Key0;
        if (key == Qt::Key_1) return KeyboardKey::Key1;
        if (key == Qt::Key_2) return KeyboardKey::Key2;
        if (key == Qt::Key_3) return KeyboardKey::Key3;
        if (key == Qt::Key_4) return KeyboardKey::Key4;
        if (key == Qt::Key_5) return KeyboardKey::Key5;
        if (key == Qt::Key_6) return KeyboardKey::Key6;
        if (key == Qt::Key_7) return KeyboardKey::Key7;
        if (key == Qt::Key_8) return KeyboardKey::Key8;
        if (key == Qt::Key_9) return KeyboardKey::Key9;

        // Function keys
        if (key == Qt::Key_F1) return KeyboardKey::F1;
        if (key == Qt::Key_F2) return KeyboardKey::F2;
        if (key == Qt::Key_F3) return KeyboardKey::F3;
        if (key == Qt::Key_F4) return KeyboardKey::F4;
        if (key == Qt::Key_F5) return KeyboardKey::F5;
        if (key == Qt::Key_F6) return KeyboardKey::F6;
        if (key == Qt::Key_F7) return KeyboardKey::F7;
        if (key == Qt::Key_F8) return KeyboardKey::F8;
        if (key == Qt::Key_F9) return KeyboardKey::F9;
        if (key == Qt::Key_F10) return KeyboardKey::F10;
        if (key == Qt::Key_F11) return KeyboardKey::F11;
        if (key == Qt::Key_F12) return KeyboardKey::F12;

        // Alphabet keys
        if (key == Qt::Key_A) return KeyboardKey::A;
        if (key == Qt::Key_B) return KeyboardKey::B;
        if (key == Qt::Key_C) return KeyboardKey::C;
        if (key == Qt::Key_D) return KeyboardKey::D;
        if (key == Qt::Key_E) return KeyboardKey::E;
        if (key == Qt::Key_F) return KeyboardKey::F;
        if (key == Qt::Key_G) return KeyboardKey::G;
        if (key == Qt::Key_H) return KeyboardKey::H;
        if (key == Qt::Key_I) return KeyboardKey::I;
        if (key == Qt::Key_J) return KeyboardKey::J;
        if (key == Qt::Key_K) return KeyboardKey::K;
        if (key == Qt::Key_L) return KeyboardKey::L;
        if (key == Qt::Key_M) return KeyboardKey::M;
        if (key == Qt::Key_N) return KeyboardKey::N;
        if (key == Qt::Key_O) return KeyboardKey::O;
        if (key == Qt::Key_P) return KeyboardKey::P;
        if (key == Qt::Key_Q) return KeyboardKey::Q;
        if (key == Qt::Key_R) return KeyboardKey::R;
        if (key == Qt::Key_S) return KeyboardKey::S;
        if (key == Qt::Key_T) return KeyboardKey::T;
        if (key == Qt::Key_U) return KeyboardKey::U;
        if (key == Qt::Key_V) return KeyboardKey::V;
        if (key == Qt::Key_W) return KeyboardKey::W;
        if (key == Qt::Key_X) return KeyboardKey::X;
        if (key == Qt::Key_Y) return KeyboardKey::Y;
        if (key == Qt::Key_Z) return KeyboardKey::Z;

        if (key == Qt::Key_Control) {
            if (nativeScanCode == rightCtrl) return KeyboardKey::RightCtrl;
            return KeyboardKey::LeftCtrl;
        }

        if (key == Qt::Key_Shift) {
            if (nativeScanCode == rightShift) return KeyboardKey::RightShift;
            return KeyboardKey::LeftShift;
        }

        if (key == Qt::Key_Alt) {
            if (nativeScanCode == rightAlt) return KeyboardKey::RightAlt;
            return KeyboardKey::LeftAlt;
        }

        // Other keys
        if (key == Qt::Key_Backspace) return KeyboardKey::Backspace;
        if (key == Qt::Key_Delete) return KeyboardKey::Delete;
        if (key == Qt::Key_Tab) return KeyboardKey::Tab;
        if (key == Qt::Key_Return || key == Qt::Key_Enter) return KeyboardKey::Enter;
        if (key == Qt::Key_Pause) return KeyboardKey::Pause;
        if (key == Qt::Key_CapsLock) return KeyboardKey::CapsLock;
        if (key == Qt::Key_Escape) return KeyboardKey::Escape;
        if (key == Qt::Key_Space) return KeyboardKey::Space;
        if (key == Qt::Key_PageUp) return KeyboardKey::PageUp;
        if (key == Qt::Key_PageDown) return KeyboardKey::PageDown;
        if (key == Qt::Key_End) return KeyboardKey::End;
        if (key == Qt::Key_Home) return KeyboardKey::Home;
        if (key == Qt::Key_Print) return KeyboardKey::PrintScreen;
        if (key == Qt::Key_Insert) return KeyboardKey::Insert;
        if (key == Qt::Key_Up) return KeyboardKey::Up;
        if (key == Qt::Key_Down) return KeyboardKey::Down;
        if (key == Qt::Key_Left) return KeyboardKey::Left;
        if (key == Qt::Key_Right) return KeyboardKey::Right;
        if (key == Qt::Key_NumLock) return KeyboardKey::NumLock;
        if (key == Qt::Key_ScrollLock) return KeyboardKey::ScrollLock;
        if (key == Qt::Key_Shift) return KeyboardKey::LeftShift;
        if (key == Qt::Key_Control) return KeyboardKey::LeftCtrl;
        if (key == Qt::Key_Alt) return KeyboardKey::LeftAlt;
        if (key == Qt::Key_Semicolon) return KeyboardKey::Semicolon;
        if (key == Qt::Key_Plus) return KeyboardKey::Plus;
        if (key == Qt::Key_Comma) return KeyboardKey::Comma;
        if (key == Qt::Key_Minus) return KeyboardKey::Minus;
        if (key == Qt::Key_Period) return KeyboardKey::Period;
        if (key == Qt::Key_Slash) return KeyboardKey::Slash;
        if (key == Qt::Key_AsciiTilde) return KeyboardKey::Tilde;
        if (key == Qt::Key_BracketLeft) return KeyboardKey::LeftBracket;
        if (key == Qt::Key_Backslash) return KeyboardKey::Backslash;
        if (key == Qt::Key_BracketRight) return KeyboardKey::RightBracket;
        if (key == Qt::Key_QuoteDbl) return KeyboardKey::Quote;
        if (key == Qt::Key_Super_L) return KeyboardKey::LeftSuper;
        if (key == Qt::Key_Super_R) return KeyboardKey::RightSuper;
        if (key == Qt::Key_Menu) return KeyboardKey::Menu;

        // Default case
        return KeyboardKey::Unknown;
    }

}
