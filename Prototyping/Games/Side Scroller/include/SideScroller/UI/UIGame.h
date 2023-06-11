#pragma once

#include <memory>

#include "IUIGame.h"
#include "UIFrame.h"
#include "UILevel.h"

namespace SideScroller {

    class UIGame : public IUIGame {
        UIFrame                  _frame{this};
        std::unique_ptr<UILevel> _level;

    public:
        UIGame(int& argc, char** argv) : IUIGame(argc, argv) {}

        void LoadLevel(const Level& level) override {
            auto levelUI = new UILevel{this};
            levelUI->LoadLevel(level);
            _level.reset(levelUI);
            _frame.SetLevel(levelUI);
            _frame.setWindowTitle(level.name.c_str());
        }
    };
}
