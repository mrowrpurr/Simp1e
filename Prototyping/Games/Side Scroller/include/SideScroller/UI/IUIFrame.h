#pragma once

#include <QMainWindow>

#include "IUILevel.h"

namespace SideScroller {

    class IUIFrame : public QMainWindow {
    public:
        IUIFrame() : QMainWindow() {}
        virtual ~IUIFrame()              = default;
        virtual void SetLevel(IUILevel*) = 0;
    };
}
