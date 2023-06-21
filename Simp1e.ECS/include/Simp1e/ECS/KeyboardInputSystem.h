#pragma once

#include <memory>

#include "ComponentCast.h"
#include "EntityManager.h"
#include "OnKeyboardComponent.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class KeyboardInputSystem {
        EntityManager&                 _entityManager;
        std::unique_ptr<KeyboardEvent> _lastMouseClickEvent;

    public:
        KeyboardInputSystem(EntityManager& entityManager) : _entityManager(entityManager) {}

        SIMP1E_ECS_SYSTEM("KeyboardInputSystem")

        void RegisterListener(EventManager& eventManager) {
            eventManager.AddListener<KeyboardEvent>([this](KeyboardEvent* event) { OnKeyboardEvent(event); });
        }

        void Update() {
            if (!_lastMouseClickEvent) return;
            auto& onKeyboardInputComponents = _entityManager.GetComponents<OnKeyboardComponent>();
            for (auto& [entity, component] : onKeyboardInputComponents) {
                auto* onKeyboardInputComponent = component_cast<OnKeyboardComponent>(component);
                if (!onKeyboardInputComponent) continue;
                onKeyboardInputComponent->TriggerEvent(_lastMouseClickEvent.get());
            }
            if (!_lastMouseClickEvent->pressed()) {
                _lastMouseClickEvent.reset();
                _lastMouseClickEvent = nullptr;
            }
        }

    protected:
        virtual void OnKeyboardEvent(KeyboardEvent* e) { _lastMouseClickEvent = std::make_unique<KeyboardEvent>(*e); }
    };
}
