#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/QtSimp1eGraphicsItem.h>
#include <_Log_.h>

#include <memory>

namespace Simp1e {

    class QtSimp1eGraphicsItemComponent {
        Entity                                _entity;
        std::unique_ptr<QtSimp1eGraphicsItem> _item;
        IFunctionPointer<void(Entity, QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>*
            _systemEntityPaintFunction;

        void OnPaint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) {
            if (_systemEntityPaintFunction) _systemEntityPaintFunction->invoke(_entity, painter, option, widget);
        }

    public:
        DEFINE_COMPONENT_TYPE("QtSimp1eGraphicsItem")

        QtSimp1eGraphicsItemComponent(
            Entity                                                                                entity,
            IFunctionPointer<void(Entity, QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>* paintFunctionPointer
        )
            : _entity(entity),
              _systemEntityPaintFunction(paintFunctionPointer),
              _item(std::make_unique<QtSimp1eGraphicsItem>(
                  new_function_pointer(this, &QtSimp1eGraphicsItemComponent::OnPaint)
              )) {}

        QtSimp1eGraphicsItem* GetQtSimp1eGraphicsItem() const { return _item.get(); }
    };
}
