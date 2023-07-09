#pragma once

#include <Simp1e/LocalEngine.h>
#include <Simp1e/PositionComponent.h>

#include <QApplication>
#include <QLabel>

using namespace Simp1e;

namespace Asteroids {

    class Game {
    public:
        void Run() {
            LocalEngine engine;

            int          argc = 0;
            char**       argv = nullptr;
            QApplication app(argc, argv);
            QLabel       window("Hello World!");
            window.setWindowTitle("The title here");
            window.show();
            app.exec();
        }
    };
}
