#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/DirtyTrackingComponent.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/SystemTypeMacro.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "QtComponentRenderer.h"
#include "QtComponentUpdateHandler.h"

namespace Simp1e::ECS {

    class QtRenderSystem {
        Game&                                                                        _game;
        QGraphicsScene&                                                              _scene;
        std::unordered_set<ComponentType>                                            _visualComponentTypes;
        std::unordered_map<ComponentType, std::unique_ptr<QtComponentRenderer>>      _componentRenderers;
        std::vector<ComponentType>                                                   _componentRenderersOrder;
        std::unordered_map<ComponentType, std::unique_ptr<QtComponentUpdateHandler>> _componentUpdateHandlers;

        // Delete copy constructor
        QtRenderSystem(const QtRenderSystem&) = delete;

    public:
        SIMP1E_ECS_SYSTEM("QtRender")

        QtRenderSystem(Game& game, QGraphicsScene& scene) : _game(game), _scene(scene) {
            _game.Entities().Events().OnComponentAdded([this](auto entity, auto& componentType) {
                if (!this->_visualComponentTypes.count(componentType)) return;
                if (_game.Entities().HasComponent<QTGraphicsItemComponent>(entity)) return;
                auto* graphicsItemComponent = new QTGraphicsItemComponent(
                    entity, _scene,
                    [this, entity](QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
                        painter->save();
                        PaintEntity(entity, painter, option, widget);
                        painter->restore();
                    }
                );
                _game.Entities().AddComponent<QTGraphicsItemComponent>(entity, graphicsItemComponent);
            });
            _game.Entities().Events().OnComponentAdded([this](auto entity, auto& componentType) {
                if (!this->_visualComponentTypes.count(componentType)) return;
                auto* graphicsItemComponent = _game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
                if (graphicsItemComponent) graphicsItemComponent->update();
            });
            _game.Entities().Events().OnComponentRemoved([this](auto entity, auto& componentType) {
                if (!this->_visualComponentTypes.count(componentType)) return;
                auto* graphicsItemComponent = _game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
                if (graphicsItemComponent) graphicsItemComponent->update();

                // If there are no more visual components on this entity, remove the graphics item
                auto entityComponents   = _game.Entities().GetComponents(entity);
                bool hasVisualComponent = false;
                for (auto& [componentType, component] : entityComponents)
                    if (_visualComponentTypes.count(componentType)) {
                        hasVisualComponent = true;
                        break;
                    }
                if (!hasVisualComponent) _game.Entities().RemoveComponent<QTGraphicsItemComponent>(entity);
            });
        }

        void AddVisualComponentType(ComponentType componentType) { _visualComponentTypes.insert(componentType); }

        template <typename T>
        void AddVisualComponentType() {
            AddVisualComponentType(T::GetComponentType());
        }

        void AddComponentRenderer(ComponentType componentType, QtComponentRenderer* renderer) {
            _componentRenderers[componentType] = std::unique_ptr<QtComponentRenderer>(renderer);
            _componentRenderersOrder.push_back(componentType);
            AddVisualComponentType(componentType);
        }

        template <typename T>
        void AddComponentRenderer(QtComponentRenderer* renderer) {
            AddComponentRenderer(T::GetComponentType(), renderer);
        }

        template <typename T, typename R>
        void AddComponentRenderer() {
            AddComponentRenderer(T::GetComponentType(), new R());
        }

        void AddComponentUpdateHandler(ComponentType componentType, QtComponentUpdateHandler* updateHandler) {
            _componentUpdateHandlers[componentType] = std::unique_ptr<QtComponentUpdateHandler>(updateHandler);
            AddVisualComponentType(componentType);
        }

        template <typename T>
        void AddComponentUpdateHandler(QtComponentUpdateHandler* updateHandler) {
            AddComponentUpdateHandler(T::GetComponentType(), updateHandler);
        }

        template <typename T, typename U>
        void AddComponentUpdateHandler() {
            AddComponentUpdateHandler(T::GetComponentType(), new U());
        }

        void Update() {
            bool somethingChanged = false;
            for (auto& [componentType, componentUpdateHandler] : _componentUpdateHandlers)
                for (auto& [entityId, componentPtr] : _game.Entities().GetComponents(componentType))
                    if (auto* component = static_cast<DirtyTrackingComponent*>(componentPtr.get()))
                        if (component->IsDirty()) {
                            component->SetDirty(false);
                            somethingChanged = true;
                            componentUpdateHandler->Update(_game, entityId, componentPtr);
                        }

            // if (somethingChanged) _scene.update();
        }

        void PaintEntity(Entity entityId, QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
            auto entity     = _game.Entities().Get(entityId);
            auto components = entity.GetComponents();

            for (int i = 0; i < _componentRenderersOrder.size(); ++i) {
                auto componentType = _componentRenderersOrder[i];
                if (!components.HasComponent(componentType)) continue;
                auto foundRenderer = _componentRenderers.find(componentType);
                if (foundRenderer == _componentRenderers.end()) continue;
                auto* component = components.GetComponent(componentType);
                foundRenderer->second->Render(_game, entityId, component, components, painter, option, widget);
            }
        }
    };
}
