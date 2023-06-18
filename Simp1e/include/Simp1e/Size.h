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

        void SetWidth(sreal width) { _width = width; }
        void SetHeight(sreal height) { _height = height; }

        const sreal& width() const { return _width; }
        const sreal& height() const { return _height; }

        bool isNull() const { return _width == 0 && _height == 0; }
    };
}
