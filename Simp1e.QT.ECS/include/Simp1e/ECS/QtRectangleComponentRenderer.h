#pragma once

#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/QT/Conversions/ToQColor.h>
#include <Simp1e/QT/Conversions/ToQPointF.h>
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

            Rectangle rectangle;
            if (rectangleComponent->GetRectangle().has_value()) {
                rectangle = rectangleComponent->GetRectangle().value();
            } else {
                auto positionComponent = components.GetComponent<PositionComponent>();
                if (!positionComponent) return;
                auto sizeComponent = components.GetComponent<SizeComponent>();
                if (!sizeComponent) return;
                rectangle = Rectangle(positionComponent->position(), sizeComponent->size());
            }

            auto rect = ToQRectF(rectangle);

            if (rectangleComponent->GetBackgroundColor().has_value())
                painter->fillRect(rect, ToQColor(rectangleComponent->GetBackgroundColor().value()));

            if (rectangleComponent->GetBorderColor().has_value())
                painter->setPen(ToQColor(rectangleComponent->GetBorderColor().value()));

            painter->drawRect(rect);
        }
    };
}
