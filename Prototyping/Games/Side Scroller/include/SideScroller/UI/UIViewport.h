#pragma once

#include <QScrollBar>

#include "IUIViewport.h"

namespace SideScroller {

    class UIViewport : public IUIViewport {
        IUILevel* _levelUI;

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

    protected:
        // Don't scroll the scene with the wheel
        // void wheelEvent(QWheelEvent* event) override {}
    };
}
