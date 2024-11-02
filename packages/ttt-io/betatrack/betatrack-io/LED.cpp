#include <Arduino.h>
#include "LED.h"
#include <Adafruit_NeoPixel.h>

// unsigned long patternInterval = 20 ; // time between steps in the pattern
// unsigned long lastUpdate = 0 ; // for millis() when last update occoured
// unsigned long intervals [] = { 20, 20, 50, 100 } ; // speed for each pattern
// const byte button = 2; // pin to connect button switch to between pin and ground

LED::LED(int pin, int numPixels) {
  this->_numPixels = numPixels;
  this->strip = Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);
}

void LED::begin() {
  this->strip.begin();
  this->strip.show(); // Initialize all pixels to off
}

void LED::setColor(int r, int g, int b) {
  for(int i=0; i < this->_numPixels; i++) {
    this->strip.setPixelColor(i, this->strip.Color(r, g, b));
  }
  this->strip.show();
}

void LED::setBrightness(uint8_t brightness) {
  this->strip.setBrightness(brightness);
  this->strip.show();
}
