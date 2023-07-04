#pragma once

#include <string>

#include "INameComponent.h"

namespace Simp1e {

    class NameComponent : public INameComponent {
        std::string _name;

    public:
        NameComponent() = default;
        NameComponent(const char* name) : _name(name) {}

        const char* GetName() const override { return _name.c_str(); }

        void SetName(const char* name) override {
            _name = name;
            SetDirtyFlag(Fields::Name);
        }
    };
}