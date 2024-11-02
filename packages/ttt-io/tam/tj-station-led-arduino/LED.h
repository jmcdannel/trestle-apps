
#ifndef LED_h
#define LED_h
// MARK: Libraries
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
// MARK: Class / Functions / Variables
class LED {
  public:
    LED(int pin, int numPixels);
    // MARK: Lifecycle
    void begin();
    void loop();
    void setPattern(int ledParams[5]);
    void turnOff();
    void setBrightness(uint8_t brightness);
  private:
    // MARK: Variables
    Adafruit_NeoPixel strip;

    unsigned long pixelPrevious = 0;        // Previous Pixel Millis
    unsigned long patternPrevious = 0;      // Previous Pattern Millis

    int           pixelInterval = 50;       // Pixel Interval (ms)
    int           pixelQueue = 0;           // Pattern Pixel Queue
    int           pixelCycle = 0;           // Pattern Pixel Cycle
    uint16_t      pixelNumber = 0;  // Total Number of Pixels
    
    int pattern;
    uint32_t color;
    int r;
    int g;
    int b;
    uint16_t j;    
    uint32_t Wheel(byte WheelPos);
    void wipe();
    void updatePattern();
    void setColor(uint32_t color);
    void rainbow(uint8_t wait);
    void theaterChase(uint32_t color, int wait);
    void theaterChaseRainbow(uint8_t wait);
    void colorWipe(uint32_t color, int wait);
    // Debuging Identifier

    // Private constructor to prevent direct instantiation
    
};

#endif
