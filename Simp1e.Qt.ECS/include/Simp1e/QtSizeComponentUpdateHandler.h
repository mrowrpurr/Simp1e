#pragma once

#include <Simp1e/IPositionComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <Simp1e/ToQPointF.h>
#include <Simp1e/ToQRectF.h>
#include <_Log_.h>

#include "IQtComponentUpdateHandler.h"
#include "QtSimp1eGraphicsItemComponent.h"

namespace Simp1e {

    class QtSizeComponentUpdateHandler : public IQtComponentUpdateHandler {
    public:
        void Update(IEngine* engine, Entity entity, void* component) override {
            auto* entityManager = engine->GetEntities();

            auto* size = entityManager->GetComponent<ISizeComponent>(entity);
            if (!size) return;
            if (!size->IsDirty()) return;

            auto* graphicsItemComponent = entityManager->GetComponent<QtSimp1eGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetQtSimp1eGraphicsItem();
            if (!graphicsItem) return;

            auto rect = ToQRectF(Rectangle({0, 0}, size->GetSize()));
            if (rect != graphicsItem->GetBoundingRect()) graphicsItem->SetBoundingRect(rect);
            size->SetDirty(false);
            _Log_("UPDATED SIZE. Width: {} Height: {}", rect.width(), rect.height());
        }
    };
}
