#pragma once

#include <string>
#include <vector>

#include "LevelItem.h"
#include "PlayerCharacter.h"

namespace SideScroller {

    struct Level {
        std::string            name;
        double                 width;
        double                 height;
        PlayerCharacter        player;
        std::vector<LevelItem> items;
    };
}
