#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include <list>

// Configuration variables
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;
const int SNAKE_START_SIZE = 4;
const int TICK_TIME = 50;

// Game colors
const int GCOLOR_BACKGROUND = 0x000000;
const int GCOLOR_SNAKEHEAD = 0x2707de;
// const int GCOLOR_SNAKEBODY = 0xc240c9;
// const int GCOLOR_SNAKEBODY = 0x20fc03;
const int GCOLOR_SNAKEBODY = 0xff00ff;
const int GCOLOR_BERRY = 0xff0000;
const int GCOLOR_BORDER = 0xffaa00;
const int GCOLOR_GAMEOVER = 0xff5555;
const int GCOLOR_SCORE = 0xffff55;
const int GCOLOR_SCORENUMBER = 0xffaa00;


enum GameState { IDLE, MENU, PLAYING, GAMEOVER, WIN };

// Game classes
class Pixel {
   public:
    int x;
    int y;

    Pixel() {};

    Pixel(int x, int y);

    void render(CRGB leds[], CRGB color);
    bool equals(Pixel pixel);
};

class Border {
   private:
    std::list<Pixel> borderPixels;

   public:
    Border(bool visible);

    void render(CRGB leds[], int color);
    bool contains(Pixel pixel);
};

enum SnakeDirection { Up, Right, Left, Down, None };

class Snake {
   private:
    std::list<Pixel> bodyPixels;
    Pixel headPixel;

   public:
    Snake(int snakeSize);

    void render(CRGB leds[], int headColor, int bodyColor);

    bool move(SnakeDirection direction, Border border, bool phasing, bool testing);
    bool fakeMove(SnakeDirection direction, bool phasing);;

    void grow();
    int getSize();
    bool contains(Pixel pixel);
};

class Berry {
   private:
    bool borderVisible;

   public:
    Pixel position;

    Berry(bool borderVisible);

    void spawn(Snake snake);

    void render(CRGB leds[], int color);
};