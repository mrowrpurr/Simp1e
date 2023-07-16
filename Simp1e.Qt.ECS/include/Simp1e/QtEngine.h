#pragma once

#include <Simp1e/DefaultSystemGroupTypes.h>
#include <Simp1e/FromQKeyEvent.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/IKeyboardEvent.h>
#include <Simp1e/LocalSystemGroup.h>
#include <Simp1e/QSimp1eApp.h>
#include <Simp1e/QtGameLoop.h>
#include <Simp1e/QtGuiSystem.h>
#include <Simp1e/QtSystemGroupTypes.h>
#include <_Log_.h>

#include <QLabel>
#include <memory>

namespace Simp1e {

    class QtEngine {
        QSimp1eApp                   _app;
        IEngine*                     _engine;
        std::unique_ptr<QtGameLoop>  _gameLoop;
        std::unique_ptr<QtGuiSystem> _guiSystem;

        void SetupSystems() {
            auto* qtGroup = _engine->GetSystemGroups()->AddGroup(QtSystemGroupTypes::QtRenderGroup);
            _engine->GetSystemGroups()->MoveGroupBeforeGroup(
                QtSystemGroupTypes::QtRenderGroup, DefaultSystemGroupTypes::PresentationGroup
            );
            qtGroup->AddSystemPointer<QtGuiSystem>(_guiSystem.get());
        }

        EventResult::Value OnKeyEvent(QKeyEvent* event) {
            auto keyboardEvent = FromQKeyEvent(event);
            _Log_(
                "Qt OnKeyEvent key:{} pressed:{} repeat:{}", keyboardEvent.key(), keyboardEvent.pressed(),
                keyboardEvent.repeated()
            );
            _engine->GetInput()->GetKeyboardInputManager()->EmitKeyEvent(&keyboardEvent);
            return EventResult::Continue;
        }

        void SetupInputEventEmitting() { _app.OnKeyEvent({this, &QtEngine::OnKeyEvent}); }

    public:
        // TODO: Find a way to refactor into something that actually really makes proper use of Systems<> maybe?
        QtEngine(IEngine* engine)
            : _engine(engine),
              _gameLoop(std::make_unique<QtGameLoop>(engine)),
              _guiSystem(std::make_unique<QtGuiSystem>(engine)) {
            SetupSystems();
            SetupInputEventEmitting();
        }

        ISystemGroup* GetQtRenderGroup() const {
            return _engine->GetSystemGroups()->GetGroup(QtSystemGroupTypes::QtRenderGroup);
        }

        QtGuiSystem* GetQtGuiSystem() const { return _guiSystem.get(); }

        void Run() {
            _gameLoop->Run();
            _app.exec();
        }
    };
}
