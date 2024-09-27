#include "menus.h"

PlayScreen::PlayScreen() {
    type = PLAY_SCREEN;

    for (int i = 0; i < 3; i++) {
        for (int j = 2 + i; j < 7 - i; j++) {
            pixels.push_back(Pixel(i + 4, j));
        }
    }     
};

void PlayScreen::render(CRGB leds[], Settings* settings, bool frameTick) {
    for (Pixel p : pixels) {
        p.render(leds, 0x00ff00);
    }
}