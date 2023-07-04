#pragma once

#include "Vec3.h"
#include "sreal.h"

namespace Simp1e {
    class Position : public Vec3<sreal> {
    public:
        struct Params {
            sreal x;
            sreal y;
            sreal z;
        };

        Position() = default;
        Position(sreal x, sreal y, sreal z) : Vec3(x, y, z) {}
        Position(const Params& params) : Vec3(params.x, params.y, params.z) {}

        virtual sreal x() const { return one(); }
        virtual sreal y() const { return two(); }
        virtual sreal z() const { return three(); }

        virtual void SetX(sreal x) { SetOne(x); }
        virtual void SetY(sreal y) { SetTwo(y); }
        virtual void SetZ(sreal z) { SetThree(z); }

        bool operator==(const Position& other) const {
            return x() == other.x() && y() == other.y() && z() == other.z();
        }
    };
}
