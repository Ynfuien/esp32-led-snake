#include "game.h"

Snake::Snake(int snakeSize) {
    // Creates snake more or less on the center of the grid
    headPixel = Pixel(GRID_WIDTH / 2 + (snakeSize / 2) - 2, GRID_HEIGHT / 2 - 1);

    for (int i = snakeSize - 1; i > 0; i--) {
        bodyPixels.push_back(Pixel(headPixel.x - i, headPixel.y));
    }
};

void Snake::render(CRGB leds[], int headColor, int bodyColor) {
    headPixel.render(leds, headColor);

    // Renders snake's body.
    // Why so complicated?
    // Cause we doin' gradient >.>
    int bodySize = bodyPixels.size();
    int brightness = FastLED.getBrightness();
    // double step = (1.0 / 255 * brightness) / bodySize;
    double step = 1.0 / bodySize;

    // Serial.printf("Brightness: %d; Calc: %f; Step: %f\n", brightness, 1.0 / 255 * brightness, step);
    // Serial.printf("Brightness: %d; Step: %f\n", brightness, 1.0 / 255 * brightness, step);

    char red1 = (bodyColor >> 16) & 0xFF;
    char green1 = (bodyColor >> 8) & 0xFF;
    char blue1 = (bodyColor >> 0) & 0xFF; 

    char red2 = 1;
    char green2 = 0;
    char blue2 = 1;

    double current = 0.9;
    for (Pixel p : bodyPixels) {
        current -= step;
        if (current < 0) current = 0;
        
        double resultRed = red1 + current * (red2 - red1);
        double resultGreen = green1 + current * (green2 - green1);
        double resultBlue = blue1 + current * (blue2 - blue1);

        // if (bodyPixels.front().equals(p)) {
        //     Serial.printf("R: %f, G: %f, B: %f\n", resultRed, resultGreen, resultBlue);
        // }
        
        p.render(leds, CRGB(resultRed, resultGreen, resultBlue));
    }

    // double current = 0;
    // for (std::list<Pixel>::reverse_iterator it = bodyPixels.rbegin(); !it->equals(bodyPixels.front()); it++) {
    //     current += step;
        
    //     double resultRed = red1 + current * (red2 - red1);
    //     double resultGreen = green1 + current * (green2 - green1);
    //     double resultBlue = blue1 + current * (blue2 - blue1);

    //     if (bodyPixels.front().equals(*it)) {
    //         Serial.printf("R: %f, G: %f, B: %f\n", resultRed, resultGreen, resultBlue);
    //     }
        
    //     it->render(leds, CRGB(resultRed, resultGreen, resultBlue));
    // }

    // double current = 0;
    // std::list<Pixel>::reverse_iterator it = bodyPixels.rbegin();
    // while (true) {
    //     current += step;
        
    //     double resultRed = red1 + current * (red2 - red1);
    //     double resultGreen = green1 + current * (green2 - green1);
    //     double resultBlue = blue1 + current * (blue2 - blue1);

    //     if (bodyPixels.front().equals(*it)) {
    //         Serial.printf("R: %f, G: %f, B: %f\n", resultRed, resultGreen, resultBlue);
    //     }
        
    //     it->render(leds, CRGB(resultRed, resultGreen, resultBlue));

    //     if (it->equals(bodyPixels.front())) break;
    //     it++;
    // }

    // 

    std::list<Pixel>::iterator it2 = bodyPixels.begin();
    it2++;

    it2->render(leds, CRGB(0, 255, 0));
};

bool Snake::move(SnakeDirection direction, Border border, bool phasing, bool testing = false) {
    int x = headPixel.x;
    int y = headPixel.y;

    // Move head in provided direction
    if (direction == Up) y--;
    else if (direction == Right) x++;
    else if (direction == Down) y++;
    else if (direction == Left) x--;
    
    // Check if we ain't goin' outside the grid
    if (!phasing) {
        if (x < 0 || x >= GRID_WIDTH) return false;
        if (y < 0 || y >= GRID_HEIGHT) return false;
    } else { // Or move the head to the other side, if phasing is enabled
        if (x < 0) x = GRID_WIDTH - 1;
        else if (x > GRID_WIDTH - 1) x = 0;

        if (y < 0) y = GRID_HEIGHT - 1;
        else if (y > GRID_HEIGHT - 1) y = 0;
    }


    // Create pixel for the new head position
    Pixel newHead = Pixel(x, y);
    // Check if we didn't hit ourself
    if (this->contains(newHead)) {
        // If we hit our tail, then it's fine
        if (!bodyPixels.front().equals(newHead)) return false;
        
        // Check if snake didn't grow a second before.
        // Because then tail does have 2 pixels, and it's not fine
        std::list<Pixel>::iterator it = bodyPixels.begin();
        it++;

        if (it->equals(newHead)) return false;
    }
    // Check for the border
    if (border.contains(newHead)) return false;


    if (testing) return true;
    
    // Everything is good, so
    // we add old head to the body pixels
    bodyPixels.push_back(headPixel);
    // Remove pixel at a snake's tail
    bodyPixels.pop_front();
    // And set new head
    headPixel = newHead;
    // Making for a great "move" ilusion ;p

    return true;
}

bool Snake::fakeMove(SnakeDirection direction, bool phasing) {
    int x = headPixel.x;
    int y = headPixel.y;

    if (direction == Up)
        y--;
    else if (direction == Right)
        x++;
    else if (direction == Down)
        y++;
    else if (direction == Left)
        x--;

    if (!phasing) {
        if (x < 0 || x >= GRID_WIDTH) return false;
        if (y < 0 || y >= GRID_HEIGHT) return false;
    } else {
        if (x < 0) x = GRID_WIDTH - 1;
        else if (x > GRID_WIDTH - 1) x = 0;

        if (y < 0) y = GRID_HEIGHT - 1;
        else if (y > GRID_HEIGHT - 1) y = 0;
    }

    Pixel newHead = Pixel(x, y);

    bodyPixels.push_back(headPixel);
    bodyPixels.pop_front();
    headPixel = newHead;
    return true;
}

void Snake::grow() {
    // Add a new pixel at snake's butt
    Pixel newBody = Pixel(bodyPixels.front().x, bodyPixels.front().y);
    bodyPixels.push_front(newBody);
};

int Snake::getSize() { return bodyPixels.size() + 1; }

bool Snake::contains(Pixel pixel) {
    if (headPixel.equals(pixel)) return true;
    for (Pixel p : bodyPixels) {
        if (p.equals(pixel)) return true;
    }

    return false;
};