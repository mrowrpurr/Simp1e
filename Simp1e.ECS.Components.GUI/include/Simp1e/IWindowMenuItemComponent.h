#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/Entity.h>
#include <function_pointer.h>

namespace Simp1e {

    struct IWindowMenuItemComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("WindowMenuItem");

        enum class Fields : int {
            Text          = 1 << 0,
            Visible       = 1 << 1,
            Enabled       = 1 << 2,
            ClickFunction = 1 << 3,
        };

        virtual Entity                    GetWindowMenuEntity() const                                 = 0;
        virtual const char*               GetText() const                                             = 0;
        virtual void                      SetText(const char* text)                                   = 0;
        virtual bool                      IsVisible() const                                           = 0;
        virtual void                      SetVisible(bool enabled)                                    = 0;
        virtual bool                      IsEnabled() const                                           = 0;
        virtual void                      SetEnabled(bool enabled)                                    = 0;
        virtual void                      SetClickFunction(IFunctionPointer<void()>* functionPointer) = 0;
        virtual IFunctionPointer<void()>* GetClickFunction() const                                    = 0;
    };
}
