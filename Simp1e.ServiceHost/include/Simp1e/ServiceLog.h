#pragma once

#define ServiceLog(serviceName, ...) _Log_("[" serviceName "] " __VA_ARGS__)
