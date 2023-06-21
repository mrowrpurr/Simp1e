#pragma once

#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/QT/Conversions/ToQPointF.h>
#include <Simp1e/QT/Conversions/ToQRectF.h>

#include "QtComponentUpdateHandler.h"

namespace Simp1e::ECS {

    struct QtPositionComponentUpdateHandler : public QtComponentUpdateHandler {
        void Update(Game& game, Entity entity, ComponentPointer& component) override {
            qDebug() << "Updating position of entity " << entity;
            auto* positionComponent = component_cast<PositionComponent>(component);
            if (!positionComponent) return;

            // qDebug() << Position

            auto* graphicsItemComponent = game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetGraphicsItem();
            if (!graphicsItem) return;

            // We don't actually update the pos(), we update the bounding box instead via setting size to dirty.

            // Changing the position changes the bounding box, which right now is owned by SizeComponent.
            auto* sizeComponent = game.Entities().GetComponent<SizeComponent>(entity);
            if (sizeComponent) sizeComponent->SetDirty(true);

            // auto point = ToQPointF({positionComponent->x(), positionComponent->y()});
            // qDebug() << "Updating position of entity " << entity << " to " << point;

            // graphicsItem->setPos(point);
            // graphicsItem->update();
        }
    };
}
