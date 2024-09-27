#include "menus.h"

BorderScreen::BorderScreen() {
    type = BORDER_SCREEN;

    // Border in width
    for (int i = 0; i < GRID_WIDTH; i++) {
        pixels.push_back(Pixel(i, 0));
        pixels.push_back(Pixel(i, GRID_HEIGHT - 1));

        if (i % 2 == 0) invisibleBorderPixels.push_back(Pixel(i, 0));
        else invisibleBorderPixels.push_back(Pixel(i, GRID_HEIGHT - 1));
    }

    // Border in height
    for (int i = 1; i < GRID_HEIGHT - 1; i++) {
        pixels.push_back(Pixel(0, i));
        pixels.push_back(Pixel(GRID_WIDTH - 1, i));

    
        if (i % 2 == 0) invisibleBorderPixels.push_back(Pixel(0, i));
        else invisibleBorderPixels.push_back(Pixel(GRID_WIDTH - 1, i));
    }
};

void BorderScreen::render(CRGB leds[], Settings* settings, bool frameTick) {
    std::list<Pixel> pixelsToRender = pixels;

    if (frameTick) frame = !frame;
    if (!frame) return;

    if (!settings->borderVisible) pixelsToRender = invisibleBorderPixels;
    
    for (Pixel p : pixelsToRender) {
        p.render(leds, GCOLOR_BORDER);
    }
};

void BorderScreen::changeSetting(SnakeDirection direction, Settings* settings) {
    settings->borderVisible = !settings->borderVisible;
};