#pragma once

#include <Simp1e/FromKeyboardKey.h>
#include <Simp1e/KeyboardEvent.h>

#include <QKeyEvent>

#include "FromQKeyValue.h"

namespace Simp1e {
    KeyboardEvent FromQKeyEvent(QKeyEvent* event) {
        auto key      = FromKeyboardKey(FromQKeyValue(event->key()));
        auto pressed  = event->type() == QEvent::Type::KeyPress;
        auto repeated = event->isAutoRepeat();
        return {key, pressed, repeated};
    }
}
