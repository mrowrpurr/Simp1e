#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/ECS/Entity.h>
#include <Simp1e/ECS/EntitySimp1eQGraphicsItem.h>

#include <QGraphicsScene>

namespace Simp1e::ECS {

    class Simp1eQGraphicsItemComponent {
        EntitySimp1eQGraphicsItem* _graphicsItem = nullptr;

    public:
        SIMP1E_ECS_COMPONENT("Simp1eQGraphicsItem")

        Simp1eQGraphicsItemComponent(
            Entity entity, QGraphicsScene& scene,
            std::function<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)> painter
        )
            : _graphicsItem(new EntitySimp1eQGraphicsItem(entity, painter)) {
            scene.addItem(_graphicsItem);
        }

        void update() { _graphicsItem->update(); }

        EntitySimp1eQGraphicsItem* GetGraphicsItem() { return _graphicsItem; }
        Entity                     GetEntity() const {
            if (_graphicsItem == nullptr) return {};
            return _graphicsItem->GetEntity();
        }
    };
}
