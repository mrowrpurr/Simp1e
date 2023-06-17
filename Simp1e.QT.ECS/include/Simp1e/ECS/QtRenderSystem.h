#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/SystemTypeMacro.h>
#include <_Log_.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "QtComponentRenderer.h"
#include "QtComponentUpdateHandler.h"

namespace Simp1e::ECS {

    class QtRenderSystem {
        Game&                             _game;
        QGraphicsScene&                   _scene;
        std::unordered_set<ComponentType> _visualComponentTypes;
        std::unordered_map<ComponentType, std::unique_ptr<QtComponentRenderer>> _componentRenderers;
        std::unordered_map<ComponentType, std::unique_ptr<QtComponentUpdateHandler>>
            _componentUpdateHandlers;

        // Delete copy constructor
        QtRenderSystem(const QtRenderSystem&) = delete;

    public:
        SIMP1E_ECS_SYSTEM("QtRender")

        QtRenderSystem(Game& game, QGraphicsScene& scene) : _game(game), _scene(scene) {
            _Log_("QtRenderSystem::QtRenderSystem");
            _game.Entities().Events().OnComponentAdded([this](auto entity, auto& componentType) {
                _Log_("QtRenderSystem::OnComponentAdded");
                if (!this->_visualComponentTypes.count(componentType)) return;
                if (_game.Entities().HasComponent<QTGraphicsItemComponent>(entity)) return;
                auto* graphicsItemComponent = new QTGraphicsItemComponent(
                    _scene,
                    [this, entity](
                        QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget
                    ) { PaintEntity(entity, painter, option, widget); }
                );
                _game.Entities().AddComponent<QTGraphicsItemComponent>(
                    entity, graphicsItemComponent
                );
            });
            _game.Entities().Events().OnComponentAdded([this](auto entity, auto& componentType) {
                _Log_("QtRenderSystem::OnComponentAdded");
                if (!this->_visualComponentTypes.count(componentType)) return;
                auto* graphicsItemComponent =
                    _game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
                if (graphicsItemComponent) graphicsItemComponent->update();
            });
            _game.Entities().Events().OnComponentRemoved([this](auto entity, auto& componentType) {
                if (!this->_visualComponentTypes.count(componentType)) return;
                auto* graphicsItemComponent =
                    _game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
                if (graphicsItemComponent) graphicsItemComponent->update();
            });
        }

        void AddVisualComponentType(ComponentType componentType) {
            _visualComponentTypes.insert(componentType);
        }

        template <typename T>
        void AddVisualComponentType() {
            AddVisualComponentType(T::GetComponentType());
        }

        void AddComponentRenderer(ComponentType componentType, QtComponentRenderer* renderer) {
            _componentRenderers[componentType] = std::unique_ptr<QtComponentRenderer>(renderer);
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

        void AddComponentUpdateHandler(
            ComponentType componentType, QtComponentUpdateHandler* updateHandler
        ) {
            _componentUpdateHandlers[componentType] =
                std::unique_ptr<QtComponentUpdateHandler>(updateHandler);
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
            _Log_("QtRenderSystem::Update");
            for (auto& [componentType, componentUpdateHandler] : _componentUpdateHandlers)
                for (auto& [entityId, componentPtr] : _game.Entities().GetComponents(componentType))
                    if (auto* component = static_cast<ComponentBase*>(componentPtr.get()))
                        if (component->IsDirty()) {
                            somethingChanged = true;
                            componentUpdateHandler->Update(_game, entityId, componentPtr);
                        }

            if (somethingChanged) _scene.update();
        }

        void PaintEntity(
            Entity entityId, QPainter* painter, const QStyleOptionGraphicsItem* option,
            QWidget* widget
        ) {
            _Log_("QtRenderSystem::PaintEntity {}", entityId);
            auto entity = _game.Entities().Get(entityId);

            auto components = entity.GetComponents();
            _Log_("Entity {} has {} components", entityId, components.size());

            // TODO have some kind of way to check the VisibleComponent and not do this if it's not
            // visible

            for (auto& [componentType, component] : components) {
                _Log_("QtRenderSystem::PaintEntity::Component {}", componentType.c_str());
                if (!_visualComponentTypes.count(componentType)) continue;
                auto foundRenderer = _componentRenderers.find(componentType);
                if (foundRenderer == _componentRenderers.end()) continue;
                foundRenderer->second->Render(
                    _game, entityId, component, components, painter, option, widget
                );
            }
        }
    };
}
