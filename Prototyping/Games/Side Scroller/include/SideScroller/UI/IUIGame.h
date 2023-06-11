#pragma once

#include <QApplication>

#include "../Game/Level.h"

namespace SideScroller {

    class IUIGame : public QApplication {
    public:
        IUIGame(int& argc, char** argv) : QApplication(argc, argv) { setStyle("fusion"); }
        virtual ~IUIGame() = default;

        virtual void LoadLevel(const Level& level) = 0;
        int          Run() { return exec(); }
    };
}
