#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/QSimp1eGraphicsItem.h>
#include <_Log_.h>

#include <memory>

namespace Simp1e {

    class QSimp1eGraphicsItemComponent {
        Entity                               _entity;
        std::unique_ptr<QSimp1eGraphicsItem> _item;
        IFunctionPointer<void(Entity, QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>*
            _systemEntityPaintFunction;

        void OnPaint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) {
            if (_systemEntityPaintFunction) _systemEntityPaintFunction->invoke(_entity, painter, option, widget);
        }

    public:
        DEFINE_COMPONENT_TYPE("QSimp1eGraphicsItem")

        QSimp1eGraphicsItemComponent(
            Entity                                                                                entity,
            IFunctionPointer<void(Entity, QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>* paintFunctionPointer
        )
            : _entity(entity),
              _systemEntityPaintFunction(paintFunctionPointer),
              _item(std::make_unique<QSimp1eGraphicsItem>(
                  new_function_pointer(this, &QSimp1eGraphicsItemComponent::OnPaint)
              )) {}

        QSimp1eGraphicsItem* GetQSimp1eGraphicsItem() const { return _item.get(); }
    };
}
