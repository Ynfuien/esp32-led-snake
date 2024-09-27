#pragma once

//// Hardware configuration

// Joystick
#define BUTTON_PIN 5
#define ANALOG_X_PIN A3
#define ANALOG_Y_PIN A0
#define ANALOG_STEP 600

#define RANDOM_SEED_ANALOG_PIN A6

// Serial
#define SERIAL_BAUD 115200

// Leds
#define LED_TYPE WS2812B
#define LED_ORDER GRB
#define LED_DATA_PIN 4
#define LED_NUMBER GRID_WIDTH * GRID_HEIGHT