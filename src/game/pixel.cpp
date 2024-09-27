#include "game.h"

Pixel::Pixel(int x, int y) {
    this->x = x >= GRID_WIDTH ? GRID_WIDTH - 1 : x;
    this->y = y >= GRID_HEIGHT ? GRID_HEIGHT - 1 : y;
};

// THE method that's responsible for accurate representation of
// 2D pixels on a 1D led stripe (that's obviously setup to look like 2D plane).
// It this case, it's written to work with a matrix of leds that goes like this:
//   > > > > V
//   V < < < <
//   > > > > V
//   V < < < <
//   > > > > >
// From left to right, to bottom, then right to left, bottom and so on.
// You need to change this code however is needed, to work with a matrix setup
// that you have.
void Pixel::render(CRGB leds[], CRGB color) {
    int ledNumber = y * GRID_WIDTH;
    
    // If it's even, LED line goes from left to right,
    // if it's odd (1, 3, 5), line goes from right to left.
    ledNumber += (y % 2 == 0) ? x : (GRID_WIDTH - x - 1);

    leds[ledNumber] = color;
};

bool Pixel::equals(Pixel pixel) {
    return pixel.x == x && pixel.y == y;
};