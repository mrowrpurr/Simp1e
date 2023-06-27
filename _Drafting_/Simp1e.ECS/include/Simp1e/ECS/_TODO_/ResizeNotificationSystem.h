#pragma once

#include <memory>

#include "ComponentCast.h"
#include "EntityManager.h"
#include "OnResizeComponent.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class ResizeNotificationSystem {
        EntityManager&               _entityManager;
        std::unique_ptr<ResizeEvent> _lastResizeEvent;

    public:
        ResizeNotificationSystem(EntityManager& entityManager) : _entityManager(entityManager) {}

        SIMP1E_ECS_SYSTEM("ResizeNotificationSystem")

        void RegisterListener(EventManager& eventManager) {
            eventManager.AddListener<ResizeEvent>([this](ResizeEvent* event) {
                OnResizeEvent(event);
            });
        }

        void Update() {
            if (!_lastResizeEvent) return;
            auto& onResizeInputComponents = _entityManager.GetComponents<OnResizeComponent>();
            for (auto& [entity, component] : onResizeInputComponents) {
                auto* onResizeInputComponent = component_cast<OnResizeComponent>(component);
                if (!onResizeInputComponent) continue;
                onResizeInputComponent->TriggerEvent(_lastResizeEvent.get());
            }
            _lastResizeEvent.reset();
        }

    protected:
        virtual void OnResizeEvent(ResizeEvent* e) {
            _lastResizeEvent = std::make_unique<ResizeEvent>(*e);
        }
    };
}
