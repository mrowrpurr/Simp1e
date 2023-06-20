#pragma once

#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTImageComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/QT/Conversions/ToQPointF.h>

#include "QtComponentRenderer.h"

namespace Simp1e::ECS {

    struct QTImageComponentRenderer : public QtComponentRenderer {
        void Render(
            Game& game, Entity entity, ComponentPointer* component,
            ReadonlyEntityComponentCollection& components, QPainter* painter,
            const QStyleOptionGraphicsItem* option, QWidget* widget
        ) override {
            auto* imageComponent = component_cast<QTImageComponent>(component);
            if (!imageComponent) return;
            if (imageComponent->pixmap().isNull()) return;

            auto positionComponent = components.GetComponent<PositionComponent>();
            if (!positionComponent) return;

            auto sizeComponent = components.GetComponent<SizeComponent>();
            if (!sizeComponent) return;
            if (sizeComponent->size().isNull()) return;
            imageComponent->SetSize(sizeComponent->size());

            // painter->resetTransform();
            qDebug() << "Drawing pixmap at position: " << ToQPointF(positionComponent->position());
            painter->drawPixmap(
                ToQPointF(positionComponent->position()), imageComponent->GetPixmap()
            );
        }
    };
}
