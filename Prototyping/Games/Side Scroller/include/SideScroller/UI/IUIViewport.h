#pragma once

#include <Simp1e/UI/UIPoint.h>
#include <Simp1e/UI/UISize.h>

#include <QGraphicsView>

#include "IUILevel.h"

namespace SideScroller {

    class IUIViewport : public QGraphicsView {
    public:
        IUIViewport(QWidget* parent = nullptr) : QGraphicsView(parent) {}
        virtual ~IUIViewport()                         = default;
        virtual void MoveTo(Simp1e::UI::UIPoint point) = 0;
        virtual void SetZoom(double zoom)              = 0;
        virtual void SetSize(Simp1e::UI::UISize size)  = 0;
        virtual void SetLevelUI(IUILevel* levelUI)     = 0;
    };
}
