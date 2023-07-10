#pragma once

#include <Simp1e/IPositionComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <Simp1e/ToQPointF.h>
#include <Simp1e/ToQRectF.h>
#include <_Log_.h>

#include "IQtComponentUpdateHandler.h"
#include "QtSimp1eGraphicsItemComponent.h"

namespace Simp1e {

    class QtPositionComponentUpdateHandler : public IQtComponentUpdateHandler {
    public:
        void Update(IEngine* engine, Entity entity, void* component) override {
            auto* entityManager = engine->GetEntities();

            auto* position = entityManager->GetComponent<IPositionComponent>(entity);
            if (!position) return;
            if (!position->IsDirty()) return;

            auto* graphicsItemComponent = entityManager->GetComponent<QtSimp1eGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetQtSimp1eGraphicsItem();
            if (!graphicsItem) return;

            auto pos = ToQPointF(position->GetPosition().ToPoint());
            if (pos != graphicsItem->pos()) graphicsItem->setPos(pos);
            position->SetDirty(false);
            _Log_("UPDATED POSITION x:{} y:{}", pos.x(), pos.y());
        }
    };
}
