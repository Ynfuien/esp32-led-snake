#pragma once

#define MAX_SETTING_LEVEL 9
const short SNAKE_SPEED_VALUES[] = {19, 17, 15, 13, 11, 9, 7, 5, 3, 2};
const unsigned char BRIGHTNESS_VALUES[] = {10, 25, 50, 75, 100, 125, 150, 175, 200, 255};

typedef struct Settings {
    long lastBuildTime = BUILD_TIME;

    bool borderPhasing = true;
    bool borderVisible = false;

    short ledBrightness = 4;
    short snakeSpeed = 7;
} Settings;