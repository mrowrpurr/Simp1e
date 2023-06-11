#pragma once

#include "IUILevel.h"
#include "IUILevelItem.h"

namespace SideScroller {

    class UILevelItem : public IUILevelItem {
        IUILevel* _level;

    public:
        UILevelItem(IUILevel* level, QGraphicsItem* parent = nullptr)
            : IUILevelItem(parent), _level(level) {}
    };
}
