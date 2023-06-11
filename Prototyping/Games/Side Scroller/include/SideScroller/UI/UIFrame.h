#pragma once

#include <QDebug>

#include "IUIFrame.h"
#include "IUIGame.h"
#include "UIViewport.h"

namespace SideScroller {

    class UIFrame : public IUIFrame {
        IUIGame*   _game;
        UIViewport _viewport{this};
        double     viewportPadding = 5;

    public:
        UIFrame(IUIGame* game) : IUIFrame(), _game(game) {}

        void SetLevelUI(IUILevel* levelUI) override { _viewport.SetLevelUI(levelUI); }

        void SetViewportWidth(double width) override {
            qDebug() << "Set Frame Viewport size: " << width << "x" << _viewport.height();
            _viewport.SetSize({width + viewportPadding, static_cast<double>(_viewport.height())});
        }
    };
}
