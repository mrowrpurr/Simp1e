#pragma once

#include <cstdint>

namespace Prototyping::UI {
    struct UIColor {
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        uint8_t alpha = 255;

        UIColor() = default;
        UIColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
            : red(red), green(green), blue(blue), alpha(alpha) {}

        static UIColor Red(uint8_t alpha = 255) { return UIColor(255, 0, 0, alpha); }
        static UIColor Green(uint8_t alpha = 255) { return UIColor(0, 255, 0, alpha); }
        static UIColor Blue(uint8_t alpha = 255) { return UIColor(0, 0, 255, alpha); }
        static UIColor Yellow(uint8_t alpha = 255) { return UIColor(255, 255, 0, alpha); }
        static UIColor Magenta(uint8_t alpha = 255) { return UIColor(255, 0, 255, alpha); }
        static UIColor Cyan(uint8_t alpha = 255) { return UIColor(0, 255, 255, alpha); }
        static UIColor White(uint8_t alpha = 255) { return UIColor(255, 255, 255, alpha); }
        static UIColor Black(uint8_t alpha = 255) { return UIColor(0, 0, 0, alpha); }
    };
}
