#pragma once

#include <Simp1e/KeyboardEvent.h>

#include <QKeyEvent>

namespace Simp1e {
    KeyboardEvent FromQKeyEvent(QKeyEvent* event) {
        KeyboardEvent keyboardEvent{69};
        return keyboardEvent;
    }
}
