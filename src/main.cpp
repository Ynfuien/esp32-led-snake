#include <Arduino.h>
#include <FastLED.h>
#include <Preferences.h>
#include <list>
#include <vector>
#include <iterator>

#include "game/game.h"
#include "hardware.h"
#include "settings.cpp"
#include "menus/menus.h"


// Analog neutral position. Mesured on the startup.
// 1850 is just value for my joystick
int analogNeutral = 1850;

// Array of leds
CRGB leds[LED_NUMBER];

// Game variables
Border border = NULL;
Snake snake = Snake(SNAKE_START_SIZE);
Berry berry = Berry(true);
SnakeDirection direction = Right;
SnakeDirection newDirection = direction;
GameState gameState = IDLE;
// GameState gameState = WIN;
int maxSnakeSize = GRID_WIDTH * GRID_HEIGHT;

std::vector<Screen*> menuScreens;
int currentScreen = 0;
WinScreen winScreen;

// Settings
Preferences preferences;
Settings settings;


// Method definitions
void prepareSettings();
void saveSettings();
void setupGame();

void checkKeys();
SnakeDirection getAnalogDirection();
bool tick();
void renderGame();

void loop();

void idleChangeDirection();

void idleLoop(bool buttonPressed);
void menuLoop(bool buttonPressed);
void gameLoop();
void gameOverLoop(bool buttonPressed);
void winLoop(bool buttonPressed);


void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("Starting...");
    Serial.printf("Build time: %d\n", BUILD_TIME);
    
    prepareSettings();

    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_ORDER>(leds, LED_NUMBER);
    FastLED.clear(true);
    FastLED.setBrightness(BRIGHTNESS_VALUES[settings.ledBrightness]);


    randomSeed(analogRead(RANDOM_SEED_ANALOG_PIN));

    pinMode(LED_DATA_PIN, OUTPUT);
    pinMode(ANALOG_X_PIN, INPUT);
    pinMode(ANALOG_Y_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    analogNeutral = analogRead(ANALOG_X_PIN);


    setupGame();
    
    menuScreens.push_back(new PlayScreen());
    
    menuScreens.push_back(new BrightnessScreen());
    menuScreens.push_back(new SnakeSpeedScreen());
    menuScreens.push_back(new BorderScreen());
    menuScreens.push_back(new BorderPhasingScreen());

    Serial.printf("Menu screens: %d\n", menuScreens.size());

    Screen* currentMenu = menuScreens.at(currentScreen);
    Serial.printf("Current menu: %d\n", currentMenu->type);
};


void prepareSettings() {
    preferences.begin("settings", false);

    if (preferences.isKey("last-build-time")) {
        // long lastBuildTime = preferences.getLong("last-build-time");
        // if (lastBuildTime == BUILD_TIME) {

            settings.borderPhasing = preferences.getBool("border-phasing");
            settings.borderVisible = preferences.getBool("border-visible");

            settings.snakeSpeed = preferences.getUShort("snake-speed");
            settings.ledBrightness = preferences.getUShort("led-brightness");

            return;
        // }

        // Serial.println("Build time is different. Clearing settings...");
        // preferences.clear();
    }
    
    saveSettings();
};

void saveSettings() {
    preferences.putLong("last-build-time", settings.lastBuildTime);

    preferences.putBool("border-phasing", settings.borderPhasing);
    preferences.putBool("border-visible", settings.borderVisible);

    preferences.putUShort("snake-speed", settings.snakeSpeed);
    preferences.putUShort("led-brightness", settings.ledBrightness);
};

void setupGame() {
    border = Border(settings.borderVisible);
    snake = Snake(SNAKE_START_SIZE);
    berry = Berry(settings.borderVisible);
    berry.spawn(snake);
    direction = Right;
    newDirection = direction;

    maxSnakeSize = settings.borderVisible ? (GRID_WIDTH - 2) * (GRID_HEIGHT - 2) : GRID_WIDTH * GRID_HEIGHT;
};


unsigned long now;
unsigned long lastTickTime = 0;
int gameTick = 100;
bool gameOverFrame = false;
bool lastButtonState = false;
bool changedSettings = false;
unsigned long lastInputTimestamp;

SnakeDirection lastJoystickDirection = None;
void loop() {
    now = millis();
    if (now - lastTickTime >= TICK_TIME) {
        gameTick++;
        lastTickTime = now;
    }
    
    bool buttonPressed = false;
    // True for pressed, false otherwise
    bool buttonState = !digitalRead(BUTTON_PIN);
    if (lastButtonState != buttonState) {
        lastButtonState = buttonState;
        buttonPressed = buttonState;
        
        lastInputTimestamp = now;
    }
    
    if (gameState == IDLE) {
        idleLoop(buttonPressed);
        return;
    }
    
    if (gameState == MENU) {
        // TICK_TIME(50) * 20 == 1 second
        if (now - lastInputTimestamp >= (TICK_TIME * 20 * 60)) {
            setupGame();
            gameState = IDLE;
            currentScreen = 0;
            return;
        }

        menuLoop(buttonPressed);
        return;
    }

    if (gameState == GAMEOVER) {
        gameOverLoop(buttonPressed);
        return;
    }

    if (gameState == WIN) {
        winLoop(buttonPressed);
        return;
    }

    if (gameState == PLAYING) {
        gameLoop();
    }
};

void idleLoop(bool buttonPressed) {
    if (buttonPressed) {
        gameState = MENU;
        return;
    }

    if (gameTick < SNAKE_SPEED_VALUES[settings.snakeSpeed]) return;
    gameTick = 0;

    idleChangeDirection();

    if (!tick()) {
        gameTick = 10;
        return;
    }

    renderGame();
    FastLED.show();
};

void idleChangeDirection() {
    for (int i = 0; i < 20; i++) {
        SnakeDirection randomDir = static_cast<SnakeDirection>(random(0, 4));
        if (newDirection == Up && randomDir == Down) continue;
        if (newDirection == Down && randomDir == Up) continue;
        if (newDirection == Right && randomDir == Left) continue;
        if (newDirection == Left && randomDir == Right) continue;

        if (!snake.move(randomDir, border, settings.borderPhasing, true)) continue;

        newDirection = randomDir;
        break;
    }
};

unsigned long joystickCooldown = 0;
void menuLoop(bool buttonPressed) {
    Screen* currentMenu = menuScreens.at(currentScreen);

    if (buttonPressed && currentMenu->type == PLAY_SCREEN) {
        setupGame();
        gameState = PLAYING;
        return;
    }

    SnakeDirection dir = getAnalogDirection();
    if (now - joystickCooldown >= TICK_TIME && dir != lastJoystickDirection) {
        lastJoystickDirection = dir;
        joystickCooldown = now;
        lastInputTimestamp = now;

        // Move to right/left menu
        if (dir == Left || dir == Right) {
            if (changedSettings) {
                changedSettings = false;
                saveSettings();
            }

            if (dir == Right) currentScreen++;
            else if (dir == Left) currentScreen--;

            int menuSize = menuScreens.size();
            if (currentScreen < 0) currentScreen = menuSize - 1;
            else if (currentScreen > menuSize - 1) currentScreen = 0;

            if (currentMenu->type != SNAKE_SPEED_SCREEN) currentMenu->frame = 0;
            currentMenu = menuScreens.at(currentScreen);

            if (settings.borderVisible && currentMenu->type == BORDER_PHASING_SCREEN) {
                if (dir == Left) currentScreen--;
                else if (dir == Right) currentScreen++;

                if (currentScreen < 0) currentScreen = menuSize - 1;
                else if (currentScreen > menuSize - 1) currentScreen = 0;

                currentMenu = menuScreens.at(currentScreen);
            }

            FastLED.clear();
            currentMenu->render(leds, &settings, true);
            FastLED.show();

            gameTick = 0;
            return;
        }

        if (dir != None && currentMenu->type != PLAY_SCREEN) {
            SettingScreen* settingScreen = dynamic_cast<SettingScreen*>(currentMenu);
            settingScreen->changeSetting(dir, &settings);

            changedSettings = true;

            FastLED.clear();
            currentMenu->render(leds, &settings, false);
            FastLED.show();
        }
    }


    if (currentMenu->type == SNAKE_SPEED_SCREEN || currentMenu->type == BORDER_PHASING_SCREEN) {
        if (gameTick < SNAKE_SPEED_VALUES[settings.snakeSpeed]) return;
        gameTick = 0;
    } else {
        if (gameTick < 10) return;
        gameTick = 0;
    }

    FastLED.clear();
    currentMenu->render(leds, &settings, true);
    FastLED.show();
};

void gameLoop() {
    checkKeys();

    if (gameTick < SNAKE_SPEED_VALUES[settings.snakeSpeed]) return;
    gameTick = 0;

    if (!tick()) {
        gameTick = 10;
        return;
    }

    renderGame();
    FastLED.show();
};

void gameOverLoop(bool buttonPressed) {
    if (buttonPressed) {
        gameState = MENU;
        return;
    }
    
    if (gameTick < 10) return;
    gameTick = 0;

    gameOverFrame = !gameOverFrame;
    if (gameOverFrame) {
        snake.render(leds, 0, 0);
        FastLED.show();

        return;
    }
    
    snake.render(leds, GCOLOR_SNAKEHEAD, GCOLOR_SNAKEBODY);
    FastLED.show();
};

void winLoop(bool buttonPressed) {
    if (buttonPressed) {
        gameState = MENU;
        winScreen.showingSpark = true;
        return;
    }

    if (gameTick < (winScreen.showingSpark ? 1 : 10)) return;
    gameTick = 0;

    FastLED.clear();
    winScreen.render(leds, &settings, true);
    FastLED.show();
};

void renderGame() {
    for (int i = 0; i < LED_NUMBER; i++) {
        leds[i] = GCOLOR_BACKGROUND;
    }

    border.render(leds, GCOLOR_BORDER);
    berry.render(leds, GCOLOR_BERRY);
    snake.render(leds, GCOLOR_SNAKEHEAD, GCOLOR_SNAKEBODY);
};


void checkKeys() {
    SnakeDirection dir = getAnalogDirection();
    if (dir == None) return;

    lastInputTimestamp = now;

    switch (dir) {
        case Up:
            if (direction == Down) break;
            newDirection = Up;
            break;
        case Down:
            if (direction == Up) break;
            newDirection = Down;
            break;
        case Left:
            if (direction == Right) break;
            newDirection = Left;
            break;
        case Right:
            if (direction == Left) break;
            newDirection = Right;
            break;
    }
};

SnakeDirection getAnalogDirection() {
    int xValue = analogRead(ANALOG_X_PIN);
    int yValue = analogRead(ANALOG_Y_PIN);


    SnakeDirection dir = None;
    if (xValue > analogNeutral + ANALOG_STEP)
        dir = Right;
    else if (xValue < analogNeutral - ANALOG_STEP)
        dir = Left;

    int xDiff = abs(analogNeutral - xValue);
    int yDiff = abs(analogNeutral - yValue);
    boolean xDiffBigger = xDiff > yDiff;

    if (yValue > analogNeutral + ANALOG_STEP) {
        if (!xDiffBigger) dir = Down;
    } else if (yValue < analogNeutral - ANALOG_STEP) {
        if (!xDiffBigger) dir = Up;
    }

    return dir;
};

bool tick() {
    direction = newDirection;

    // Move snake and check if it actually moved
    if (!snake.move(direction, border, settings.borderPhasing, false)) {
        // Game over
        if (gameState == IDLE) {
            setupGame();
            return false;
        }

        gameState = GAMEOVER;
        // gameOverTime = millis();
        return false;
    }
    

    // Check if snake got the berry
    if (snake.contains(berry.position)) {
        snake.grow();

        if (snake.getSize() >= maxSnakeSize) {
            gameState = WIN;
            return false;
        }

        berry.spawn(snake);
    }

    return true;
};