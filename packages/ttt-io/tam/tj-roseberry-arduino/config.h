#include <TurnoutPulser.h>

#define DEVICE_ID "tj-roseberry-arduino"
#define ENABLE_PWM false
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

int OUTPINS[] = {52};

int SIGNALPINS[] = {};

TurnoutPulser turnouts[] = {
    TurnoutPulser(2, 3),   // 0
    TurnoutPulser(4, 5),   // 1
    TurnoutPulser(6, 7),   // 2
    TurnoutPulser(8, 9),   // 3
    TurnoutPulser(10, 11), // 4
    TurnoutPulser(12, 13), // 5
    TurnoutPulser(22, 23), // 6
    TurnoutPulser(24, 25), // (NOT USED)
    TurnoutPulser(36, 37), // 8 ()
    TurnoutPulser(38, 39), // 9
    TurnoutPulser(40, 41), // 10
    TurnoutPulser(42, 43), // 11
    TurnoutPulser(44, 45), // 12
    TurnoutPulser(46, 47), // 13
    TurnoutPulser(48, 49), // 14
    TurnoutPulser(50, 51), // 15
};