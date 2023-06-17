#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/ECS/Entity.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/ECS/VisibleComponent.h>
#include <Simp1e/Position.h>
#include <Simp1e/QT/Conversions.h>
#include <Simp1e/QT/QTGraphicsItem.h>
#include <Simp1e/Size.h>

#include <string>

namespace Simp1e::ECS {

    class QTGraphicsItemComponent {
        Entity         _entity;
        Game&          _game;
        QTGraphicsItem _item;

    public:
        SIMP1E_ECS_COMPONENT("QTGraphicsItem")

        QTGraphicsItemComponent(Game& game, Entity entity, QGraphicsScene* scene)
            : _entity(entity),
              _game(game),
              _item(
                  [this](
                      QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget
                  ) { OnPaint(painter, option, widget); },
                  scene
              ) {}

    protected:
        void OnPaint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
            auto entity     = _game.Entities().Get(_entity);
            auto components = entity.GetComponents();

            // TODO - update GetComponent() to return optional or ptr

            if (components.HasComponent<VisibleComponent>() &&
                components.GetComponent<VisibleComponent>()->IsVisible()) {
                _item.SetBoundingRect(QRectF(0, 0, 100, 100));
                _item.SetSize(QSizeF(100, 100));
                _item.SetText("Hello from the entity.");

                painter->setBrush(QBrush(QColor(255, 0, 255)));
                painter->drawRect(_item.GetBoundingRect());
            }
        }
    };
}
