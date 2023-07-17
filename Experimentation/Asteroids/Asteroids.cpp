#include <string_format.h>

#include <QDebug>

// #define _Log_(...) qDebug() << string_format(__VA_ARGS__).c_str()
#include <_Log_.h>
_LogToFile_("C:/Temp/AsteroidsGame.log");

#include "Asteroids/Game.h"

int main(int argc, char* argv[]) {
    Asteroids::Game().Run();
    return 0;
}
