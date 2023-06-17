#pragma once

#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/QT/Conversions/ToQRectF.h>
#include <Simp1e/QT/Conversions/ToQString.h>

#include "QtComponentRenderer.h"

namespace Simp1e::ECS {

    struct QtTextComponentRenderer : public QtComponentRenderer {
        void Render(
            Game& game, Entity entity, ComponentPointer* component,
            ReadonlyEntityComponentCollection& components, QPainter* painter,
            const QStyleOptionGraphicsItem* option, QWidget* widget
        ) override {
            // TODO make this like a SizeComponent maybe
            auto* rectangleComponent = components.GetComponent<RectangleComponent>();
            if (!rectangleComponent) return;

            auto* textComponent = component_cast<TextComponent>(component);
            if (!textComponent) return;

            painter->drawText(
                ToQRectF(rectangleComponent->GetRectangle()), ToQString(textComponent->GetText())
            );
        }
    };
}
