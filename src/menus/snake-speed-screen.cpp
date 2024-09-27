#include "menus.h"

SnakeSpeedScreen::SnakeSpeedScreen() {
    type = SNAKE_SPEED_SCREEN;
        
    snake.grow();
    snake.grow();
    snake.grow();

    frame = 2;

    for (int i = 0; i < 10; i++) {
        moveTick();
    }
};

void SnakeSpeedScreen::moveTick() {
    frame++;
    if (frame > 12) frame = 1;

    SnakeDirection direction = None;
    if (frame < 4) direction = Up;
    else if (frame < 7) direction = Left;
    else if (frame < 10) direction = Down;
    else if (frame < 13) direction = Right;

    snake.fakeMove(direction, false);
};


void SnakeSpeedScreen::render(CRGB leds[], Settings* settings, bool frameTick) {
    if (frameTick) moveTick();
    
    snake.render(leds, GCOLOR_SNAKEHEAD, GCOLOR_SNAKEBODY);
    renderLevel(leds, settings);
};

void SnakeSpeedScreen::renderLevel(CRGB leds[], Settings* settings) {
    int level = settings->snakeSpeed;
    
    for (int i = 1; i < level + 2; i++) {
        Pixel(0, GRID_HEIGHT - i).render(leds, 0xff0066);
    }
};

void SnakeSpeedScreen::changeSetting(SnakeDirection direction, Settings* settings) {
    if (direction == Up) {
        settings->snakeSpeed++;
        if (settings->snakeSpeed > MAX_SETTING_LEVEL) settings->snakeSpeed = MAX_SETTING_LEVEL;
        return;
    }

    if (direction == Down) {
        settings->snakeSpeed--;
        if (settings->snakeSpeed < 0) settings->snakeSpeed = 0;
    }
};