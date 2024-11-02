#include <TurnoutPulser.h>

#define DEVICE_ID "deja-arduino"
#define ENABLE_PWM false
#define ENABLE_OUTPUTS false
#define ENABLE_SIGNALS false
#define ENABLE_TURNOUTS false

#define SERVOMIN 150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // This is the 'maximum' pulse length count (out of 4096)
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_COUNT 16

int OUTPINS[] = {};
int SIGNALPINS[] = {};

TurnoutPulser turnouts[] = {};