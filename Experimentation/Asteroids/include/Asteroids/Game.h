#pragma once

// using namespace Simp1e;

#include <QApplication>
#include <QLabel>

namespace Asteroids {

    class Game {
    public:
        void Run() {
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
