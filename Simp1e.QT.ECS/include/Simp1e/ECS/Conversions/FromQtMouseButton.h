#pragma once

#include <Simp1e/ECS/MouseClickEvent.h>

#include <QMouseEvent>

namespace Simp1e::ECS {

    MouseClickEvent::Button FromQtMouseButton(Qt::MouseButton button) {
        switch (button) {
            case Qt::MouseButton::LeftButton:
                return MouseClickEvent::Button::Left;
            case Qt::MouseButton::RightButton:
                return MouseClickEvent::Button::Right;
            case Qt::MouseButton::MiddleButton:
                return MouseClickEvent::Button::Middle;
            default:
                return MouseClickEvent::Button::Left;
        }
    }
}
