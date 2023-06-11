#include "SideScroller/Game/Level.h"
#include "SideScroller/UI/UIGame.h"

using namespace SideScroller;

int main(int argc, char** argv) {
    Level level{
        .name   = "My first level",
        .width  = 8000,
        .height = 500,
        .items  = {{
             .type            = LevelItemType::Player,
             .position        = {10, 30},
             .size            = {10, 50},
             .backgroundColor = {255, 0, 255},
        }}};

    UIGame game{argc, argv};

    game.LoadLevel(level);

    return game.Run();
}
