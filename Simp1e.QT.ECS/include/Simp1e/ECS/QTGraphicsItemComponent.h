#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/QT/QTGraphicsItem.h>

#include <QGraphicsScene>

namespace Simp1e::ECS {

    class QTGraphicsItemComponent {
        QTGraphicsItem* _graphicsItem = nullptr;

    public:
        SIMP1E_ECS_COMPONENT("QTGraphicsItem")

        QTGraphicsItemComponent(
            QGraphicsScene&                                                           scene,
            std::function<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)> painter
        )
            : _graphicsItem(new QTGraphicsItem(painter)) {
            scene.addItem(_graphicsItem);
        }

        void update() { _graphicsItem->update(); }

        QTGraphicsItem* GetGraphicsItem() { return _graphicsItem; }
    };
}
