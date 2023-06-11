#pragma once

#include <QScrollBar>

#include "IUIViewport.h"

namespace SideScroller {

    class UIViewport : public IUIViewport {
    public:
        UIViewport(QWidget* parent = nullptr) : IUIViewport(parent) {
            setDragMode(QGraphicsView::NoDrag);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }

        void SetLevelUI(IUILevel* levelUI) override {
            setScene(levelUI);
            auto& level = levelUI->GetLevel();
            SetSize({level->width, level->height});
            MoveTo({0, 0});
        }

        void MoveTo(Simp1e::UI::UIPoint point) override {
            centerOn(
                point.x() + static_cast<double>(width()) / 2,
                point.y() + static_cast<double>(height()) / 2
            );
        }

        void SetZoom(double zoom) override { setTransform(QTransform::fromScale(zoom, zoom)); }

        void SetSize(Simp1e::UI::UISize size) override {
            setFixedSize(size.width(), size.height());
        }

    protected:
        void wheelEvent(QWheelEvent* event) override {
            // Don't scroll the scene with the wheel
        }
    };
}
