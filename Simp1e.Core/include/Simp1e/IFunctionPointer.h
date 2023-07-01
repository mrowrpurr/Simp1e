#pragma once

namespace Simp1e {

    class IFunctionPointer {
    public:
        virtual ~IFunctionPointer() {}
        virtual bool IsMemberFunction() const = 0;
    };
}
