#include "game.h"

Berry::Berry(bool borderVisible) {
    this->borderVisible = borderVisible;
};

// Spawns berry in a random position,
// but not inside of the snake nor at a border.
void Berry::spawn(Snake snake) {
    // This is equivalent to
    // "int min = borderVisible ? 1 : 0"
    int min = borderVisible;

    do {
        int randX = random(min, GRID_WIDTH - borderVisible);
        int randY = random(min, GRID_HEIGHT - borderVisible);
        position = Pixel(randX, randY);
    } while (snake.contains(position));
};

void Berry::render(CRGB leds[], int color) {
    position.render(leds, color);
};