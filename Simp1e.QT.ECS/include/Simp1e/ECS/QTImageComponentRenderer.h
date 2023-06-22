#pragma once

#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/ImageComponent.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/QT/Conversions/ToQPointF.h>

#include <QPixMapCache>
#include <QPixmap>

#include "QtComponentRenderer.h"

namespace Simp1e::ECS {

    struct QtImageComponentRenderer : public QtComponentRenderer {
        void Render(
            Game& game, Entity entity, ComponentPointer* component, ReadonlyEntityComponentCollection& components,
            QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget
        ) override {
            auto* imageComponent = component_cast<ImageComponent>(component);
            if (!imageComponent) return;

            auto imagePath = imageComponent->GetImagePath();
            if (imagePath.empty()) return;

            auto* positionComponent = components.GetComponent<PositionComponent>();
            if (!positionComponent) return;

            auto* sizeComponent = components.GetComponent<SizeComponent>();
            if (!sizeComponent) return;

            // TODO give this some caching instead of making a new one every time for every entity
            QPixmap pixmap;
            QPixmapCache::find(imagePath.c_str(), &pixmap);

            if (pixmap.isNull()) {
                pixmap = QPixmap(imagePath.c_str());
                QPixmapCache::insert(imagePath.c_str(), pixmap);
            }

            if (pixmap.isNull()) return;

            if (pixmap.width() != sizeComponent->width() || pixmap.height() != sizeComponent->height())
                pixmap = pixmap.scaled(sizeComponent->width(), sizeComponent->height());

            painter->drawPixmap(ToQPointF(positionComponent->position()), pixmap);
        }
    };
}
