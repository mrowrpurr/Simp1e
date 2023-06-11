#pragma once

#include <QGraphicsItem>

namespace SideScroller {

    struct IUILevelItem : public QGraphicsItem {
        IUILevelItem(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {}
        virtual ~IUILevelItem() = default;
    };
}
