#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

#include <string>

namespace Simp1e {

    class WindowComponent : public DirtyTrackingComponentBase {
        std::string _title;
        std::string _statusBarText;

    public:
        DEFINE_COMPONENT_TYPE("Window");

        WindowComponent() = default;
        WindowComponent(const char* title) : _title(title) {}

        enum class Fields : int {
            Title         = 1 << 0,
            StatusBarText = 1 << 1,
        };

        const char* GetTitle() const { return _title.c_str(); }
        const char* GetStatusBarText() const { return _statusBarText.c_str(); }

        void SetTitle(const char* title) {
            _title = title;
            AddDirtyFlag(Fields::Title);
        }

        void SetStatusBarText(const char* text) {
            _statusBarText = text;
            AddDirtyFlag(Fields::StatusBarText);
        }
    };
}
