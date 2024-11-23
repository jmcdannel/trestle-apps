
#ifndef LED_h
#define LED_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class LED
{
public:
  LED(int pin, int numPixels);
  // MARK: Lifecycle
  void begin();
  void loop();
  void setPattern(char pattern[20], char range[20], char config[20]);
  void turnOff();
  void setBrightness(uint8_t brightness);

private:
  Adafruit_NeoPixel strip;

  unsigned long pixelPrevious = 0;   // Previous Pixel Millis
  unsigned long patternPrevious = 0; // Previous Pattern Millis

  int pixelInterval = 50;   // Pixel Interval (ms)
  int pixelQueue = 0;       // Pattern Pixel Queue
  int pixelCycle = 0;       // Pattern Pixel Cycle
  uint16_t pixelNumber = 0; // Total Number of Pixels

  char* pattern;
  char* range;
  char* config;
  uint32_t color;
  int r;
  int g;
  int b;
  int start;
  int end;
  uint16_t j;
  uint32_t Wheel(byte WheelPos);
  void wipe();
  void updatePattern();
  void setColor();
  void rainbow(uint8_t wait);
  void theaterChase(int wait);
  void theaterChaseRainbow(uint8_t wait);
  void colorWipe(int wait);
  // Debuging Identifier

  // Private constructor to prevent direct instantiation
};

#endif
