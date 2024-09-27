#include "game.h"

Border::Border(bool visible) {
    if (!visible) return;
    
    // Border in width
    for (int i = 0; i <= GRID_WIDTH - 1; i++) {
        borderPixels.push_back(Pixel(i, 0));
        borderPixels.push_back(Pixel(i, GRID_HEIGHT - 1));
    }

    // Border in height
    for (int i = 1; i <= GRID_HEIGHT - 1; i++) {
        borderPixels.push_back(Pixel(0, i));
        borderPixels.push_back(Pixel(GRID_WIDTH - 1, i));
    }
};

void Border::render(CRGB leds[], int color) {
    for (Pixel p : borderPixels) {
        p.render(leds, color);
    }
};

bool Border::contains(Pixel pixel) {
    for (Pixel p : borderPixels) {
        if (p.equals(pixel)) return true;
    }

    return false;
};