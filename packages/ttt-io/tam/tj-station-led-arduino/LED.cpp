#include <Arduino.h>
#include "LED.h"
#include <Adafruit_NeoPixel.h>
#include <textparser.h>

LED::LED(int pin, int numPixels)
{
  this->pixelNumber = numPixels;
  this->strip = Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);
}

void LED::begin()
{
  this->strip.begin();
  this->turnOff(); // Initialize all pixels to off
}

void LED::loop()
{
  unsigned long currentMillis = millis();
  if (strcmp(this->pattern, "") != 0 && currentMillis - this->pixelPrevious >= this->pixelInterval)
  {                                      //  Check for expired time
    this->pixelPrevious = currentMillis; //  Run current frame
    this->updatePattern();
  }
}

void LED::setPattern(char *pattern, char *range, char *config)
{
  Serial.println(pattern);
  Serial.println(range);
  Serial.println(config);
  this->pattern = pattern;
  this->range = range;
  this->config = config;

  Serial.print("setPattern:");
  Serial.println(this->pattern);

  this->start = 0;
  this->end = this->pixelNumber;
  if (strcmp(this->range, "all") != 0) 
  {
    TextParser rangeParser(":");
    int rangeParams[2];
    rangeParser.parseLine(this->range, rangeParams);
    this->start = rangeParams[0];
    this->end = rangeParams[1];
  }
  else
  {
    Serial.println("All");
  }

  Serial.print("Range ");
  Serial.print(this->start);
  Serial.print(" - ");
  Serial.println(this->end);
  Serial.println(this->range);

if (strcmp(this->pattern, "color") == 0 || strcmp(this->pattern, "chasecolor") == 0 || strcmp(this->pattern, "wipe") == 0)
{
  int rgbParams[3];
  TextParser configParser(":");
  configParser.parseLine(this->config, rgbParams);
  this->color = this->strip.Color(rgbParams[0], rgbParams[1], rgbParams[2]);
}

if (strcmp(this->pattern, "off") == 0)
{
  this->turnOff();
}
else if (strcmp(this->pattern, "color") == 0)
{
  this->setColor();
}
else if (strcmp(this->pattern, "rainbow") == 0)
{
  this->rainbow(10);
}
else if (strcmp(this->pattern, "rainbowchase") == 0)
{
  this->theaterChaseRainbow(50);
}
else if (strcmp(this->pattern, "colorchase") == 0)
{
  this->theaterChase(50);
}
else if (strcmp(this->pattern, "wipe") == 0)
{
  this->colorWipe(50);
}
}

void LED::updatePattern()
{
  if (strcmp(this->pattern, "rainbow") == 0)
  {
    this->rainbow(10);
  }
  else if (strcmp(this->pattern, "rainbowchase") == 0)
  {
    this->theaterChaseRainbow(50);
  }
  else if (strcmp(this->pattern, "colorchase") == 0)
  {
    this->theaterChase(50);
  }
  else if (strcmp(this->pattern, "wipe") == 0)
  {
    this->colorWipe(50);
  }
}

void LED::setColor()
{
  Serial.print("setColor");
  for (int i = this->start; i < this->end; i++)
  {
    this->strip.setPixelColor(i, this->color);
  }
  this->strip.show();
  // this->lastUpdate = millis() + 100;
}

void LED::turnOff()
{
  for (int i = this->start; i < this->end; i++)
  {
    this->strip.setPixelColor(i, this->strip.Color(0, 0, 0));
  }
  this->strip.show();
}

void LED::rainbow(uint8_t wait)
{
  // Serial.print("rainbow");
  if (this->pixelInterval != wait)
    this->pixelInterval = wait;
  for (uint16_t i = this->start; i < this->end; i++)
  {
    strip.setPixelColor(i, Wheel((i + this->pixelCycle) & 255)); //  Update delay time
  }
  strip.show();       //  Update strip to match
  this->pixelCycle++; //  Advance current cycle
  if (this->pixelCycle >= 256)
    this->pixelCycle = 0; //  Loop the cycle back to the begining
}

void LED::theaterChase(int wait)
{
  static uint32_t loop_count = 0;
  static uint16_t current_pixel = this->start;

  this->pixelInterval = wait; //  Update delay time

  this->strip.clear();

  for (int c = current_pixel; c < end; c += 3)
  {
    strip.setPixelColor(c, this->color);
  }
  this->strip.show();

  current_pixel++;
  if (current_pixel >= 3)
  {
    current_pixel = 0;
    loop_count++;
  }

  if (loop_count >= 10)
  {
    current_pixel = 0;
    loop_count = 0;
  }
}

void LED::theaterChaseRainbow(uint8_t wait)
{ // modified from Adafruit example to make it a state machine
  if (this->pixelInterval != wait)
    this->pixelInterval = wait; //  Update delay time
  for (int i = this->start; i < this->end; i += 3)
  {
    this->strip.setPixelColor(i + this->pixelQueue, Wheel((i + this->pixelCycle) % 255)); //  Update delay time
  }
  this->strip.show();
  for (int i = this->start; i < this->end; i += 3)
  {
    strip.setPixelColor(i + this->pixelQueue, strip.Color(0, 0, 0)); //  Update delay time
  }
  this->pixelQueue++; //  Advance current queue
  this->pixelCycle++; //  Advance current cycle
  if (this->pixelQueue >= 3)
    this->pixelQueue = 0; //  Loop
  if (this->pixelCycle >= 256)
    this->pixelCycle = 0; //  Loop
}

void LED::colorWipe(int wait)
{
  static uint16_t current_pixel = this->start;
  this->pixelInterval = wait;                              //  Update delay time
  this->strip.setPixelColor(current_pixel++, this->color); //  Set pixel's color (in RAM)
  this->strip.show();                                      //  Update strip to match
  if (current_pixel >= this->end)
  { //  Loop the pattern from the first LED
    current_pixel = 0;
  }
}

void LED::wipe()
{ // clear all LEDs
  for (int i = this->start; i < this->end; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
}

uint32_t LED::Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
