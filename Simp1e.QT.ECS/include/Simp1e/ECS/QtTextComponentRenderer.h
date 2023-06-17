#pragma once

#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/QT/Conversions/ToQRectF.h>
#include <Simp1e/QT/Conversions/ToQString.h>
#include <_Log_.h>

#include "QtComponentRenderer.h"

namespace Simp1e::ECS {

    struct QtTextComponentRenderer : public QtComponentRenderer {
        void Render(
            Game& game, Entity entity, ComponentPointer* component,
            ReadonlyEntityComponentCollection& components, QPainter* painter,
            const QStyleOptionGraphicsItem* option, QWidget* widget
        ) override {
            _Log_("QtTextComponentRenderer::Render");

            // TODO make this like a SizeComponent maybe
            auto* rectangleComponent = components.GetComponent<RectangleComponent>();
            if (!rectangleComponent) return;

            auto* textComponent = component_cast<TextComponent>(component);
            if (!textComponent) return;

            _Log_("The text is {}", textComponent->GetText());
            painter->drawText(
                ToQRectF(rectangleComponent->GetRectangle()), ToQString(textComponent->GetText())
            );
        }
    };
}
