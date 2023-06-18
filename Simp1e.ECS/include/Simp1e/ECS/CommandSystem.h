#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <_Log_.h>

#include "CommandExecutor.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class CommandSystem {
        std::queue<std::unique_ptr<CommandExecutor>> _commands;

    public:
        SIMP1E_ECS_SYSTEM("CommandSystem")

        template <typename T>
        void AddCommand(const CommandType& commandType, T* command) {
            _commands.emplace(std::make_unique<CommandExecutor>(
                commandType, command,
                [](CommandPointer& command) { static_cast<T*>(command.get())->Execute(); }
            ));
        }
        template <typename T>
        void AddCommand(T* command) {
            AddCommand(T::GetCommandType(), command);
        }

        template <typename T>
        void AddCommand(const CommandType& commandType, T&& command) {
            AddCommand(commandType, new T(std::forward<T>(command)));
        }

        template <typename T>
        void AddCommand(T&& command) {
            AddCommand(T::GetCommandType(), std::forward<T>(command));
        }

        template <typename T>
        void AddCommand() {
            AddCommand(T::GetCommandType(), new T());
        }

        void Update() {
            while (!_commands.empty()) {
                auto& command = _commands.front();
                try {
                    command->Execute();
                } catch (...) {
                    _Log_(
                        "CommandSystem::Update() - Exception caught while executing command of type '{}'",
                        command->GetCommandType()
                    );
                }
                _commands.pop();
            }
        }

        size_t GetCommandCount() const { return _commands.size(); }
    };
}
