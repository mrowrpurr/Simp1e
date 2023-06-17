#pragma once

#include <functional>

#include "CommandPointer.h"
#include "CommandType.h"

namespace Simp1e::ECS {

    class CommandExecutor {
        CommandType                          _commandType;
        CommandPointer                       _commandPointer;
        std::function<void(CommandPointer&)> _execute;

        // Delete copy constructor
        CommandExecutor(const CommandExecutor&) = delete;

    public:
        template <typename T>
        CommandExecutor(
            const CommandType& commandType, T* command, std::function<void(CommandPointer&)> execute
        )
            : _commandType(commandType),
              _commandPointer(MakeCommandPointer(command)),
              _execute(execute) {}

        template <typename T>
        CommandExecutor(
            const CommandType& commandType, T&& command,
            std::function<void(CommandPointer&)> execute
        )
            : _commandType(commandType),
              _commandPointer(MakeCommandPointer(command)),
              _execute(execute) {}

        CommandType GetCommandType() const { return _commandType; }

        template <typename T>
        T* GetCommand() const {
            return static_cast<T*>(_commandPointer.get());
        }

        void Execute() { _execute(_commandPointer); }
    };
}
