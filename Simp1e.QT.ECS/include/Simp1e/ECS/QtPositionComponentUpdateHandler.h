#pragma once

#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/QT/Conversions/ToQPointF.h>

#include "QtComponentUpdateHandler.h"

namespace Simp1e::ECS {

    struct QtPositionComponentUpdateHandler : public QtComponentUpdateHandler {
        void Update(Game& game, Entity entity, ComponentPointer& component) override {
            auto* positionComponent = component_cast<PositionComponent>(component);
            if (!positionComponent) return;
            if (!positionComponent->IsDirty()) return;
            positionComponent->SetDirty(false);
            auto* graphicsItemComponent =
                game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetGraphicsItem();
            if (!graphicsItem) return;
            graphicsItem->setPos(ToQPointF(positionComponent->GetPosition()));
            graphicsItem->update();
        }
    };
}
