#pragma once

#include <Simp1e/ECS/Entity.h>
#include <Simp1e/QT/Simp1eQGraphicsItem.h>

namespace Simp1e::ECS {

    class EntitySimp1eQGraphicsItem : public Simp1eQGraphicsItem {
        // So that we can get from the Qt scene item ---> an entity and its components
        Entity _entity;

    public:
        EntitySimp1eQGraphicsItem(
            Entity entity, std::function<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)> painter
        )
            : _entity(entity), Simp1eQGraphicsItem(painter) {}

        Entity GetEntity() const { return _entity; }
    };
}
