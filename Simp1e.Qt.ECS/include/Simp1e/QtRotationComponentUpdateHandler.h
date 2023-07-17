#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/IImageComponent.h>
#include <Simp1e/IRotationComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <Simp1e/ToQPointF.h>
#include <Simp1e/ToQRectF.h>
#include <_Log_.h>

#include "IQtComponentUpdateHandler.h"
#include "QtSimp1eGraphicsItemComponent.h"
#include "QtSimp1eImageComponent.h"

namespace Simp1e {

    class QtRotationComponentUpdateHandler : public IQtComponentUpdateHandler {
    public:
        void Update(IEngine* engine, Entity entity, void* component) override {
            auto* rotation = component_cast<IRotationComponent>(component);
            if (!rotation) return;
            if (!rotation->IsDirty()) return;
            _Log_("Rotation is dirty!");

            auto* entityManager = engine->GetEntities();

            auto* graphicsItemComponent = entityManager->GetComponent<QtSimp1eGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetQtSimp1eGraphicsItem();
            if (!graphicsItem) return;

            auto* qtImageComponent = entityManager->GetComponent<QtSimp1eImageComponent>(entity);
            if (qtImageComponent) {
                //
                _Log_("Rotation of IMAGE graphics item changed");
                qtImageComponent->GetQSimp1eImage()->RotateTo(rotation->GetRotation());
                graphicsItem->update();
                graphicsItem->scene()->update();
            } else {
                //
                _Log_("Rotation of (non-image) graphics item changed");
                _Log_("NOT YET SUPPORTED");
            }

            rotation->SetDirty(false);
        }
    };
}
