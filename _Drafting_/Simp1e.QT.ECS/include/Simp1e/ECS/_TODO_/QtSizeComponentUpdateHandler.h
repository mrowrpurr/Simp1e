#pragma once

#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/Simp1eQGraphicsItemComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/QT/Conversions/ToQPointF.h>
#include <Simp1e/QT/Conversions/ToQRectF.h>

#include "QtComponentUpdateHandler.h"

namespace Simp1e::ECS {

    struct QtSizeComponentUpdateHandler : public QtComponentUpdateHandler {
        void Update(Game& game, Entity entity, ComponentPointer& component) override {
            auto* sizeComponent = component_cast<SizeComponent>(component);
            if (!sizeComponent) return;
            auto* graphicsItemComponent = game.Entities().GetComponent<Simp1eQGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetGraphicsItem();
            if (!graphicsItem) return;
            auto  rect              = ToQRectF(sizeComponent->size());
            auto* positionComponent = game.Entities().GetComponent<PositionComponent>(entity);
            if (positionComponent) rect.moveTopLeft(ToQPointF(positionComponent->position()));
            if (graphicsItem->GetBoundingRect() != rect) {
                graphicsItem->SetBoundingRect(rect);
                graphicsItem->update();
            }
        }
    };
}
