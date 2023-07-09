#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/IFillColorComponent.h>
#include <Simp1e/ILineColorComponent.h>
#include <Simp1e/IPositionComponent.h>
#include <Simp1e/IRectangleComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <Simp1e/Rectangle.h>
#include <Simp1e/ToQColor.h>
#include <Simp1e/ToQRectF.h>
#include <_Log_.h>

#include "IQtComponentPainter.h"

namespace Simp1e {

    class QtRectangleComponentPainter : public IQtComponentPainter {
    public:
        void Paint(
            IEngine* engine, Entity entity, void* component, QPainter* painter, const QStyleOptionGraphicsItem* option,
            QWidget* widget
        ) override {
            if (!component_cast<IRectangleComponent>(component))
                return;  // TODO fix it so Painters() only get called for the right components

            auto* entityManager = engine->GetEntities();

            auto* position = entityManager->GetComponent<IPositionComponent>(entity);
            if (!position) {
                _Log_("[Rectangle Painter] No position component found for entity {}", entity);
                return;
            }

            auto* size = entityManager->GetComponent<ISizeComponent>(entity);
            if (!size) {
                _Log_("[Rectangle Painter] No size component found for entity {}", entity);
                return;
            }
            if (size->GetSize().IsNull()) {
                _Log_("[Rectangle Painter] Size is null for entity {}", entity);
                return;
            }

            auto rect = ToQRectF(size->GetSize());

            auto* fillColor = entityManager->GetComponent<IFillColorComponent>(entity);
            if (fillColor) painter->fillRect(rect, ToQColor(fillColor->GetColor()));

            auto* lineColor = entityManager->GetComponent<ILineColorComponent>(entity);
            if (lineColor) painter->setPen(ToQColor(lineColor->GetColor()));

            painter->drawRect(rect);
        }
    };
}
