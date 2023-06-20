#pragma once

#include <Simp1e/ECS/Conversions/ToKeyboardEvent.h>
#include <Simp1e/ECS/EventManager.h>
#include <Simp1e/ECS/KeyboardEvent.h>
#include <Simp1e/ECS/ResizeEvent.h>

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QKeyEvent>

namespace Simp1e::ECS {

    class SideScrollerGraphicsView : public QGraphicsView {
        EventManager& _eventManager;

    public:
        SideScrollerGraphicsView(EventManager& eventManager) : _eventManager(eventManager) {
            setRenderHint(QPainter::Antialiasing);  // Enable smooth rendering.
        }

    protected:
        void keyPressEvent(QKeyEvent* event) override {
            _eventManager.Emit<KeyboardEvent>(ToKeyboardEvent(event, true));
            QGraphicsView::keyPressEvent(event);
        }

        void keyReleaseEvent(QKeyEvent* event) override {
            _eventManager.Emit<KeyboardEvent>(ToKeyboardEvent(event, false));
            QGraphicsView::keyReleaseEvent(event);
        }

        void resizeEvent(QResizeEvent* event) override {
            auto topLeft = mapToScene(0, 0);
            _eventManager.Emit<ResizeEvent>({
                {static_cast<sreal>(topLeft.x()),           static_cast<sreal>(topLeft.y())           },
                {static_cast<sreal>(event->size().width()), static_cast<sreal>(event->size().height())}
            });
            for (auto item : scene()->items()) item->update();
        }
    };
}