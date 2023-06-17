#pragma once

#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/QT/Conversions/ToQColor.h>
#include <Simp1e/QT/Conversions/ToQRectF.h>

#include "QtComponentRenderer.h"

namespace Simp1e::ECS {

    struct QtRectangleComponentRenderer : public QtComponentRenderer {
        void Render(
            Game& game, Entity entity, ComponentPointer* component,
            ReadonlyEntityComponentCollection& components, QPainter* painter,
            const QStyleOptionGraphicsItem* option, QWidget* widget
        ) override {
            auto* rectangleComponent = component_cast<RectangleComponent>(component);
            if (!rectangleComponent) return;

            if (rectangleComponent->GetBackgroundColor().has_value())
                painter->fillRect(
                    ToQRectF(rectangleComponent->GetRectangle()),
                    ToQColor(rectangleComponent->GetBackgroundColor().value())
                );
            if (rectangleComponent->GetBorderColor().has_value())
                painter->setPen(ToQColor(rectangleComponent->GetBorderColor().value()));
            painter->drawRect(ToQRectF(rectangleComponent->GetRectangle()));
        }
    };
}
