#include "SideScroller/Game/Level.h"
#include "SideScroller/UI/UIGame.h"

// More specifically: Platformer
using namespace SideScroller;

int main(int argc, char** argv) {
    Level level{
        .name   = "My first level",
        .width  = 8000,
        .height = 1000,
        .player = {{.position = {600, 0}, .size = {200, 200}, .backgroundColor = {255, 0, 255}}},
        .items  = {
                   {
                 .type            = LevelItemType::Platform,
                 .position        = {10, 100},
                 .size            = {400, 30},
                 .backgroundColor = {255, 0, 0},
            }, {
                 .type            = LevelItemType::Platform,
                 .position        = {1200, 200},
                 .size            = {400, 30},
                 .backgroundColor = {0, 255, 255},
            }, {
                 .type            = LevelItemType::Platform,
                 .position        = {1800, 300},
                 .size            = {400, 30},
                 .backgroundColor = {255, 255, 0},
            }}
    };

    UIGame game{argc, argv};

    game.LoadLevel(level);

    return game.Run();
}
