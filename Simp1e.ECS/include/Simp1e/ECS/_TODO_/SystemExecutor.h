#pragma once

#include <functional>

#include "SystemPointer.h"
#include "SystemType.h"

namespace Simp1e::ECS {

    class SystemExecutor {
        bool                                _isEnabled = true;
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

        SystemType GetSystemType() const { return _systemType; }

        template <typename T>
        T* GetSystem() const {
            return static_cast<T*>(_systemPointer.get());
        }

        bool IsEnabled() const { return _isEnabled; }

        void Enable() { _isEnabled = true; }
        void Disable() { _isEnabled = false; }
        void Toggle() { _isEnabled = !_isEnabled; }
        void SetEnabled(bool isEnabled) { _isEnabled = isEnabled; }

        void Update() {
            if (_isEnabled) _update(_systemPointer);
        }
    };
}
