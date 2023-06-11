#pragma once

#include "IUILevel.h"
#include "IUIPlayerCharacter.h"
#include "UILevelItem.h"

namespace SideScroller {

    class UIPlayerCharacter : public IUIPlayerCharacter, public UILevelItem {
    public:
        UIPlayerCharacter(IUILevel* level, QGraphicsItem* parent = nullptr)
            : IUIPlayerCharacter(parent), UILevelItem(level, parent) {}
    };
}
