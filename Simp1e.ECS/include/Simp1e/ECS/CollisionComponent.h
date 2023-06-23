#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
// #include <Simp1e/Polygon.h>
// #include <Simp1e/Rectangle.h>
// #include <Simp1e/sreal.h>

// #include <optional>

// #include "DirtyTrackingComponent.h"

namespace Simp1e::ECS {
    class CollisionComponent {  // : public DirtyTrackingComponent {
        bool _raisesEvents = false;
        // std::optional<Rectangle> _box;
        // std::optional<Polygon>   _polygon;

    public:
        SIMP1E_ECS_COMPONENT("Collision")

        CollisionComponent() = default;
        CollisionComponent(bool raisesEvents) : _raisesEvents(raisesEvents) {}
        // CollisionComponent(const Rectangle& box) : _box(box) {}
        // CollisionComponent(const Polygon& polygon) : _polygon(polygon) {}

        // virtual std::optional<Rectangle> Box() const { return _box; }
        // virtual std::optional<Polygon>   Polygon() const { return _polygon; }

        // virtual void SetBox(const Rectangle& box) {
        //     if (_box != box) {
        //         _box = box;
        //         SetDirty();
        //     }
        // }
        // virtual void SetPolygon(const Polygon& polygon) {
        //     if (_polygon != polygon) {
        //         _polygon = polygon;
        //         SetDirty();
        //     }
        // }

        // virtual void ClearBox() {
        //     if (_box.has_value()) {
        //         _box.reset();
        //         SetDirty();
        //     }
        // }
        // virtual void ClearPolygon() {
        //     if (_polygon.has_value()) {
        //         _polygon.reset();
        //         SetDirty();
        //     }
        // }

        // virtual bool HasBox() const { return _box.has_value(); }
        // virtual bool HasPolygon() const { return _polygon.has_value(); }

        virtual bool RaisesEvents() const { return _raisesEvents; }

        virtual void SetRaisesEvents(bool raisesEvents) {
            if (_raisesEvents != raisesEvents) {
                _raisesEvents = raisesEvents;
                // SetDirty();
            }
        }
    };
}
