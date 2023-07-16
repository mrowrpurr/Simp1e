#pragma once

#include <Simp1e/KeyboardEvent.h>
#include <function_pointer.h>

namespace Simp1e {

    struct IKeyboardInputManager {
        virtual ~IKeyboardInputManager() = default;

        virtual bool IsKeyPressed(int key)              = 0;
        virtual bool IsModifierPressed(int modifierKey) = 0;

        virtual void EmitKeyEvent(KeyboardEvent* event)                                             = 0;
        virtual void SetKeyPressed(int key, bool pressed, bool isAutoRepeat, bool triggerCallbacks) = 0;
        virtual void SetModifierPressed(int modifierKey, bool pressed)                              = 0;

        virtual IFunctionPointer<void(KeyboardEvent*)>* RegisterForKey(
            int key, IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        )                                                                               = 0;
        virtual void UnregisterForKey(int key, IFunctionPointer<void(KeyboardEvent*)>*) = 0;
        virtual void UnregisterForKey(int key)                                          = 0;
        virtual void UnregisterForKey()                                                 = 0;

        virtual IFunctionPointer<void(KeyboardEvent*)>* RegisterForKeyPressed(
            int key, IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        )                                                                                      = 0;
        virtual void UnregisterForKeyPressed(int key, IFunctionPointer<void(KeyboardEvent*)>*) = 0;
        virtual void UnregisterForKeyPressed(int key)                                          = 0;
        virtual void UnregisterForKeyPressed()                                                 = 0;

        virtual IFunctionPointer<void(KeyboardEvent*)>* RegisterForKeyReleased(
            int key, IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        )                                                                                       = 0;
        virtual void UnregisterForKeyReleased(int key, IFunctionPointer<void(KeyboardEvent*)>*) = 0;
        virtual void UnregisterForKeyReleased(int key)                                          = 0;
        virtual void UnregisterForKeyReleased()                                                 = 0;

        virtual IFunctionPointer<void(KeyboardEvent*)>* RegisterForAnyKey(
            IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        )                                                                         = 0;
        virtual void UnregisterForAnyKey(IFunctionPointer<void(KeyboardEvent*)>*) = 0;
        virtual void UnregisterForAnyKey()                                        = 0;

        virtual IFunctionPointer<void(KeyboardEvent*)>* RegisterForAnyKeyPressed(
            IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        )                                                                                = 0;
        virtual void UnregisterForAnyKeyPressed(IFunctionPointer<void(KeyboardEvent*)>*) = 0;
        virtual void UnregisterForAnyKeyPressed()                                        = 0;

        virtual IFunctionPointer<void(KeyboardEvent*)>* RegisterForAnyKeyReleased(
            IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        )                                                                                 = 0;
        virtual void UnregisterForAnyKeyReleased(IFunctionPointer<void(KeyboardEvent*)>*) = 0;
        virtual void UnregisterForAnyKeyReleased()                                        = 0;
    };
}
