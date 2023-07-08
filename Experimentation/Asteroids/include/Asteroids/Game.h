#pragma once

#include <Simp1e/DefineSystemType.h>
#include <Simp1e/LocalEntityEventManager.h>
#include <Simp1e/LocalEntityManager.h>
#include <Simp1e/LocalSystemGroupManager.h>
#include <Simp1e/PositionComponent.h>

#include <QApplication>
#include <QLabel>

using namespace Simp1e;

void StaticFn(Entity entity) {}

struct SomeSystem {
    DEFINE_SYSTEM_TYPE("SomeSystem")
    void Update(IEngine*, double) { _Log_("SomeSystem Update called"); }
};

namespace Asteroids {

    class Game {
    public:
        void MemberFn(Entity entity) {}

        void EachSystem(SystemType type, ISystem* system) {
            //
            _Log_("for each system...{}", type);
        }

        void Run() {
            LocalEntityEventManager eventManager;
            eventManager.RegisterForEntityCreated(StaticFn);
            eventManager.RegisterForEntityCreated({this, &Game::MemberFn});

            LocalEntityManager entityManager;
            auto               entity1  = entityManager.CreateEntity();
            auto*              position = entityManager.AddComponent<PositionComponent>(entity1, 1, 2, 3);

            LocalSystemGroupManager systemManager;
            systemManager.AddSystem<SomeSystem>();
            systemManager.ForEachSystem({this, &Game::EachSystem});

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
