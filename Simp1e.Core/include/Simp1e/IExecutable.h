#pragma once

namespace Simp1e {

    struct IExecutable {
        virtual ~IExecutable()               = default;
        virtual void* operator()(void* data) = 0;
        virtual void* operator()()           = 0;
        virtual void* Execute(void* data)    = 0;
        virtual void* Execute()              = 0;
    };
}
