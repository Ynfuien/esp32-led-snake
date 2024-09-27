#include "menus.h"

BorderPhasingScreen::BorderPhasingScreen() {
    type = BORDER_PHASING_SCREEN;

    phasingSnake.fakeMove(Down, true);
    normalSnake.fakeMove(Down, true);
    for (int i = 0; i < GRID_WIDTH; i++) {
        phasingSnake.fakeMove(Right, true);
        normalSnake.fakeMove(Right, true);
    }
};

void BorderPhasingScreen::render(CRGB leds[], Settings* settings, bool frameTick) {
    if (frameTick) {
        phasingSnake.fakeMove(Right, true);

        SnakeDirection dir = nextSnakeDirection;
        
        if (!normalSnake.fakeMove(dir, false)) {
            if (dir == Right) {
                dir = Up;
                nextSnakeDirection = Left;
            }
            else if (dir == Left) {
                dir = Down;
                nextSnakeDirection = Right;
            }

            normalSnake.fakeMove(dir, false);
        }
    }

    if (settings->borderPhasing) {
        phasingSnake.render(leds, GCOLOR_SNAKEHEAD, GCOLOR_SNAKEBODY);
        return;
    }

    normalSnake.render(leds, GCOLOR_SNAKEHEAD, GCOLOR_SNAKEBODY);
};

void BorderPhasingScreen::changeSetting(SnakeDirection direction, Settings* settings) {
    settings->borderPhasing = !settings->borderPhasing;
};