#pragma once

#include "IUILevelItem.h"

namespace SideScroller {

    struct IUIPlayerCharacter : public IUILevelItem {
        IUIPlayerCharacter(QGraphicsItem* parent = nullptr) : IUILevelItem(parent) {}
        virtual ~IUIPlayerCharacter() = default;
    };
}
