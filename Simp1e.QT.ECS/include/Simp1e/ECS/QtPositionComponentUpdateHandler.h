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
            auto* positionComponent = component_cast<PositionComponent>(component);
            if (!positionComponent) return;

            // Updating a position needs to update bounding box, so mark the size component as dirty (represents the
            // bounding box)
            auto* sizeComponent = game.Entities().GetComponent<SizeComponent>(entity);
            if (sizeComponent) {
                sizeComponent->SetDirty(true);
            }
        }
    };
}
