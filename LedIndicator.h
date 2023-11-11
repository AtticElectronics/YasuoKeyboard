// LedIndicator.h

#ifndef _LED_INDICATOR_H_
#define _LED_INDICATOR_H_

#include <Adafruit_NeoPixel.h>

class LedIndicator
{
public:
    LedIndicator(uint16_t numPixels, uint8_t pin);
    void run(uint8_t r, uint8_t g, uint8_t b);
    void run(const String &color);
    void runRainbow();
    void setBrightnessStep(uint8_t step);
    void setRainbowDensity(uint8_t density);

private:
    Adafruit_NeoPixel pixels;
    uint16_t count;
    int brightness;
    uint16_t brightnessStep;

    bool isIncreasing;
    bool isDecreasing;

    uint8_t rainbowDensity;
    uint8_t rainbowIndex;

    uint8_t prevRed;
    uint8_t prevGreen;
    uint8_t prevBlue;

    uint8_t currRed;
    uint8_t currGreen;
    uint8_t currBlue;

    void runCommon(uint32_t color);
    void adjustBrightness();
    uint32_t wheel(byte WheelPos);
};

#endif // _LED_INDICATOR_H_
