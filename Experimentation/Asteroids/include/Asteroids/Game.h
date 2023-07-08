#pragma once

// #include <Simp1e/LocalEntityEventManager.h>
// #include <Simp1e/LocalEntityManager.h>
#include <Simp1e/Entity.h>
// #include <Simp1e/PositionComponent.h>

#include <QApplication>
#include <QLabel>

using namespace Simp1e;

void StaticFn(Entity entity) {}

namespace Asteroids {

    class Game {
    public:
        void MemberFn(Entity entity) {}

        void Run() {
            // LocalEntityEventManager eventManager;
            // eventManager.RegisterForEntityCreated(StaticFn);
            // eventManager.RegisterForEntityCreated({this, &Game::MemberFn});

            // LocalEntityManager entityManager;
            // auto               entity1  = entityManager.CreateEntity();
            // auto*              position = entityManager.AddComponent<PositionComponent>(entity1, 1, 2, 3);

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
