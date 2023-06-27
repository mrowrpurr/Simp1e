#pragma once

#include <QMainWindow>

#include "IUILevel.h"
#include "IUIViewport.h"

namespace SideScroller {

    class IUIFrame : public QMainWindow {
    public:
        IUIFrame() : QMainWindow() {}
        virtual ~IUIFrame()                           = default;
        virtual void         SetLevelUI(IUILevel*)    = 0;
        virtual void         SetViewportWidth(double) = 0;
        virtual IUIViewport* GetViewport()            = 0;
    };
}
