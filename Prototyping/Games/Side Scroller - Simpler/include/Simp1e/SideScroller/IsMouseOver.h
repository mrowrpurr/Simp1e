#pragma once

#include <Simp1e/ECS/MouseClickEvent.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/SizeComponent.h>

namespace Simp1e::ECS {

    bool IsMouseOver(MouseClickEvent* event, PositionComponent* position, SizeComponent* size) {
        return event->x() >= position->x() && event->x() <= position->x() + size->width() &&
               event->y() >= position->y() && event->y() <= position->y() + size->height();
    }
}
