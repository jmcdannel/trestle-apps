#include <Arduino.h>
#include "LED.h"
#include <Adafruit_NeoPixel.h>

LED::LED(int pin, int numPixels) {
  this->pixelNumber = 0;
  this->pixelNumber = numPixels;
  this->strip = Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);
}

void LED::begin() {
  this->strip.begin();
  this->turnOff(); // Initialize all pixels to off
}

void LED::loop() {
  unsigned long currentMillis = millis();    
  if(this->pattern > 1 && currentMillis - this->pixelPrevious >= this->pixelInterval) {        //  Check for expired time
    this->pixelPrevious = currentMillis;                            //  Run current frame
    this->updatePattern();
  }
}

void LED::setPattern(int ledParams[5]) {
  this->pattern = ledParams[1];
  Serial.print("setPattern:");
  Serial.println(pattern);
  // this->wipe();
  // this->j = 0;
  switch(pattern) {
      case 0:
        this->turnOff();
        break;
      case 1:
        this->color = this->strip.Color(ledParams[2],  ledParams[3],  ledParams[4]);
        this->setColor(this->color);
        Serial.println("set to new color");
        break;
      case 2:
        this->rainbow(10);
        break;
      case 3:
        this->theaterChaseRainbow(50);
        break;
      case 4:
        this->color = this->strip.Color(ledParams[2],  ledParams[3],  ledParams[4]);
        this->theaterChase(this->color, 50); 
        break;
      case 5:
        this->color = this->strip.Color(ledParams[2],  ledParams[3],  ledParams[4]);
         this->colorWipe(this->color, 50); 
         break;
      default:
        // this->setColor(r, g, b);
        // this->lastUpdate = millis() + 100;
        break;
    }
}

void LED::updatePattern() { 
  switch(pattern) {
      case 2:
        this->rainbow(10);
        break;
      case 3:
        this->theaterChaseRainbow(50);
        break;
      case 4:
        this->theaterChase(this->color, 50);  
        break;
      case 5:
         this->colorWipe(this->color, 50); 
         break;
      default:
        break;
    }
}

void LED::setColor(uint32_t color) {
  Serial.print("setColor");
  Serial.print(this->pixelNumber);
  Serial.print(r);
  Serial.print(g);
  Serial.println(b);
  for(int i=0; i < this->pixelNumber; i++) {
    this->strip.setPixelColor(i, this->color);
  }
  this->strip.show();
  // this->lastUpdate = millis() + 100;
}

void LED::turnOff() {
  this->pattern = 0;
  for(int i=0; i < this->pixelNumber; i++) {
    this->strip.setPixelColor(i, this->strip.Color(0, 0, 0));
  }
  this->strip.show();
}

void LED::rainbow(uint8_t wait) {
  // Serial.print("rainbow");
  if(this->pixelInterval !=wait)
    this->pixelInterval = wait;                   
  for(uint16_t i=0; i < this->pixelNumber; i++) {
    strip.setPixelColor(i, Wheel((i + this->pixelCycle) & 255)); //  Update delay time  
  }
  strip.show();                             //  Update strip to match
  this->pixelCycle++;                             //  Advance current cycle
  if(this->pixelCycle >= 256)
    this->pixelCycle = 0;                         //  Loop the cycle back to the begining

}

void LED::theaterChase(uint32_t color, int wait) {
  static uint32_t loop_count = 0;
  static uint16_t current_pixel = 0;

  this->pixelInterval = wait;                   //  Update delay time

  this->strip.clear();

  for(int c=current_pixel; c < this->pixelNumber; c += 3) {
    strip.setPixelColor(c, color);
  }
  this->strip.show();

  current_pixel++;
  if (current_pixel >= 3) {
    current_pixel = 0;
    loop_count++;
  }

  if (loop_count >= 10) {
    current_pixel = 0;
    loop_count = 0;
  }
}

void LED::theaterChaseRainbow(uint8_t wait) { // modified from Adafruit example to make it a state machine
  if(this->pixelInterval != wait)
    this->pixelInterval = wait;                   //  Update delay time  
  for(int i=0; i < this->pixelNumber; i+=3) {
    this->strip.setPixelColor(i + this->pixelQueue, Wheel((i + this->pixelCycle) % 255)); //  Update delay time  
  }
  this->strip.show();
  for(int i=0; i < this->pixelNumber; i+=3) {
    strip.setPixelColor(i + this->pixelQueue, strip.Color(0, 0, 0)); //  Update delay time  
  }      
  this->pixelQueue++;                           //  Advance current queue  
  this->pixelCycle++;                           //  Advance current cycle
  if(this->pixelQueue >= 3)
    this->pixelQueue = 0;                       //  Loop
  if(this->pixelCycle >= 256)
    this->pixelCycle = 0;                       //  Loop
}

void LED::colorWipe(uint32_t color, int wait) {
  static uint16_t current_pixel = 0;
  this->pixelInterval = wait;                        //  Update delay time
  this->strip.setPixelColor(current_pixel++, color); //  Set pixel's color (in RAM)
  this->strip.show();                                //  Update strip to match
  if(current_pixel >= this->pixelNumber) {           //  Loop the pattern from the first LED
    current_pixel = 0;
  }
}


void LED::wipe(){ // clear all LEDs
  for(int i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i, strip.Color(0,0,0)); 
  }
}

uint32_t LED::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
