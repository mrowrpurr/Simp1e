#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/QSimp1eGraphicsItem.h>
#include <_Log_.h>

namespace Simp1e {

    class QSimp1eGraphicsItemComponent {
        std::unique_ptr<QSimp1eGraphicsItem> _item;

        void PaintFunction(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) {
            _Log_("HELLO FROM THE PAINT FUNCTION.");
        }

    public:
        DEFINE_COMPONENT_TYPE("QSimp1eGraphicsItem")

        QSimp1eGraphicsItemComponent()
            : _item(std::make_unique<QSimp1eGraphicsItem>(
                  new_function_pointer(this, &QSimp1eGraphicsItemComponent::PaintFunction)
              )) {}

        QSimp1eGraphicsItem* GetQSimp1eGraphicsItem() const { return _item.get(); }
    };
}
