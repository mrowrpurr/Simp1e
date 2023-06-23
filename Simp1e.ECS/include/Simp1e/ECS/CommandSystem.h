#pragma once

#include <_Log_.h>

#include <functional>
#include <memory>
#include <queue>

#include "CommandExecutor.h"
#include "Game.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class CommandSystem {
        Game&                                        _game;
        std::queue<std::unique_ptr<CommandExecutor>> _commands;

    public:
        SIMP1E_ECS_SYSTEM("Commands")

        CommandSystem(Game& game) : _game(game) {}

        template <typename T>
        void AddCommand(const CommandType& commandType, T* command) {
            _commands.emplace(std::make_unique<CommandExecutor>(
                commandType, command,
                [&](Game& game, CommandPointer& command) { static_cast<T*>(command.get())->Execute(_game); }
            ));
        }

        template <typename T>
        void AddCommand(const CommandType& commandType, T command) {
            AddCommand(commandType, new T(std::move(command)));
        }

        template <typename T>
        void AddCommand(T* command) {
            AddCommand(T::GetCommandType(), command);
        }

        template <typename T>
        void AddCommand(T command) {
            AddCommand(T::GetCommandType(), command);
        }

        template <typename T>
        void AddCommand() {
            AddCommand(T::GetCommandType(), new T());
        }

        template <typename T, typename... Args>
        void AddCommand(Args&&... args) {
            AddCommand(T::GetCommandType(), new T(std::forward<Args>(args)...));
        }

        void Update() {
            while (!_commands.empty()) {
                auto& command = _commands.front();
                try {
                    command->Execute(_game);
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
