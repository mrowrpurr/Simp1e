#pragma once

#include <memory>
#include <unordered_set>

#include "ComponentCast.h"
#include "EntityManager.h"
#include "OnKeyboardComponent.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class KeyboardInputSystem {
        EntityManager& _entityManager;
        // std::unique_ptr<KeyboardEvent> _lastMouseClickEvent;
        std::unordered_set<KeyboardEvent::Key> _pressedKeys;
        std::unordered_set<KeyboardEvent::Key> _releasedKeys;

    public:
        KeyboardInputSystem(EntityManager& entityManager) : _entityManager(entityManager) {}

        SIMP1E_ECS_SYSTEM("KeyboardInput")

        virtual void RegisterListener(EventManager& eventManager) {
            eventManager.AddListener<KeyboardEvent>([this](KeyboardEvent* event) { OnKeyboardEvent(event); });
        }

        virtual void Update() {
            if (_pressedKeys.empty() && _releasedKeys.empty()) return;
            auto& onKeyboardInputComponents = _entityManager.GetComponents<OnKeyboardComponent>();
            for (auto& [entity, component] : onKeyboardInputComponents) {
                auto* onKeyboardInputComponent = component_cast<OnKeyboardComponent>(component);
                if (!onKeyboardInputComponent) continue;
                // TODO : possibly optimize by providing ONE EVENT which has sets of pressed and released...
                // Yeah, in the future, soon :)
                for (auto& key : _pressedKeys) {
                    auto event = std::make_unique<KeyboardEvent>(key, true);
                    onKeyboardInputComponent->TriggerEvent(event.get());
                }
                for (auto& key : _releasedKeys) {
                    auto event = std::make_unique<KeyboardEvent>(key, false);
                    onKeyboardInputComponent->TriggerEvent(event.get());
                }
                // onKeyboardInputComponent->TriggerEvent(_lastMouseClickEvent.get());
            }
        }

    protected:
        virtual void OnKeyboardEvent(KeyboardEvent* e) {
            if (e->pressed()) {
                _pressedKeys.insert(e->key());
                _releasedKeys.erase(e->key());
            } else {
                _pressedKeys.erase(e->key());
                _releasedKeys.insert(e->key());
            }
        }
    };
}
