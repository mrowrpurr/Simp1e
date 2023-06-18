#pragma once

// Require this file before any other file that uses _Log_

#include <string_format.h>

#include <QDebug>

#define _Log_(...) qDebug() << string_format(__VA_ARGS__).c_str()
