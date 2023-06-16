#pragma once

#include <cstdint>

namespace Simp1e {
    class Color {
        uint8_t _red   = 0;
        uint8_t _green = 0;
        uint8_t _blue  = 0;
        uint8_t _alpha = 255;

    public:
        Color() = default;
        Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
            : _red(red), _green(green), _blue(blue), _alpha(alpha) {}

        static Color Red(uint8_t alpha = 255) { return Color(255, 0, 0, alpha); }
        static Color Green(uint8_t alpha = 255) { return Color(0, 255, 0, alpha); }
        static Color Blue(uint8_t alpha = 255) { return Color(0, 0, 255, alpha); }
        static Color Yellow(uint8_t alpha = 255) { return Color(255, 255, 0, alpha); }
        static Color Magenta(uint8_t alpha = 255) { return Color(255, 0, 255, alpha); }
        static Color Cyan(uint8_t alpha = 255) { return Color(0, 255, 255, alpha); }
        static Color White(uint8_t alpha = 255) { return Color(255, 255, 255, alpha); }
        static Color Black(uint8_t alpha = 255) { return Color(0, 0, 0, alpha); }

        uint8_t& red() { return _red; }
        uint8_t& green() { return _green; }
        uint8_t& blue() { return _blue; }
        uint8_t& alpha() { return _alpha; }

        const uint8_t& red() const { return _red; }
        const uint8_t& green() const { return _green; }
        const uint8_t& blue() const { return _blue; }
        const uint8_t& alpha() const { return _alpha; }
    };
}
