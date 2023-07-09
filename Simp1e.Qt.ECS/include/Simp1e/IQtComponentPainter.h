#pragma once

#include <Simp1e/Entity.h>
#include <Simp1e/IEngine.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

namespace Simp1e {

    // TODO - add an entity component collection! optimize the lookup of various components from a given entity.

    struct IQtComponentPainter {
        virtual ~IQtComponentPainter() = default;

        virtual void Paint(
            IEngine* engine, Entity entity, void* component, QPainter* painter, const QStyleOptionGraphicsItem* option,
            QWidget* widget
        ) = 0;
    };
}
