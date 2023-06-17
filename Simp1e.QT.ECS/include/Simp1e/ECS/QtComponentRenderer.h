#pragma once

#include <Simp1e/ECS/ComponentPointer.h>
#include <Simp1e/ECS/Entity.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/ReadonlyEntityComponentCollection.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

namespace Simp1e::ECS {

    struct QtComponentRenderer {
        virtual void Render(
            Game& game, Entity entity, ComponentPointer* component,
            ReadonlyEntityComponentCollection& components, QPainter* painter,
            const QStyleOptionGraphicsItem* option, QWidget* widget
        ) = 0;
    };
}
