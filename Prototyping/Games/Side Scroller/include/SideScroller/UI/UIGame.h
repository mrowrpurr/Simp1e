#pragma once

#include <QDebug>
#include <memory>

#include "IUIGame.h"
#include "UIFrame.h"
#include "UILevel.h"

namespace SideScroller {

    class UIGame : public IUIGame {
        UIFrame                  _frame{this};
        std::unique_ptr<UILevel> _levelUI;
        double                   _viewportWidth = 600;

    public:
        UIGame(int& argc, char** argv) : IUIGame(argc, argv) {}

        void LoadLevel(const Level& level) override {
            _levelUI = std::make_unique<UILevel>(this);
            _levelUI->LoadLevel(level);

            _frame.SetViewportWidth(_viewportWidth);
            _frame.SetLevelUI(_levelUI.get());
            _frame.setWindowTitle(level.name.c_str());
            qDebug() << "Frame Level size: " << level.width << "x" << level.height;
            _frame.setFixedSize(_viewportWidth + 50, level.height);
            _frame.show();
        }
    };
}
