#pragma once

#include <memory>

#include "ComponentCast.h"
#include "EntityManager.h"
#include "OnMouseClickComponent.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class MouseClickInputSystem {
        EntityManager&                   _entityManager;
        std::unique_ptr<MouseClickEvent> _lastMouseClickEvent;

    public:
        MouseClickInputSystem(EntityManager& entityManager) : _entityManager(entityManager) {}

        SIMP1E_ECS_SYSTEM("MouseClickInput")

        virtual void RegisterListener(EventManager& eventManager) {
            eventManager.AddListener<MouseClickEvent>([this](MouseClickEvent* event) { OnMouseClickEvent(event); });
        }

        virtual void Update() {
            if (!_lastMouseClickEvent) return;
            auto& onMouseClickInputComponents = _entityManager.GetComponents<OnMouseClickComponent>();
            for (auto& [entity, component] : onMouseClickInputComponents) {
                auto* onMouseClickInputComponent = component_cast<OnMouseClickComponent>(component);
                if (!onMouseClickInputComponent) continue;
                onMouseClickInputComponent->TriggerEvent(_lastMouseClickEvent.get());
            }
            if (!_lastMouseClickEvent->pressed()) _lastMouseClickEvent.reset();
        }

    protected:
        virtual void OnMouseClickEvent(MouseClickEvent* e) {
            _lastMouseClickEvent = std::make_unique<MouseClickEvent>(*e);
        }
    };
}
