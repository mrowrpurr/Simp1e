#pragma once

#include <Simp1e/UI/UIColor.h>
#include <Simp1e/UI/UIImage.h>
#include <Simp1e/UI/UIPosition.h>
#include <Simp1e/UI/UISize.h>

#include <string>

#include "LevelItemType.h"

namespace SideScroller {

    struct LevelItem {
        LevelItemType          type;
        Simp1e::UI::UIPosition position;
        Simp1e::UI::UISize     size;
        Simp1e::UI::UIImage    image;
        Simp1e::UI::UIColor    backgroundColor;
    };
}
