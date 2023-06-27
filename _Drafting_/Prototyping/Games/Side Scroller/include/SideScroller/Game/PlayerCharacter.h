#pragma once

#include "LevelItem.h"

namespace SideScroller {

    struct PlayerCharacter : public LevelItem {
        PlayerCharacter() { type = LevelItemType::Player; }
        PlayerCharacter(const LevelItem& item) : LevelItem(item) { type = LevelItemType::Player; }
    };
}
