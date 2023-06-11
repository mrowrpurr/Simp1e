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
            _viewport.setFixedSize(width + viewportPadding, _viewport.height());
        }
    };
}
