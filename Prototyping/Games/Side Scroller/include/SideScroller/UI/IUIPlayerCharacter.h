#pragma once

#include "../Game/PlayerCharacter.h"
#include "IUILevelItem.h"

namespace SideScroller {

    struct IUIPlayerCharacter : public IUILevelItem {
        IUIPlayerCharacter(QGraphicsItem* parent = nullptr) : IUILevelItem(parent) {}
        virtual ~IUIPlayerCharacter()               = default;
        virtual PlayerCharacter& GetPlayer()        = 0;
        virtual void             StartMovingRight() = 0;
        virtual void             StartMovingLeft()  = 0;
        virtual void             StopMovingRight()  = 0;
        virtual void             StopMovingLeft()   = 0;
        virtual void             Jump()             = 0;
    };
}
