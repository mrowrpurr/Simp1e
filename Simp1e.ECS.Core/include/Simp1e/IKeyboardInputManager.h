#pragma once

#include <Simp1e/IKeyboardEvent.h>
#include <function_pointer.h>

namespace Simp1e {

    struct IKeyboardInputManager {
        virtual ~IKeyboardInputManager() = default;

        virtual bool IsKeyPressed(int key)              = 0;
        virtual bool IsModifierPressed(int modifierKey) = 0;

        virtual void EmitKeyboardEvent(IKeyboardEvent* event)                                       = 0;
        virtual void SetKeyPressed(int key, bool pressed, bool isAutoRepeat, bool triggerCallbacks) = 0;
        virtual void SetModifierPressed(int modifierKey, bool pressed)                              = 0;

        virtual IFunctionPointer<void(IKeyboardEvent*)>* RegisterForKey(
            int key, IFunctionPointer<void(IKeyboardEvent*)>* callbackFunctionPointer
        )                                                                                = 0;
        virtual void UnregisterForKey(int key, IFunctionPointer<void(IKeyboardEvent*)>*) = 0;
        virtual void UnregisterForKey(int key)                                           = 0;
        virtual void UnregisterForKey()                                                  = 0;

        virtual IFunctionPointer<void(IKeyboardEvent*)>* RegisterForKeyPressed(
            int key, IFunctionPointer<void(IKeyboardEvent*)>* callbackFunctionPointer
        )                                                                                       = 0;
        virtual void UnregisterForKeyPressed(int key, IFunctionPointer<void(IKeyboardEvent*)>*) = 0;
        virtual void UnregisterForKeyPressed(int key)                                           = 0;
        virtual void UnregisterForKeyPressed()                                                  = 0;

        virtual IFunctionPointer<void(IKeyboardEvent*)>* RegisterForKeyReleased(
            int key, IFunctionPointer<void(IKeyboardEvent*)>* callbackFunctionPointer
        )                                                                                        = 0;
        virtual void UnregisterForKeyReleased(int key, IFunctionPointer<void(IKeyboardEvent*)>*) = 0;
        virtual void UnregisterForKeyReleased(int key)                                           = 0;
        virtual void UnregisterForKeyReleased()                                                  = 0;

        virtual IFunctionPointer<void(IKeyboardEvent*)>* RegisterForAnyKey(
            IFunctionPointer<void(IKeyboardEvent*)>* callbackFunctionPointer
        )                                                                          = 0;
        virtual void UnregisterForAnyKey(IFunctionPointer<void(IKeyboardEvent*)>*) = 0;
        virtual void UnregisterForAnyKey()                                         = 0;

        virtual IFunctionPointer<void(IKeyboardEvent*)>* RegisterForAnyKeyPressed(
            IFunctionPointer<void(IKeyboardEvent*)>* callbackFunctionPointer
        )                                                                                 = 0;
        virtual void UnregisterForAnyKeyPressed(IFunctionPointer<void(IKeyboardEvent*)>*) = 0;
        virtual void UnregisterForAnyKeyPressed()                                         = 0;

        virtual IFunctionPointer<void(IKeyboardEvent*)>* RegisterForAnyKeyReleased(
            IFunctionPointer<void(IKeyboardEvent*)>* callbackFunctionPointer
        )                                                                                  = 0;
        virtual void UnregisterForAnyKeyReleased(IFunctionPointer<void(IKeyboardEvent*)>*) = 0;
        virtual void UnregisterForAnyKeyReleased()                                         = 0;
    };
}
