#pragma once

#include <Simp1e/ECS/EntityManager.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/SystemTypeMacro.h>

#include <QGraphicsView>

#include "ViewCenteredComponent.h"

namespace Simp1e::ECS {

    class QtViewCenteredSystem {
        EntityManager& _entityManager;
        QGraphicsView& _view;
        QPointF        _lastViewCenteredPoint;

    public:
        SIMP1E_ECS_SYSTEM("QtViewCentered")

        QtViewCenteredSystem(QGraphicsView& view, EntityManager& entityManager)
            : _view(view), _entityManager(entityManager) {}

        void Update() {
            auto& viewCenteredEntities = _entityManager.GetComponents<ViewCenteredComponent>();
            if (viewCenteredEntities.empty()) return;

            auto viewCenteredEntity         = viewCenteredEntities.begin()->first;
            auto viewCenteredQtGraphicsItem = _entityManager.GetComponent<QTGraphicsItemComponent>(viewCenteredEntity);
            if (!viewCenteredQtGraphicsItem) return;

            auto centerPoint = viewCenteredQtGraphicsItem->GetGraphicsItem()->GetBoundingRect().center();
            if (centerPoint == _lastViewCenteredPoint) return;
            _lastViewCenteredPoint = centerPoint;

            _view.centerOn(centerPoint);
            qDebug() << "Centered at point " << centerPoint;
        }
    };
}