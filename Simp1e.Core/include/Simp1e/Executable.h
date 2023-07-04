#pragma once

#include <functional>

#include "IExecutable.h"

namespace Simp1e {

    // TODO - stop using me
    class Executable : public IExecutable {
        std::function<void*(void*)> _execute;

    public:
        Executable(std::function<void*(void*)> execute) : _execute(execute) {}
        Executable(std::function<void(void*)> execute)
            : _execute([execute](void* data) {
                  execute(data);
                  return nullptr;
              }) {}
        Executable(std::function<void()> execute)
            : _execute([execute](void*) {
                  execute();
                  return nullptr;
              }) {}

        void* operator()(void* data) override { return _execute(data); }
        void* operator()() override { return _execute(nullptr); }
        void* Execute(void* data) override { return _execute(data); }
        void* Execute() override { return _execute(nullptr); }
    };
}
