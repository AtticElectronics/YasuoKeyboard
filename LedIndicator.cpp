// LedIndicator.cpp
#include "LedIndicator.h"

LedIndicator::LedIndicator(uint16_t numPixels, uint8_t pin) : pixels(numPixels, pin, NEO_GRB + NEO_KHZ800)
{
    pixels.begin();
    brightness = 0;
    brightnessStep = 1;

    isDecreasing = true;

    rainbowDensity = 51;
    rainbowIndex = 0;

    prevRed = 0;
    prevGreen = 0;
    prevBlue = 0;

    currRed = 0;
    currGreen = 0;
    currBlue = 0;

    count = numPixels;
}

void LedIndicator::setBrightnessStep(uint8_t step)
{
    brightnessStep = 0;
    brightnessStep = step;
}

void LedIndicator::run(uint8_t r, uint8_t g, uint8_t b)
{

    currRed = r;
    currGreen = g;
    currBlue = b;

    adjustBrightness();
    // Serial.println(brightness);

    if (prevRed == 0 && prevGreen == 0 && prevBlue == 1)
    {

        // rainbow
        for (uint16_t i = 0; i < count; i++)
        {
            uint8_t pos = (i * rainbowDensity + rainbowIndex) % 256;
            uint32_t color = wheel(pos);
            uint8_t r = (uint8_t)(color >> 16);
            uint8_t g = (uint8_t)(color >> 8);
            uint8_t b = (uint8_t)color;
            r = (r * brightness) / 255;
            g = (g * brightness) / 255;
            b = (b * brightness) / 255;
            pixels.setPixelColor(i, pixels.Color(r, g, b));
        }
        if (!isDecreasing)
        {

            rainbowIndex = (rainbowIndex + 1) % 256;
        }
    }
    else
    {

        for (uint16_t i = 0; i < count; i++)
        {
            uint8_t r = (prevRed * brightness) / 255;
            uint8_t g = (prevGreen * brightness) / 255;
            uint8_t b = (prevBlue * brightness) / 255;
            pixels.setPixelColor(i, pixels.Color(r, g, b));
        }
    }
    pixels.show();
}

void LedIndicator::adjustBrightness()
{
    if (!((prevRed == currRed) && (prevGreen == currGreen) && (prevBlue == currBlue)))
    {
        isDecreasing = true;
    }
    if (isDecreasing)
    {
        brightness -= brightnessStep;
        if (brightness <= 0)
        {
            brightness = 0;
            isDecreasing = false;
            prevRed = currRed;
            prevGreen = currGreen;
            prevBlue = currBlue;
        }
    }
    else
    {
        brightness += brightnessStep;
        if (brightness >= 255)
        {
            brightness = 255;
        }
    }
}

uint32_t LedIndicator::wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}