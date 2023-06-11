#pragma once

#include <QDebug>
#include <QKeyEvent>
#include <QScrollBar>
#include <functional>
#include <vector>

#include "IUIViewport.h"

namespace SideScroller {

    class UIViewport : public IUIViewport {
        IUILevel*                                    _levelUI;
        std::vector<std::function<void(QKeyEvent*)>> _keyPressCallbacks;
        std::vector<std::function<void(QKeyEvent*)>> _keyReleaseCallbacks;

    public:
        UIViewport(QWidget* parent = nullptr) : IUIViewport(parent) {
            // setDragMode(QGraphicsView::NoDrag);
            // setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            // setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }

        void SetLevelUI(IUILevel* levelUI) override {
            _levelUI = levelUI;
            setScene(levelUI);
            auto& level = levelUI->GetLevel();
            SetSize({level->width, level->height});
            CenterOnPlayer();
        }

        void MoveToX(qreal x) { centerOn(x, y()); }
        void MoveTo(Simp1e::UI::UIPoint point) override {
            centerOn(
                point.x() + static_cast<double>(width()) / 2,
                point.y() + static_cast<double>(height()) / 2
            );
        }

        void CenterOnPlayer() {
            auto player = _levelUI->GetPlayer()->GetPlayer();
            MoveToX(player.position.x());
        }

        void SetZoom(double zoom) override { setTransform(QTransform::fromScale(zoom, zoom)); }

        void SetSize(Simp1e::UI::UISize size) override {
            qDebug() << "Set Viewport size: " << size.width() << "x" << size.height();
            setFixedSize(size.width(), size.height());
        }

        void OnKeyPress(std::function<void(QKeyEvent*)> callback) override {
            _keyPressCallbacks.push_back(callback);
        }

        void OnKeyRelease(std::function<void(QKeyEvent*)> callback) override {
            _keyReleaseCallbacks.push_back(callback);
        }

    protected:
        // Don't scroll the scene with the wheel
        // void wheelEvent(QWheelEvent* event) override {}

        void keyPressEvent(QKeyEvent* event) override {
            if (event->isAutoRepeat()) return;
            qDebug() << "Key pressed: " << event->key();
            for (auto& callback : _keyPressCallbacks) callback(event);
        }

        void keyReleaseEvent(QKeyEvent* event) override {
            if (event->isAutoRepeat()) return;
            qDebug() << "Key released: " << event->key();
            for (auto& callback : _keyReleaseCallbacks) callback(event);
        }
    };
}
