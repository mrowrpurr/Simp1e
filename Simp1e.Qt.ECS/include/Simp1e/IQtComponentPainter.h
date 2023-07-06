#pragma once

#include <Simp1e/Entity.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/IEntityComponentCollection.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

namespace Simp1e {

    struct IQtComponentPainter {
        virtual ~IQtComponentPainter() = default;

        virtual void Paint(
            IEngine* engine, Entity entity, void* component, IEntityComponentCollection* components, QPainter* painter,
            const QStyleOptionGraphicsItem* option, QWidget* widget
        ) = 0;
    };
}
