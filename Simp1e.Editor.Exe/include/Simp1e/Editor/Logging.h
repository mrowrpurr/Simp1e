#pragma once

#include <string_format.h>

#include <QDebug>
#define _Log_(...) qDebug() << string_format(__VA_ARGS__)
#include <_Log_.h>
