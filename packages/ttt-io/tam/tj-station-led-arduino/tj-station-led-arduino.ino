
/*
Examples:
0, 2 # set strip 0 to rainbow

ledParams
  0: stripIdx (int)
  1: pattern
    off
    color
    rainbow
    chasecolor
    wipe

  2: range
    "all" | "[start:end]"

  4: config
    r:g:b

0, color, all, 255:30:30
1, rainbow, 5:10, 6:230:30
0, rainbow, all, 240:20:20

*/

#include <textparser.h>
#include "LED.h"

LED strips[] = {
    LED(6, 47),
    LED(5, 256)};

int numStrips = (sizeof(strips) / sizeof(strips[0]));

boolean newData = false;

TextParser parser(", ");
const byte numChars = 72;
char receivedChars[numChars]; // an array to store the received data

void setup()
{
  Serial.begin(115200);

  Serial.println("<Arduino is listening>");

  for (int idx = 0; idx < numStrips; idx++)
  {
    strips[idx].begin();
  }
}

void loop()
{
  recvWithEndMarker(); // check for incoming data
  showNewData();

  for (int idx = 0; idx < numStrips; idx++)
  {
    strips[idx].loop();
  }
}

void recvWithEndMarker()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false)
  {
    rc = Serial.read();

    if (rc != endMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    else
    {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData()
{
  // param format: stripIdx: number, mode: string, range: string, config:string

  static int pattern = 0;
  if (newData == true)
  {
    Serial.print("This just in ... ");
    Serial.println(receivedChars);
    int stripIdx;
    char pattern[20];
    char range[20];
    char config[20];
    // String ledParams[4];
    parser.parseLine(receivedChars, stripIdx, pattern, range, config);
    Serial.println(stripIdx);
    Serial.println(pattern);
    Serial.println(range);
    Serial.println(config);
    strips[stripIdx].setPattern(pattern, range, config);
    newData = false;
  }
}
