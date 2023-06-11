#pragma once

#include "IUIFrame.h"
#include "IUIGame.h"
#include "UIViewport.h"

namespace SideScroller {

    class UIFrame : public IUIFrame {
        IUIGame*   _game;
        UIViewport _viewport{this};

    public:
        UIFrame(IUIGame* game) : IUIFrame(), _game(game) {
            setWindowTitle("Side Scroller");
            show();
        }

        void SetLevel(IUILevel* level) override {
            _viewport.setScene(level);
            _viewport.show();
        }
    };
}
