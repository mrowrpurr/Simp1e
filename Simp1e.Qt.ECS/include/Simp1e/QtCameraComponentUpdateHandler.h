#pragma once

#include <Simp1e/ICameraComponent.h>
#include <Simp1e/IPositionComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <Simp1e/QSimp1eGraphicsView.h>
#include <Simp1e/ToQRectF.h>
#include <_Log_.h>

#include "IQtComponentUpdateHandler.h"

namespace Simp1e {

    class QtCameraComponentUpdateHandler : public IQtComponentUpdateHandler {
        QSimp1eGraphicsView* _graphicsView;

    public:
        QtCameraComponentUpdateHandler(QSimp1eGraphicsView* graphicsView) : _graphicsView(graphicsView) {}

        void Update(IEngine* engine, Entity entity, void* component) override {
            auto camera = component_cast<ICameraComponent>(component);
            if (!camera) return;
            if (!camera->IsDirty()) return;

            auto* entityManager = engine->GetEntities();

            auto* position = entityManager->GetComponent<IPositionComponent>(entity);
            if (!position) return;

            auto* size = entityManager->GetComponent<ISizeComponent>(entity);
            if (!size) return;
            if (size->GetSize().IsNull()) return;

            Rectangle rect(position->GetPosition().ToPoint(), size->GetSize());

            _graphicsView->fitInView(ToQRectF(rect), Qt::KeepAspectRatio);

            camera->ClearDirty();
        }
    };
}
