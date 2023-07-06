#pragma once

#include <Simp1e/DefaultSystemGroupTypes.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/LocalSystemGroupManager.h>
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
            // Add the QtRenderGroup (before the default PresentationGroup)
            auto* qtGroup =
                _engine->GetSystemManager()->Add<LocalSystemGroupManager>(QtSystemGroupTypes::QtRenderGroup);
            _engine->GetSystemManager()->MoveGroupBeforeGroup(
                QtSystemGroupTypes::QtRenderGroup, DefaultSystemGroupTypes::PresentationGroup
            );
            qtGroup->Add<QtGuiSystem>(engine);
        }

        ISystemGroupManager* GetQtRenderGroup() const {
            return _engine->GetSystemManager()->GetGroup(QtSystemGroupTypes::QtRenderGroup);
        }

        void Run() {
            _gameLoop->Run();
            _app.exec();
        }
    };
}