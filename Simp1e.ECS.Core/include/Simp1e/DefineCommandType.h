#pragma once

#include "CommandType.h"

#define DEFINE_COMMAND_TYPE(CommandName) \
    static CommandType GetCommandType() { return CommandName; }
