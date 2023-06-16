#pragma once

#include "sreal.h"

namespace Simp1e {
    class Size {
        sreal _width  = 0;
        sreal _height = 0;

    public:
        struct Params {
            sreal width;
            sreal height;
        };

        Size() = default;
        Size(sreal width, sreal height) : _width(width), _height(height) {}
        Size(const Params& params) : _width(params.width), _height(params.height) {}

        sreal& width() { return _width; }
        sreal& height() { return _height; }

        const sreal& width() const { return _width; }
        const sreal& height() const { return _height; }
    };
}
