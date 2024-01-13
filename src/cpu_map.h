#ifndef CPU_MAP_H
#define CPU_MAP_H
// Pin Configuration
//  #define Alarm_pin       PA3
//  #define Alarm_Clear     PA4
// Lubricate
// Oil Pump
#define Oil_Pump PA5
#define On_Oil_Timer PA6
#define Oil_level_sensor PA7
#define Buzzer PB0

// Coolant
#define Flood_level PA3

//================================================================
// AUX IO PIN Configuration (ATC)
// pin untuk sensor proximity
#define INPUT0 PB9  // proxy 1
#define INPUT1 PB8  // proxy 2
#define INPUT2 PA8  // proxy 3
#define INPUT3 PB10 // proxy 4
#define INPUT4 PB5  // proxy 5
#define INPUT5 PB4  // proxy 6
#define INPUT6 PB3  // yang ini tidak dipakai
// ini pin untuk lain2
// #define INPUT7 PA0
// #define INPUT8 PA1
// #define INPUT9 PA2
// #define INPUT10 PB14
// #define INPUT11 PB15
// #define INPUT12 PA4
// pinout untuk relayy hidrolik
#define OUTPUT0 PB14 // PIN UNTUK LOCK
#define OUTPUT1 PA15 // PIN UNTUK UNLOCK
#define OUTPUT2 PC13 // PIN CW
#define OUTPUT3 PC14 // PIN CCW
// #define OUTPUT4             PC15
// #define OUTPUT5             PB1
// #define OUTPUT6             PA9
// #define OUTPUT8             PA10

//================================================================
#endif