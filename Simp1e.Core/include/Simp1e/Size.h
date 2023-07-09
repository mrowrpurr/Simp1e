#pragma once

#include "Vec2.h"
#include "sreal.h"

namespace Simp1e {
    class Size : public Vec2<sreal> {
    public:
        struct Params {
            sreal width;
            sreal height;
        };

        Size() = default;
        Size(sreal width, sreal height) : Vec2(width, height) {}
        Size(const Params& params) : Vec2(params.width, params.height) {}

        virtual sreal width() const { return one(); }
        virtual sreal height() const { return two(); }

        virtual void SetWidth(sreal width) { SetOne(width); }
        virtual void SetHeight(sreal height) { SetTwo(height); }

        virtual bool IsNull() const { return width() == 0 && height() == 0; }
    };
}
