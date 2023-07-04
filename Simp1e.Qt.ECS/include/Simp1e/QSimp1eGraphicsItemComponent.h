#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/QSimp1eGraphicsItem.h>

namespace Simp1e {

    class QSimp1eGraphicsItemComponent {
        QSimp1eGraphicsItem* _item;

    public:
        DEFINE_COMPONENT_TYPE("QSimp1eGraphicsItem")

        QSimp1eGraphicsItemComponent(QSimp1eGraphicsItem* item) : _item(item) {}

        QSimp1eGraphicsItem* GetQSimp1eGraphicsItem() const { return _item; }
    };
}
