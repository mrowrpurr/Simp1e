#pragma once

#include <string>

#include "IDockedComponent.h"


namespace Simp1e {

    class DockedComponent : public IDockedComponent {
        std::string    _title;
        DockedLocation _defaultDockedLocation;

    public:
        DockedComponent(const char* title, DockedLocation defaultDockedLocation = DockedLocation::Left)
            : _title(title), _defaultDockedLocation(defaultDockedLocation) {}

        const char* GetTitle() const override { return _title.c_str(); }
        void        SetTitle(const char* title) override {
            _title = title;
            SetDirty(Fields::Title);
        }

        DockedLocation GetDefaultDockedLocation() const override { return _defaultDockedLocation; }
        void           SetDefaultDockedLocation(DockedLocation location) override {
            _defaultDockedLocation = location;
            SetDirty(Fields::DefaultDockedLocation);
        }
    };
}
