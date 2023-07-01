#pragma once

#include "IValueWrapper.h"

namespace Simp1e {

    struct IFunctionPointer {
        virtual ~IFunctionPointer() {}
        virtual bool IsMemberFunction() const = 0;
        // virtual IValueWrapper* Invoke(IValueWrapper** args)     = 0;
        // virtual IValueWrapper* operator()(IValueWrapper** args) = 0;
        virtual IValueWrapper* Invoke(IValueWrapper** args) = 0;
        // virtual IValueWrapper* operator()(IValueWrapper** args) = 0;
    };
}
