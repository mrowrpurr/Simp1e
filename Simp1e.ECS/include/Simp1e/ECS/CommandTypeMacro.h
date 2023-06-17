#pragma once

#include "CommandType.h"

#define SIMP1E_ECS_COMMAND(type) \
    static CommandType GetCommandType() { return type; }
