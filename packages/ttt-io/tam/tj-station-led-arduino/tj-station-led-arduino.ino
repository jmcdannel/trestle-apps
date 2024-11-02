// StrandTest from AdaFruit implemented as a state machine
// pattern change by push button
// By Mike Cook Jan 2016

#define PINforControl   4 // pin connected to the small NeoPixels strip
#define NUMPIXELS1      256 // number of LEDs on strip

#include <SoftwareSerial.h>
#include <textparser.h>
#include "LED.h"

LED strips[] = {
   LED(4, 47),
   LED(5, 256)
};

int numStrips = (sizeof(strips) / sizeof(strips[0]));

boolean newData = false;

SoftwareSerial SoftSerial(3, 2);
TextParser parser(", ");
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

void setup() {
  Serial.begin(9600);

  SoftSerial.begin(9600);

  Serial.println("<Arduino is listening>");
  SoftSerial.println("<Arduino is listening>");

  for (int idx=0; idx<numStrips; idx++) {
    strips[idx].begin();
  }
}

void loop() {
  recvWithEndMarker(); // check for incoming data
  showNewData();

  for (int idx=0; idx<numStrips; idx++) {
    strips[idx].loop();
  }  
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (SoftSerial.available() > 0 && newData == false) {
        rc = SoftSerial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
  int ledParams[5];
  int stripIdx;
  static int pattern = 0;
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        parser.parseLine(receivedChars, ledParams);
        stripIdx = ledParams[0];
        Serial.println(ledParams[0]);
        Serial.println(ledParams[1]);
        strips[stripIdx].setPattern(ledParams);
        newData = false;
    }
}
