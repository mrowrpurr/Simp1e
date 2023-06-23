#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/ECS/Entity.h>
#include <Simp1e/ECS/EntityQTGraphicsItem.h>

#include <QGraphicsScene>

namespace Simp1e::ECS {

    class QTGraphicsItemComponent {
        EntityQTGraphicsItem* _graphicsItem = nullptr;

    public:
        SIMP1E_ECS_COMPONENT("QTGraphicsItem")

        QTGraphicsItemComponent(
            Entity entity, QGraphicsScene& scene,
            std::function<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)> painter
        )
            : _graphicsItem(new EntityQTGraphicsItem(entity, painter)) {
            scene.addItem(_graphicsItem);
        }

        void update() { _graphicsItem->update(); }

        EntityQTGraphicsItem* GetGraphicsItem() { return _graphicsItem; }
        Entity                GetEntity() const {
            if (_graphicsItem == nullptr) return {};
            return _graphicsItem->GetEntity();
        }
    };
}
