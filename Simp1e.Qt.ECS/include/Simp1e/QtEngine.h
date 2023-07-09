#pragma once

#include <Simp1e/DefaultSystemGroupTypes.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/LocalSystemGroup.h>
#include <Simp1e/QSimp1eApp.h>
#include <Simp1e/QtGameLoop.h>
#include <Simp1e/QtGuiSystem.h>
#include <Simp1e/QtSystemGroupTypes.h>

#include <memory>

namespace Simp1e {

    class QtEngine {
        QSimp1eApp                  _app;
        IEngine*                    _engine;
        std::unique_ptr<QtGameLoop> _gameLoop;

    public:
        QtEngine(IEngine* engine) : _engine(engine), _gameLoop(std::make_unique<QtGameLoop>(engine)) {
            auto* qtGroup = _engine->GetSystemGroups()->AddGroup(QtSystemGroupTypes::QtRenderGroup);
            _engine->GetSystemGroups()->MoveGroupBeforeGroup(
                QtSystemGroupTypes::QtRenderGroup, DefaultSystemGroupTypes::PresentationGroup
            );
            qtGroup->AddSystem<QtGuiSystem>(engine);
        }

        ISystemGroup* GetQtRenderGroup() const {
            return _engine->GetSystemGroups()->GetGroup(QtSystemGroupTypes::QtRenderGroup);
        }

        void Run() {
            _gameLoop->Run();
            _app.exec();
        }
    };
}
