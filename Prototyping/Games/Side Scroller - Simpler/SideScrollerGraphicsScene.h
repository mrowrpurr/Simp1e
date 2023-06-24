#pragma once

// #include <Simp1e/ECS/Conversions/ToMouseClickEvent.h>
// #include <Simp1e/ECS/EventManager.h>
// #include <Simp1e/ECS/MouseClickEvent.h>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace Simp1e::ECS {

    class SideScrollerGraphicsScene : public QGraphicsScene {
        // EventManager& eventManager;

    public:
        // SideScrollerGraphicsScene(EventManager& eventManager, QObject* parent = nullptr)
        //     : QGraphicsScene(parent), eventManager(eventManager) {}

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            // eventManager.Emit<MouseClickEvent>(ToMouseClickEvent(event, true));
        }
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
            // eventManager.Emit<MouseClickEvent>(ToMouseClickEvent(event, false));
        }
    };
}
