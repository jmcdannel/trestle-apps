#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ArduinoJson.h>
#include <TurnoutPulser.h>
#include "LED.h"


#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_COUNT 16

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

LED testStrip(4, 256);

int outPins [] = { 2, 3, 4, 5, 10, 11, 12 };

TurnoutPulser turnouts[] = {
  TurnoutPulser(8, 9),
  TurnoutPulser(6, 7) 
};

/*
[{"action":"pin","payload":{"pin":43,"value":0}}]
[{"action":"turnout","payload":{"turnout":0,"state":1}}]
 */

const size_t capacity = 20*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
DynamicJsonDocument doc(capacity);

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("Setup");

  for (int idx=0; idx<(sizeof(outPins) / sizeof(outPins[0])); idx++) {
    pinMode(outPins[idx], OUTPUT);
  }

  for (int idx=0; idx<(sizeof(turnouts) / sizeof(turnouts[0])); idx++) {
    turnouts[idx].begin();
  }

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
//  pwm.setPWMFreq(1200);  // This is the maximum PWM frequency

  testStrip.begin();
}

void loop() {
  
  for (int idx=0; idx<(sizeof(turnouts) / sizeof(turnouts[0])); idx++) {
    turnouts[idx].loop();
  }
  if (Serial.available() > 0) {
    Serial.println("handleInput");
    handleInput();
  }
  
//  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
//    pwm.setPWM(2, 0, pulselen);
//    Serial.println(pulselen);
//  }
//  pwm.setPWM(2, 0, 150);
//  delay(500);
//  pwm.setPWM(2, 0, 600);
//  delay(500);
}

void handleInput() {
  String input = Serial.readString();
  Serial.println(input);
  deserializeJson(doc, input);
  JsonArray array = doc.as<JsonArray>();
  for(JsonVariant v : array) {
    handleAction(v);
  }  
}

void handleAction(JsonVariant v) {
  String action = v["action"];
  JsonObject payload = v["payload"];

  Serial.print("Action: ");
  Serial.println(action);

  if (action == "pin") {
    handlePin(payload);
  } else  if (action == "servo") {
    handleServo(payload);
  } else if (action == "turnout") {
    handleTurnout(payload);
  } else if (action == "ialed") {
    handleIALED(payload);
  }
  
}

void handleIALED(JsonObject payload) {
  int r = payload["r"];
  int g = payload["g"];
  int b = payload["b"];
  testStrip.setColor(r, g, b);
}

void handleTurnout(JsonObject payload) {
  int turnoutIdx = payload["turnout"];
  bool state = payload["state"];
  turnouts[turnoutIdx].set(state);
  Serial.print("turnoutIdx: ");
  Serial.println(turnoutIdx);
  Serial.print("state: ");
  Serial.println(state);
}

void handlePin(JsonObject payload) {
  int pin = payload["pin"];
  bool state = payload["state"];
  Serial.print("pin: ");
  Serial.println(pin);
  Serial.print("state: ");
  Serial.println(state);
  digitalWrite(pin, state);
}

void handleServo(JsonObject payload) {
  int angle = payload["value"];
  int servo = payload["servo"];
  int current = payload["current"];
  int pulseTarget = getPulseWidth(angle);
  int pulseOrigin = getPulseWidth(current);
  if (pulseTarget < pulseOrigin) {
    for (uint16_t pulselen = pulseOrigin; pulselen > pulseTarget; pulselen--) {
      pwm.setPWM(servo, 0, pulselen);
    }
  } else {
    for (uint16_t pulselen = pulseOrigin; pulselen < pulseTarget; pulselen++) {
      pwm.setPWM(servo, 0, pulselen);
    }
  }
}

int getPulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);
  Serial.println(analog_value);
  return analog_value;
}
