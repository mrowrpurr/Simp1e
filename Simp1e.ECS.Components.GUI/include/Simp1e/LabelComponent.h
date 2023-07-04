#pragma once

#include <string>

#include "ILabelComponent.h"

namespace Simp1e {

    class LabelComponent : public ILabelComponent {
        Entity      _parentEntity = 0;
        std::string _text;

    public:
        LabelComponent(Entity parent, const char* text = "") : _parentEntity(parent), _text(text) {}

        Entity GetParentEntity() const override { return _parentEntity; }
        void   SetParentEntity(Entity parentEntity) override {
            _parentEntity = parentEntity;
            SetDirtyFlag(Fields::ParentEntity);
        }

        const char* GetText() const override { return _text.c_str(); }
        void        SetText(const char* text) override {
            _text = text;
            SetDirtyFlag(Fields::Text);
        }
    };
}