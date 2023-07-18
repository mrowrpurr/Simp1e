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
            _engine->GetInput()->GetKeyboard()->SetKeyPressed(
                keyboardEvent.key(), keyboardEvent.pressed(), keyboardEvent.repeated(), true
            );
            return EventResult::Continue;
        }

        void SetupEvents() { _app.OnKeyEvent({this, &QtEngine::OnKeyEvent}); }

    public:
        // TODO: Find a way to refactor into something that actually really makes proper use of Systems<> maybe?
        QtEngine(IEngine* engine)
            : _engine(engine),
              _gameLoop(std::make_unique<QtGameLoop>(engine)),
              _guiSystem(std::make_unique<QtGuiSystem>(engine)) {
            SetupSystems();
            SetupEvents();
        }

        ISystemGroup* GetQtRenderGroup() const {
            return _engine->GetSystemGroups()->GetGroup(QtSystemGroupTypes::QtRenderGroup);
        }

        QtGuiSystem* GetQtGuiSystem() const { return _guiSystem.get(); }

        void UseAccelerometer(bool useAccelerometer = true) {
            if (useAccelerometer) _app.StartAccelerometer();
            else _app.StopAccelerometer();
        }

        PositionF GetAccelerometerReading() { return _app.GetAccelerometerReading(); }

        void Run() {
            _gameLoop->Run();
            _app.exec();
        }
    };
}
