#pragma once

#include <Simp1e/DefineSystemType.h>
#include <Simp1e/LocalEntityEventManager.h>
#include <Simp1e/LocalEntityManager.h>
#include <Simp1e/LocalSystemGroup.h>
#include <Simp1e/LocalSystemGroupManager.h>
#include <Simp1e/PositionComponent.h>

#include <QApplication>
#include <QLabel>

using namespace Simp1e;

namespace Asteroids {

    class Game {
    public:
        void Run() {
            LocalEntityEventManager eventManager;

            LocalEntityManager entityManager;
            auto               entity1  = entityManager.CreateEntity();
            auto*              position = entityManager.AddComponent<PositionComponent>(entity1, 1, 2, 3);

            LocalSystemGroup systemManager;

            LocalSystemGroupManager systemGroups;

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
