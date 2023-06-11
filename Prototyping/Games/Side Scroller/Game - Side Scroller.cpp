#include "SideScroller/Game/Level.h"
#include "SideScroller/UI/UIGame.h"

// More specifically: Platformer
using namespace SideScroller;

int main(int argc, char** argv) {
    Level level{
        .name   = "My first level",
        .width  = 2000,
        .height = 500,
        .player = {{.position = {400, 0}, .size = {10, 50}, .backgroundColor = {255, 0, 255}}},
        .items  = {
                   {
                 .type            = LevelItemType::Platform,
                 .position        = {10, 100},
                 .size            = {100, 10},
                 .backgroundColor = {255, 0, 0},
            }, {
                 .type            = LevelItemType::Platform,
                 .position        = {600, 40},
                 .size            = {100, 10},
                 .backgroundColor = {0, 255, 255},
            }, {
                 .type            = LevelItemType::Platform,
                 .position        = {800, 100},
                 .size            = {100, 10},
                 .backgroundColor = {255, 255, 0},
            }}
    };

    UIGame game{argc, argv};

    game.LoadLevel(level);

    return game.Run();
}