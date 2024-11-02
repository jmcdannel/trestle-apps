#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ArduinoJson.h>

#if __has_include("config.h")
#include "config.h"
#else
#warning config.h not found. Using defaults from config.default.h
#include "config.default.h"
#endif

#if ENABLE_PWM
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#endif

const size_t capacity = 20 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
DynamicJsonDocument doc(capacity);

void setup()
{
  Serial.begin(115200);

  Serial.println(DEVICE_ID);

#if ENABLE_OUTPUTS
  Serial.println("OUTPINS");
  for (int idx = 0; idx < (sizeof(OUTPINS) / sizeof(OUTPINS[0])); idx++)
  {
    pinMode(OUTPINS[idx], OUTPUT);
    Serial.println(OUTPINS[idx]);
  }
#endif

#if ENABLE_SIGNALS
  Serial.println("SIGNALPINS");
  for (int idx = 0; idx < (sizeof(SIGNALPINS) / sizeof(SIGNALPINS[0])); idx++)
  {
    pinMode(SIGNALPINS[idx], OUTPUT);
    digitalWrite(SIGNALPINS[idx], HIGH);
    Serial.println(SIGNALPINS[idx]);
  }
#endif

#if ENABLE_TURNOUTS
  Serial.println("TURNOUTS");
  for (int idx = 0; idx < (sizeof(turnouts) / sizeof(turnouts[0])); idx++)
  {
    turnouts[idx].begin();
  }
#endif

#if ENABLE_PWM
  Serial.println("SERVOS");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  //  pwm.setPWMFreq(1200);  // This is the maximum PWM frequency
#endif
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
#if ENABLE_PWM
      pwm.setPWM(servo, 0, pulselen);
#endif
    }
  }
  else
  {
    for (uint16_t pulselen = pulseOrigin; pulselen < pulseTarget; pulselen++)
    {
#if ENABLE_PWM
      pwm.setPWM(servo, 0, pulselen);
#endif
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
