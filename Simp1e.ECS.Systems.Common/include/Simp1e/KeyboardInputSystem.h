#pragma once

#include <Simp1e/DefineSystemType.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/IEventManager.h>
// #include <Simp1e/

#include <memory>

namespace Simp1e {

    class KeyboardInputSystem {
        IEventManager*                                        _eventManager;
        std::unique_ptr<IFunctionPointer<void(EventPointer)>> _keyboardEventCallbackFunction;

        void OnKeyboardEvent(EventPointer event) {
            // TODO : implement
        }

    public:
        DEFINE_SYSTEM_TYPE("KeyboardInput")

        KeyboardInputSystem(IEventManager* eventManager)
            : _eventManager(eventManager),
              _keyboardEventCallbackFunction(unique_function_pointer(this, &KeyboardInputSystem::OnKeyboardEvent)) {
            _eventManager->RegisterListener<>(IFunctionPointer<void(EventPointer)> * callback)
        }

        virtual void Update(IEngine* engine, float deltaTime) {
            // TODO : implement
        }
    };
}
