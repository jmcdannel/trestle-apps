
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
    void setColor(int r, int g, int b);
    void setBrightness(uint8_t brightness);
  private:
    // MARK: Variables
    Adafruit_NeoPixel strip;
    int _numPixels;
    // Debuging Identifier

    // Private constructor to prevent direct instantiation
    
};

#endif
