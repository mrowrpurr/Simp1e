#pragma once

#include <functional>

#include "SystemPointer.h"
#include "SystemType.h"

namespace Simp1e::ECS {

    class SystemExecutor {
        SystemType                          _systemType;
        SystemPointer                       _systemPointer;
        std::function<void(SystemPointer&)> _update;

        // Delete copy constructor
        SystemExecutor(const SystemExecutor&) = delete;

    public:
        template <typename T>
        SystemExecutor(
            const SystemType& systemType, T* system, std::function<void(SystemPointer&)> update
        )
            : _systemType(systemType), _systemPointer(MakeSystemPointer(system)), _update(update) {}

        template <typename T>
        SystemExecutor(
            const SystemType& systemType, T&& system, std::function<void(SystemPointer&)> update
        )
            : _systemType(systemType), _systemPointer(MakeSystemPointer(system)), _update(update) {}

        void Update() { _update(_systemPointer); }

        void operator()() { Update(); }
    };
}
