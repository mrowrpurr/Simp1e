#pragma once

#include <string>
#include <vector>

#include "LevelItem.h"

namespace SideScroller {

    struct Level {
        std::string            name;
        int                    width;
        int                    height;
        std::vector<LevelItem> items;
    };
}
