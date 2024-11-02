#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ArduinoJson.h>
#include <TurnoutPulser.h>
#include <SoftwareSerial.h>

#define SERVOMIN 150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // This is the 'maximum' pulse length count (out of 4096)
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_COUNT 16

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int outPins[] = {4, 5, 6, 14, 15, 16, 17, 20, 23, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53};
int signalPins[] = {
    7, 12, 13,
    18, 19, 33,
    21, 22, 34,
    24, 25, 26,
    27, 28, 29,
    30, 31, 32};
// int outPins [] = { 2, 3, 4, 5, 18, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53 };

/*f
[{"action":"pin","payload":{"pin":43,"value":0}}]
[{"action":"turnout","payload":{"turnout":0,"state":1}}]
 */

SoftwareSerial SoftSerial(2, 3);

TurnoutPulser turnouts[] = {
    TurnoutPulser(8, 9),
    TurnoutPulser(10, 11)};

const size_t capacity = 20 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
DynamicJsonDocument doc(capacity);

void setup()
{
  Serial.begin(115200);
  // int len = Serial.availableForWrite();
  // while (Serial.availableForWrite() == len) {
  //     // Serial.write('\n');
  // }

  SoftSerial.begin(9600);
  // int slen = SoftSerial.availableForWrite();
  // while (SoftSerial.availableForWrite() == slen) {
  //     // SoftSerial.write('\n');
  // }

  Serial.println("Setup");
  SoftSerial.println("<SoftSerial is ready>");

  for (int idx = 0; idx < (sizeof(outPins) / sizeof(outPins[0])); idx++)
  {
    pinMode(outPins[idx], OUTPUT);
  }

  for (int idx = 0; idx < (sizeof(signalPins) / sizeof(signalPins[0])); idx++)
  {
    pinMode(signalPins[idx], OUTPUT);
    digitalWrite(signalPins[idx], HIGH);
  }

  for (int idx = 0; idx < (sizeof(turnouts) / sizeof(turnouts[0])); idx++)
  {
    turnouts[idx].begin();
  }

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  //  pwm.setPWMFreq(1200);  // This is the maximum PWM frequency
}

void loop()
{

  for (int idx = 0; idx < (sizeof(turnouts) / sizeof(turnouts[0])); idx++)
  {
    turnouts[idx].loop();
  }
  if (Serial.available() > 0)
  {
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

void handleInput()
{
  String input = Serial.readString();
  Serial.println(input);
  deserializeJson(doc, input);
  JsonArray array = doc.as<JsonArray>();
  for (JsonVariant v : array)
  {
    handleAction(v);
  }
}

void handleAction(JsonVariant v)
{
  String action = v["action"];
  JsonObject payload = v["payload"];

  Serial.print("Action: ");
  Serial.println(action);

  if (action == "pin")
  {
    handlePin(payload);
  }
  else if (action == "servo")
  {
    handleServo(payload);
  }
  else if (action == "turnout")
  {
    handleTurnout(payload);
  }
  else if (action == "ialed")
  {
    handleLED(payload);
  }
}

void handleLED(JsonObject payload)
{
  int strip = payload["strip"];
  int pattern = payload["pattern"];
  int r = payload["r"];
  int g = payload["g"];
  int b = payload["b"];

  SoftSerial.print(strip);
  SoftSerial.print(", ");
  SoftSerial.print(pattern);
  SoftSerial.print(", ");
  SoftSerial.print(r);
  SoftSerial.print(", ");
  SoftSerial.print(g);
  SoftSerial.print(", ");
  SoftSerial.println(b);

  Serial.print(strip);
  Serial.print(", ");
  Serial.print(pattern);
  Serial.print(", ");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.println(b);
}

void handleTurnout(JsonObject payload)
{
  int turnoutIdx = payload["turnout"];
  bool state = payload["state"];
  turnouts[turnoutIdx].set(state);
  Serial.print("turnoutIdx: ");
  Serial.println(turnoutIdx);
  Serial.print("state: ");
  Serial.println(state);
}

void handlePin(JsonObject payload)
{
  int pin = payload["pin"];
  bool state = payload["state"];
  Serial.print("pin: ");
  Serial.println(pin);
  Serial.print("state: ");
  Serial.println(state);
  digitalWrite(pin, state);
}

void handleServo(JsonObject payload)
{
  int angle = payload["value"];
  int servo = payload["servo"];
  int current = payload["current"];
  int pulseTarget = getPulseWidth(angle);
  int pulseOrigin = getPulseWidth(current);
  if (pulseTarget < pulseOrigin)
  {
    for (uint16_t pulselen = pulseOrigin; pulselen > pulseTarget; pulselen--)
    {
      pwm.setPWM(servo, 0, pulselen);
    }
  }
  else
  {
    for (uint16_t pulselen = pulseOrigin; pulselen < pulseTarget; pulselen++)
    {
      pwm.setPWM(servo, 0, pulselen);
    }
  }
}

int getPulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);
  Serial.println(analog_value);
  return analog_value;
}
