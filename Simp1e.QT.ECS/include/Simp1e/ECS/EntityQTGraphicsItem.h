#pragma once

#include <Simp1e/ECS/Entity.h>
#include <Simp1e/QT/QTGraphicsItem.h>

namespace Simp1e::ECS {

    class EntityQTGraphicsItem : public QTGraphicsItem {
        // So that we can get from the Qt scene item ---> an entity and its components
        Entity _entity;

    public:
        EntityQTGraphicsItem(
            Entity entity, std::function<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)> painter
        )
            : _entity(entity), QTGraphicsItem(painter) {}

        Entity GetEntity() const { return _entity; }
    };
}
