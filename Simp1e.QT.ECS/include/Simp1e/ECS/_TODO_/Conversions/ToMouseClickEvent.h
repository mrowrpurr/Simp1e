#pragma once

#include <Simp1e/ECS/MouseClickEvent.h>

#include <QGraphicsSceneMouseEvent>

#include "FromQtMouseButton.h"

namespace Simp1e::ECS {

    MouseClickEvent ToMouseClickEvent(QGraphicsSceneMouseEvent* event, bool pressed = true) {
        return MouseClickEvent(
            event->scenePos().x(), event->scenePos().y(), FromQtMouseButton(event->button()),
            pressed
        );
    }

    // TODO: same but for QMouseEvent
}
