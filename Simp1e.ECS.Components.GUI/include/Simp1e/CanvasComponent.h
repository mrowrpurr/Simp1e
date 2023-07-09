#pragma once

#include "ICanvasComponent.h"

namespace Simp1e {

    class CanvasComponent : public ICanvasComponent {
        Entity _parentEntity = 0;
        int    _width        = 0;  // TODO: update to store as Size
        int    _height       = 0;

    public:
        CanvasComponent(Entity parent, int width = 0, int height = 0) : _width(width), _height(height) {}

        Entity GetParentEntity() const override { return _parentEntity; }
        void   SetParentEntity(Entity parentEntity) override {
            _parentEntity = parentEntity;
            SetDirtyFlag(Fields::ParentEntity);
        }

        Size GetSize() const override { return Size(_width, _height); }

        int  GetWidth() const override { return _width; }
        void SetWidth(int width) override {
            _width = width;
            SetDirtyFlag(Fields::Width);
        }

        int  GetHeight() const override { return _height; }
        void SetHeight(int height) override {
            _height = height;
            SetDirtyFlag(Fields::Height);
        }
    };
}