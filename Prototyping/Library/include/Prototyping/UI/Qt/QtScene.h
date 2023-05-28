#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <functional>

namespace Prototyping::UI::Qt {
    class QtScene : public QGraphicsScene {
        std::vector<std::function<void(QPointF)>> _leftClickHandlers;
        std::vector<std::function<void(QPointF)>> _rightClickHandlers;
        std::vector<std::function<void(QPointF)>> _middleClickHandlers;

    public:
        QtScene() : QGraphicsScene() {}

        void OnLeftClick(std::function<void(QPointF)> handler) {
            _leftClickHandlers.push_back(handler);
        }
        void OnRightClick(std::function<void(QPointF)> handler) {
            _rightClickHandlers.push_back(handler);
        }
        void OnMiddleClick(std::function<void(QPointF)> handler) {
            _middleClickHandlers.push_back(handler);
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            QGraphicsScene::mousePressEvent(event);
            if (event->button() == ::Qt::MouseButton::LeftButton)
                for (auto& handler : _leftClickHandlers) handler(event->scenePos());
            else if (event->button() == ::Qt::MouseButton::RightButton)
                for (auto& handler : _rightClickHandlers) handler(event->scenePos());
            else if (event->button() == ::Qt::MouseButton::MiddleButton)
                for (auto& handler : _middleClickHandlers) handler(event->scenePos());
            QGraphicsScene::mousePressEvent(event);
        }
    };
}
