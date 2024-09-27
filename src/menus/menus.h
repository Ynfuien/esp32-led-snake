#pragma once

#include <list>
#include "../game/game.h"
#include "settings.cpp"

enum ScreenType {
    PLAY_SCREEN,
    BRIGHTNESS_SCREEN,
    SNAKE_SPEED_SCREEN,
    BORDER_SCREEN,
    BORDER_PHASING_SCREEN,
    WIN_SCREEN
};

class Screen {
   public:
    ScreenType type;
    std::list<Pixel> pixels;
    int frame;

    virtual void render(CRGB leds[], Settings* settings, bool frameTick);
};

class SettingScreen : public Screen {
   public:
    virtual void renderLevel(CRGB leds[], Settings* settings);

    virtual void changeSetting(SnakeDirection direction, Settings* settings);
};

class PlayScreen : public Screen {
   public:

    PlayScreen();

    void render(CRGB leds[], Settings* settings, bool frameTick);
};

class BrightnessScreen : public SettingScreen {
   private: 
    std::list<Pixel> secondFramePixels;

   public:

    BrightnessScreen();

    void render(CRGB leds[], Settings* settings, bool frameTick);

    void renderLevel(CRGB leds[], Settings* settings);

    void changeSetting(SnakeDirection direction, Settings* settings);
};

class SnakeSpeedScreen : public SettingScreen {
   private:
    Snake snake = Snake(8);

    void moveTick();

   public:

    SnakeSpeedScreen();

    void render(CRGB leds[], Settings* settings, bool frameTick);

    void renderLevel(CRGB leds[], Settings* settings);

    void changeSetting(SnakeDirection direction, Settings* settings);
};

class BorderScreen : public SettingScreen {
   private:
    std::list<Pixel> invisibleBorderPixels;

   public:

    BorderScreen();

    void render(CRGB leds[], Settings* settings, bool frameTick);

    void changeSetting(SnakeDirection direction, Settings* settings);
};

class BorderPhasingScreen : public SettingScreen {
   private:
    Snake phasingSnake = Snake(6);
    Snake normalSnake = Snake(6);
    SnakeDirection nextSnakeDirection = Right;

   public:

    BorderPhasingScreen();

    void render(CRGB leds[], Settings* settings, bool frameTick);

    void changeSetting(SnakeDirection direction, Settings* settings);
};



class WinScreen : public Screen {
   private:
    std::list<Pixel> sparkFrames[8];
    std::list<Pixel> heartFrames[2];

    void addMirrorPixels(std::list<Pixel> *pixels, int x, int y);

    void addDoubleMirrorPixels(std::list<Pixel> *pixels, int x, int y);

   public:
    bool showingSpark = true;

    WinScreen();

    void render(CRGB leds[], Settings* settings, bool frameTick);
};