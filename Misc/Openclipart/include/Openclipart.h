#pragma once

#include <filesystem>
#include <stdexcept>

namespace Openclipart {
    std::filesystem::path Root() {
        auto* folder = std::getenv("Openclipart");
        if (!folder) throw std::runtime_error("Openclipart environment variable not set");
        return folder;
    }

    namespace Filenames {
        constexpr auto GRASS_GROUND                  = "315011.png";
        constexpr auto WALKING_KID_SIDE_SPRITE_SHEET = "248089 large.png";
    }
}
