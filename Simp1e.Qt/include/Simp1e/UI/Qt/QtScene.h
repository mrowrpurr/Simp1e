#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QKeySequence>
#include <QShortcut>
#include <functional>

namespace Simp1e::UI::Qt {
    class QtScene : public QGraphicsScene {
        QPointF                                   _lastClickedMousePos;
        std::vector<std::function<void(QPointF)>> _leftClickHandlers;
        std::vector<std::function<void(QPointF)>> _rightClickHandlers;
        std::vector<std::function<void(QPointF)>> _middleClickHandlers;
        std::vector<std::function<void(QPointF)>> _onPasteHandlers;

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
        void OnPaste(std::function<void(QPointF)> handler) { _onPasteHandlers.push_back(handler); }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            QGraphicsScene::mousePressEvent(event);
            if (event->button() == ::Qt::MouseButton::LeftButton) {
                _lastClickedMousePos = event->scenePos();
                for (auto& handler : _leftClickHandlers) handler(event->scenePos());
            } else if (event->button() == ::Qt::MouseButton::RightButton) {
                for (auto& handler : _rightClickHandlers) handler(event->scenePos());
            } else if (event->button() == ::Qt::MouseButton::MiddleButton) {
                for (auto& handler : _middleClickHandlers) handler(event->scenePos());
            }
            QGraphicsScene::mousePressEvent(event);
        }

        void keyPressEvent(QKeyEvent* event) override {
            QGraphicsScene::keyPressEvent(event);
            if (event->matches(QKeySequence::Paste))
                for (auto& handler : _onPasteHandlers) handler(_lastClickedMousePos);
        }
    };
}
