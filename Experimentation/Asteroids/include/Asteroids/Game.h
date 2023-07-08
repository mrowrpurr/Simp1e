#pragma once

// using namespace Simp1e;

#include <Simp1e/LocalEntityEventManager.h>

#include <QApplication>
#include <QLabel>

using namespace Simp1e;

void StaticFn(Entity entity) {}

namespace Asteroids {

    class Game {
    public:
        void MemberFn(Entity entity) {}

        void Run() {
            LocalEntityEventManager eventManager;
            eventManager.RegisterForEntityCreated(StaticFn);
            eventManager.RegisterForEntityCreated({this, &Game::MemberFn});

            //
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
