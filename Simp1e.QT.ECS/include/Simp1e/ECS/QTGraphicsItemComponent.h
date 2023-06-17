#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/Position.h>
#include <Simp1e/QT/Conversions.h>
#include <Simp1e/QT/QTGraphicsItem.h>
#include <Simp1e/Size.h>

#include <string>

namespace Simp1e::ECS {

    class QTGraphicsItemComponent {
        QTGraphicsItem* _item = nullptr;

    public:
        SIMP1E_ECS_COMPONENT("QTGraphicsItem")

        QTGraphicsItemComponent() = default;
        QTGraphicsItemComponent(QGraphicsScene* scene) : _item(new QTGraphicsItem(scene)) {}

        QTGraphicsItem* GetItem() { return _item; }

        void SetText(const std::string& text) { _item->SetText(text); }
        void SetPosition(const Position& position) { _item->setPos(ToQPointF(position)); }
        void SetSize(const Size& size) { _item->SetSize(ToQSizeF(size)); }
    };
}
