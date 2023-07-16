#pragma once

#include <Simp1e/ComponentCast.h>
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
            auto* position = component_cast<IPositionComponent>(component);
            if (!position) return;
            if (!position->IsDirty()) return;
            _Log_("Position is dirty!");

            auto* entityManager = engine->GetEntities();

            auto* graphicsItemComponent = entityManager->GetComponent<QtSimp1eGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetQtSimp1eGraphicsItem();
            if (!graphicsItem) return;

            auto pos = ToQPointF(position->GetPosition().ToPoint());
            if (pos != graphicsItem->pos()) {
                _Log_("changing the position of the graphics item associated with this entity");
                graphicsItem->setPos(pos);
                graphicsItem->update();
                // hmmm
                graphicsItem->scene()->update();  // <--- grrrr whyyyy...
                _Log_("Changed position of graphics item to {}", position->GetPosition().ToString());
            } else {
                _Log_("The position is the same as the existing graphics position");
            }
            position->SetDirty(false);
            _Log_("UPDATED POSITION x:{} y:{}", pos.x(), pos.y());
        }
    };
}
