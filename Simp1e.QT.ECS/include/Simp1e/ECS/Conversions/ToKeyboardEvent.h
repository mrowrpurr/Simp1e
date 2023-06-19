#pragma once

#include <Simp1e/ECS/KeyboardEvent.h>

#include <QKeyEvent>

#include "FromQtKey.h"

namespace Simp1e::ECS {

    KeyboardEvent ToKeyboardEvent(QKeyEvent* event, bool pressed = true) {
        std::vector<KeyboardEvent::Key> modifierKeys;
        if (event->modifiers() & Qt::ShiftModifier)
            modifierKeys.push_back(KeyboardEvent::Key::Shift);
        if (event->modifiers() & Qt::ControlModifier)
            modifierKeys.push_back(KeyboardEvent::Key::Ctrl);
        if (event->modifiers() & Qt::AltModifier) modifierKeys.push_back(KeyboardEvent::Key::Alt);
        if (event->modifiers() & Qt::MetaModifier)
            modifierKeys.push_back(KeyboardEvent::Key::Super);
        if (event->modifiers() & Qt::KeypadModifier)
            modifierKeys.push_back(KeyboardEvent::Key::NumLock);
        return KeyboardEvent(FromQtKey(event->key()), pressed, event->isAutoRepeat(), modifierKeys);
    }
}
