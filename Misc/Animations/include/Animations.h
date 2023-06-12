#pragma once

#include <filesystem>
#include <stdexcept>

namespace Animations {
    std::filesystem::path Root() {
        auto* folder = std::getenv("Animations");
        if (!folder) throw std::runtime_error("Animations environment variable not set");
        return folder;
    }

    namespace Filenames {
        constexpr auto RUN_RIGHT   = "Running 1/Run Right.gif";
        constexpr auto RUN_LEFT    = "Running 1/Run Left.gif";
        constexpr auto STAND_LEFT  = "Running 1/Left.png";
        constexpr auto STAND_RIGHT = "Running 1/Right.png";
        constexpr auto JUMP_RIGHT  = "Bounce 1/Bounce Right.gif";
        constexpr auto JUMP_LEFT   = "Bounce 1/Bounce Left.gif";
    }
}
