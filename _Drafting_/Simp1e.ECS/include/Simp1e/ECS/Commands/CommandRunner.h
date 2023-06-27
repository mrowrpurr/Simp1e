#pragma once

#include <_Log_.h>

#include <functional>
#include <memory>
#include <queue>

#include "CommandExecutor.h"
#include "Game.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class CommandRunner {
        Game&                                        _game;
        std::queue<std::unique_ptr<CommandExecutor>> _commands;

    public:
        CommandRunner(Game& game) : _game(game) {}

        template <typename T>
        void RunCommand(const CommandType& commandType, T* command) {
            std::make_unique<CommandExecutor>(commandType, command, [&](Game& game, CommandPointer& command) {
                static_cast<T*>(command.get())->Execute(_game);
            })->Execute(_game);
        }

        template <typename T>
        void RunCommand(const CommandType& commandType, T command) {
            RunCommand(commandType, new T(std::move(command)));
        }

        template <typename T>
        void RunCommand(T* command) {
            RunCommand(T::GetCommandType(), command);
        }

        template <typename T>
        void RunCommand(T command) {
            RunCommand(T::GetCommandType(), command);
        }

        template <typename T>
        void RunCommand() {
            RunCommand(T::GetCommandType(), new T());
        }

        template <typename T, typename... Args>
        void RunCommand(Args&&... args) {
            RunCommand(T::GetCommandType(), new T(std::forward<Args>(args)...));
        }

        size_t GetCommandCount() const { return _commands.size(); }
    };
}
