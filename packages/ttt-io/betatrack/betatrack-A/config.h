#include <TurnoutPulser.h>

#define DEVICE_ID "betatrack-A"
#define ENABLE_PWM true
#define ENABLE_OUTPUTS true
#define ENABLE_SIGNALS true
#define ENABLE_TURNOUTS true

#define SERVOMIN 150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // This is the 'maximum' pulse length count (out of 4096)
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_COUNT 16

int OUTPINS[] = {2, 3, 20, 21, 22, 23, 42, 43, 44, 45, 46, 47, 48};

int SIGNALPINS[] = {};

TurnoutPulser turnouts[] = {
    TurnoutPulser(24, 25),
    TurnoutPulser(26, 27)};