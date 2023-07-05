#pragma once

#include <Simp1e/Entity.h>
#include <Simp1e/IEngine.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

namespace Simp1e {

    struct IQtComponentPainter {
        virtual ~IQtComponentPainter() = default;

        virtual void Paint() = 0;
    };
}

// virtual void Render(
//     Game& game, Entity entity, ComponentPointer* component, ReadonlyEntityComponentCollection& components,
//     QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget
// ) = 0;

//         #pragma once

// #include <Simp1e/ECS/ComponentCast.h>
// #include <Simp1e/ECS/ComponentPointer.h>
// #include <Simp1e/ECS/Entity.h>
// #include <Simp1e/ECS/Game.h>

// namespace Simp1e::ECS {

//     struct QtComponentUpdateHandler {
//         virtual ~QtComponentUpdateHandler() = default;

//         virtual void Update(Game& game, Entity entity, ComponentPointer& component) = 0;
//     };
// }
