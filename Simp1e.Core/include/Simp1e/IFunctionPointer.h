#pragma once

#include "IValueWrapper.h"

namespace Simp1e {

    struct IFunctionPointer {
        virtual ~IFunctionPointer() {}
        virtual IValueWrapper* InvokeAndReturn(IValueWrapper** args) = 0;
        virtual void           Invoke(IValueWrapper** args)          = 0;
    };
}
