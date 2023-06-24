#pragma once

#include <Simp1e/ECS/EntityManager.h>
#include <Simp1e/ECS/SystemTypeMacro.h>
#include <Simp1e/ECS/ViewCenteredComponent.h>

#include <QGraphicsView>

#include "Simp1eQGraphicsItemComponent.h"

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

            auto viewCenteredEntity = viewCenteredEntities.begin()->first;
            auto viewCenteredQtGraphicsItem =
                _entityManager.GetComponent<Simp1eQGraphicsItemComponent>(viewCenteredEntity);
            if (!viewCenteredQtGraphicsItem) return;

            auto centerPoint = viewCenteredQtGraphicsItem->GetGraphicsItem()->GetBoundingRect().center();
            if (centerPoint == _lastViewCenteredPoint) return;
            _lastViewCenteredPoint = centerPoint;

            _view.centerOn(centerPoint);
        }
    };
}
