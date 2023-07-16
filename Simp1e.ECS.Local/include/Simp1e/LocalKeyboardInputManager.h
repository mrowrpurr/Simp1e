#pragma once

#include <Simp1e/IKeyboardInputManager.h>

#include <unordered_map>
#include <unordered_set>

namespace Simp1e {

    class LocalKeyboardInputManager : public IKeyboardInputManager {
        std::unordered_map<int, bool>                                    _currentlyPressedKeys;
        std::unordered_map<int, bool>                                    _currentlyPressedModifiers;
        std::unordered_map<int, IFunctionPointer<void(KeyboardEvent*)>*> _keyCallbacks;
        std::unordered_map<int, IFunctionPointer<void(KeyboardEvent*)>*> _keyPressedCallbacks;
        std::unordered_map<int, IFunctionPointer<void(KeyboardEvent*)>*> _keyReleasedCallbacks;
        std::unordered_set<IFunctionPointer<void(KeyboardEvent*)>*>      _anyKeyCallbacks;
        std::unordered_set<IFunctionPointer<void(KeyboardEvent*)>*>      _anyKeyPressedCallbacks;
        std::unordered_set<IFunctionPointer<void(KeyboardEvent*)>*>      _anyKeyReleasedCallbacks;

        inline void TriggerAnyKeyCallbacks(KeyboardEvent* event) {
            for (auto& callback : _anyKeyCallbacks) callback->invoke(event);
        }

        inline void TriggerCallbacksForKey(int key, KeyboardEvent* event) {
            auto found = _keyCallbacks.find(key);
            if (found != _keyCallbacks.end()) found->second->invoke(event);
        }

        inline void TriggerPressedCallbacksForKey(int key, KeyboardEvent* event) {
            auto found = _keyPressedCallbacks.find(key);
            if (found != _keyPressedCallbacks.end()) found->second->invoke(event);
            for (auto& callback : _anyKeyPressedCallbacks) callback->invoke(event);
        }

        inline void TriggerReleasedCallbacksForKey(int key, KeyboardEvent* event) {
            auto found = _keyReleasedCallbacks.find(key);
            if (found != _keyReleasedCallbacks.end()) found->second->invoke(event);
            for (auto& callback : _anyKeyReleasedCallbacks) callback->invoke(event);
        }

    public:
        bool IsKeyPressed(int key) override {
            auto found = _currentlyPressedKeys.find(key);
            if (found == _currentlyPressedKeys.end()) return false;
            return found->second;
        }

        bool IsModifierPressed(int modifierKey) override {
            auto found = _currentlyPressedModifiers.find(modifierKey);
            if (found == _currentlyPressedModifiers.end()) return false;
            return found->second;
        }

        void EmitKeyEvent(KeyboardEvent* event) override {
            TriggerAnyKeyCallbacks(event);
            TriggerCallbacksForKey(event->key(), event);
            if (event->pressed()) TriggerPressedCallbacksForKey(event->key(), event);
            else TriggerReleasedCallbacksForKey(event->key(), event);
        }

        void SetKeyPressed(int key, bool pressed, bool isAutoRepeat, bool triggerCallbacks) override {
            if (pressed) _currentlyPressedKeys[key] = pressed;
            else _currentlyPressedKeys.erase(key);
            if (triggerCallbacks) {
                KeyboardEvent event{key, pressed, isAutoRepeat};
                EmitKeyEvent(&event);
            }
        }

        void SetModifierPressed(int modifierKey, bool pressed) override {
            _currentlyPressedModifiers[modifierKey] = pressed;
        }

        IFunctionPointer<void(KeyboardEvent*)>* RegisterForKey(
            int key, IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        ) override {
            _keyCallbacks[key] = callbackFunctionPointer;
            return callbackFunctionPointer;
        }
        void UnregisterForKey(int key, IFunctionPointer<void(KeyboardEvent*)>* callbackPointer) override {
            auto found = _keyCallbacks.find(key);
            if (found == _keyCallbacks.end()) return;
            if (found->second != callbackPointer) return;
            _keyCallbacks.erase(found);
        }
        void UnregisterForKey(int key) override { _keyCallbacks.erase(key); }
        void UnregisterForKey() override { _keyCallbacks.clear(); }

        IFunctionPointer<void(KeyboardEvent*)>* RegisterForKeyPressed(
            int key, IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        ) override {
            _keyPressedCallbacks[key] = callbackFunctionPointer;
            return callbackFunctionPointer;
        }
        void UnregisterForKeyPressed(int key, IFunctionPointer<void(KeyboardEvent*)>* callbackPointer) override {
            auto found = _keyPressedCallbacks.find(key);
            if (found == _keyPressedCallbacks.end()) return;
            if (found->second != callbackPointer) return;
            _keyPressedCallbacks.erase(found);
        }
        void UnregisterForKeyPressed(int key) override { _keyPressedCallbacks.erase(key); }
        void UnregisterForKeyPressed() override { _keyPressedCallbacks.clear(); }

        IFunctionPointer<void(KeyboardEvent*)>* RegisterForKeyReleased(
            int key, IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        ) override {
            _keyReleasedCallbacks[key] = callbackFunctionPointer;
            return callbackFunctionPointer;
        }
        void UnregisterForKeyReleased(int key, IFunctionPointer<void(KeyboardEvent*)>* callbackPointer) override {
            auto found = _keyReleasedCallbacks.find(key);
            if (found == _keyReleasedCallbacks.end()) return;
            if (found->second != callbackPointer) return;
            _keyReleasedCallbacks.erase(found);
        }
        void UnregisterForKeyReleased(int key) override { _keyReleasedCallbacks.erase(key); }
        void UnregisterForKeyReleased() override { _keyReleasedCallbacks.clear(); }

        IFunctionPointer<void(KeyboardEvent*)>* RegisterForAnyKey(
            IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        ) override {
            _anyKeyCallbacks.insert(callbackFunctionPointer);
            return callbackFunctionPointer;
        }
        void UnregisterForAnyKey(IFunctionPointer<void(KeyboardEvent*)>* callbackPointer) override {
            _anyKeyCallbacks.erase(callbackPointer);
        }
        void UnregisterForAnyKey() override { _anyKeyCallbacks.clear(); }

        IFunctionPointer<void(KeyboardEvent*)>* RegisterForAnyKeyPressed(
            IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        ) override {
            _anyKeyPressedCallbacks.insert(callbackFunctionPointer);
            return callbackFunctionPointer;
        }
        void UnregisterForAnyKeyPressed(IFunctionPointer<void(KeyboardEvent*)>* callbackPointer) override {
            _anyKeyPressedCallbacks.erase(callbackPointer);
        }
        void UnregisterForAnyKeyPressed() override { _anyKeyPressedCallbacks.clear(); }

        IFunctionPointer<void(KeyboardEvent*)>* RegisterForAnyKeyReleased(
            IFunctionPointer<void(KeyboardEvent*)>* callbackFunctionPointer
        ) override {
            _anyKeyReleasedCallbacks.insert(callbackFunctionPointer);
            return callbackFunctionPointer;
        }
        void UnregisterForAnyKeyReleased(IFunctionPointer<void(KeyboardEvent*)>* callbackPointer) override {
            _anyKeyReleasedCallbacks.erase(callbackPointer);
        }
        void UnregisterForAnyKeyReleased() override { _anyKeyReleasedCallbacks.clear(); }
    };
}
