#pragma once

#include <cstdint>

namespace Prototyping::UI {
    struct UIColor {
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        uint8_t alpha = 255;

        UIColor() = default;
        UIColor(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
        UIColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
            : red(red), green(green), blue(blue), alpha(alpha) {}
    };
}
