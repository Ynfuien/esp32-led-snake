#include "menus.h"

BrightnessScreen::BrightnessScreen() {
    type = BRIGHTNESS_SCREEN;

    for (int i = 0; i < 3; i++) {
        pixels.push_back(Pixel(i + 3, 3 - i));
        pixels.push_back(Pixel(i + 7, 1 + i));

        pixels.push_back(Pixel(i + 3, 5 + i));
        pixels.push_back(Pixel(i + 7, 7 - i));

        for (int j = 0; j < 3; j++) {
            if ((j + i) % 2 == 0) continue;
            pixels.push_back(Pixel(5 + j, 3 + i));
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if ((j + i) % 2 == 0) continue;
            secondFramePixels.push_back(Pixel(4 + j, 2 + i));
        }
    }   
};

void BrightnessScreen::render(CRGB leds[], Settings* settings, bool frameTick) {
    std::list<Pixel> framePixels = pixels;
        
    if (frameTick) frame = !frame;
    if (!frame) framePixels = secondFramePixels;

    for (Pixel p : framePixels) {
        p.render(leds, 0xffffff);
    }

    renderLevel(leds, settings);
};

void BrightnessScreen::renderLevel(CRGB leds[], Settings* settings) {
    int level = settings->ledBrightness;

    for (int i = 1; i < level + 2; i++) {
        Pixel(0, GRID_HEIGHT - i).render(leds, 0xffffff);
    }
};

void BrightnessScreen::changeSetting(SnakeDirection direction, Settings* settings) {
    if (direction == Up) {
        short level = settings->ledBrightness;
        level++;
        if (level > MAX_SETTING_LEVEL) level = MAX_SETTING_LEVEL;

        FastLED.setBrightness(BRIGHTNESS_VALUES[level]);
        settings->ledBrightness = level;
        return;
    }

    if (direction == Down) {
        short level = settings->ledBrightness;
        level--;
        if (level < 0) level = 0;

        FastLED.setBrightness(BRIGHTNESS_VALUES[level]);
        settings->ledBrightness = level;
    }
};