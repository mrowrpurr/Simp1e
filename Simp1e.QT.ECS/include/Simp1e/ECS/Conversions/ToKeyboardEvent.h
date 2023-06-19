#pragma once

#include <Simp1e/ECS/KeyboardEvent.h>

#include <QKeyEvent>

#include "FromQtKey.h"


namespace Simp1e::ECS {

    KeyboardEvent ToKeyboardEvent(QKeyEvent* event, bool pressed = true) {
        return KeyboardEvent(FromQtKey(event->key()), pressed, event->isAutoRepeat());
    }
}
