#pragma once

namespace Simp1e {

    class FunctionPointerBase {
    public:
        virtual ~FunctionPointerBase() {}
        virtual bool IsMemberFunction() const = 0;
    };
}
