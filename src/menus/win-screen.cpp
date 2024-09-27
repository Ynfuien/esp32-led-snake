#include "menus.h"

WinScreen::WinScreen() {
    type = WIN_SCREEN;

    frame = -1;

    // Hardcoded "animation" frames
    for (int i = 0; i < 8; i++) sparkFrames[i] = std::list<Pixel>();

    // First frame
    addDoubleMirrorPixels(&sparkFrames[0], 4, 4);

    // Second
    addDoubleMirrorPixels(&sparkFrames[1], 3, 3);
    

    // Third
    for (int i = 2; i < 4; i++) {
        addDoubleMirrorPixels(&sparkFrames[2], i, 2);
        addDoubleMirrorPixels(&sparkFrames[2], i, 3);
    }

    // Fourth
    addDoubleMirrorPixels(&sparkFrames[3], 2, 2);
    addDoubleMirrorPixels(&sparkFrames[3], 2, 3);
    addDoubleMirrorPixels(&sparkFrames[3], 3, 2);

    // Fifth
    for (int i = 1; i < 4; i++) {
        addDoubleMirrorPixels(&sparkFrames[4], i, 1);
    }
    addDoubleMirrorPixels(&sparkFrames[4], 1, 2);
    addDoubleMirrorPixels(&sparkFrames[4], 1, 3);

    // Sixth
    for (int i = 0; i < 5; i+=2) {
        addDoubleMirrorPixels(&sparkFrames[5], i, 0);
    }
    addDoubleMirrorPixels(&sparkFrames[5], 0, 2);
    addDoubleMirrorPixels(&sparkFrames[5], 0, 4);

    // Seventh
    sparkFrames[6] = std::list<Pixel>(sparkFrames[5]);

    // Append fifth frame onto sixth
    for (Pixel p : sparkFrames[4]) {
        sparkFrames[5].push_back(p);
    }


    //// Heart frames
    // First
    addMirrorPixels(&heartFrames[0], 1, 2);
    addMirrorPixels(&heartFrames[0], 1, 3);
    addMirrorPixels(&heartFrames[0], 1, 4);
    
    addMirrorPixels(&heartFrames[0], 2, 5);
    addMirrorPixels(&heartFrames[0], 3, 6);
    addMirrorPixels(&heartFrames[0], 4, 7);

    addMirrorPixels(&heartFrames[0], 2, 1);
    addMirrorPixels(&heartFrames[0], 3, 1);
    addMirrorPixels(&heartFrames[0], 4, 2);

    // Second
    addMirrorPixels(&heartFrames[1], 0, 2);
    addMirrorPixels(&heartFrames[1], 0, 3);
    addMirrorPixels(&heartFrames[1], 0, 4);

    for (int i = 0; i < 4; i++) {
        addMirrorPixels(&heartFrames[1], 1 + i, 5 + i);
    }

    addMirrorPixels(&heartFrames[1], 1, 1);
    addMirrorPixels(&heartFrames[1], 2, 0);
    addMirrorPixels(&heartFrames[1], 3, 0);
    addMirrorPixels(&heartFrames[1], 4, 1);
};

void WinScreen::addMirrorPixels(std::list<Pixel> *pixels, int x, int y) {
    pixels->push_front(Pixel(x, y));
    pixels->push_front(Pixel(GRID_WIDTH - 1 - x, y));
};

void WinScreen::addDoubleMirrorPixels(std::list<Pixel> *pixels, int x, int y) {
    pixels->push_front(Pixel(x, y));
    pixels->push_front(Pixel(GRID_WIDTH - 1 - x, y));

    pixels->push_front(Pixel(x, GRID_HEIGHT - 1 - y));
    pixels->push_front(Pixel(GRID_WIDTH - 1 - x, GRID_HEIGHT - 1 - y));
};

void WinScreen::render(CRGB leds[], Settings* settings, bool frameTick) {
    if (showingSpark) {
        if (frameTick) {
            frame++;

            if (frame > 7) {
                frame = 0;
                showingSpark = false;
                return;
            }
        }

        std::list<Pixel> pixels = sparkFrames[frame];
        for (Pixel p : pixels) {
            p.render(leds, GCOLOR_BORDER);
        }

        return;
    }
    
    if (frameTick) frame = !frame;
    std::list<Pixel> pixels = heartFrames[frame];
    for (Pixel p : pixels) {
        p.render(leds, GCOLOR_BERRY);
    }
};