#define spec_name BoundingBox

#include "BoundingBox.h"

#include "GameSpecHelper.h"

Describe("BoundingBox") {
    it("can check if a coordinate is within the bounding box", []() {
        BoundingBox box{
            Coordinate{1, 1},
            Coordinate{3, 3}
        };

        AssertThat(box.Contains({0, 0}), IsFalse());
        AssertThat(box.Contains({1, 1}), IsTrue());
        AssertThat(box.Contains({2, 2}), IsTrue());
        AssertThat(box.Contains({3, 3}), IsTrue());
        AssertThat(box.Contains({4, 4}), IsFalse());
    });
}
