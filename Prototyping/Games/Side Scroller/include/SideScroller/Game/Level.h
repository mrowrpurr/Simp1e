#pragma once

#include <string>
#include <vector>

#include "LevelItem.h"

namespace SideScroller {

    struct Level {
        std::string            name;
        double                 width;
        double                 height;
        std::vector<LevelItem> items;
    };
}
