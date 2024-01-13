#include <Arduino.h>
#include <orientation.h>
#include <cpu_map.h>

#ifdef SPINDLE_ORIENT
#pragma message "ORIENTATION ACTIVE"
int data[] = {0, 0, 0, 0, 0, 0, 0, 0};

ATC::ATC(int pinNumber, int mode) // nomor pin, mode (1 output, 0 input_pullup)
{
    pin = pinNumber;
    if (mode == 1)
    {
        pinMode(pin, OUTPUT);
        OFF();
    }
    else
    {
        pinMode(pin, INPUT_PULLUP);
    }
}

void ATC::setInputBounce(int value)
{
    value = bounce;
}
void ATC::OFF()
{
    digitalWriteFast(digitalPinToPinName(pin), 0);
}
void ATC::ON()
{
    digitalWriteFast(digitalPinToPinName(pin), 1);
}

bool ATC::state()
{
    if (millis() - timestamp > bounce)
        timestamp = 0;
    if (!digitalReadFast(digitalPinToPinName(pin)))
    {
        if (millis() - timestamp > 50)
        {
            status = true;
        }
    }
    else
    {
        timestamp = millis();
        status = false;
    }
    return status;
}

// Deklarasi array objek ATC INPUT
// Tambahi kalo kurang
ATC Triggr[] = {
    // sensor untuk proximity
    ATC(INPUT0, 0), // b9
    ATC(INPUT1, 0), // b8
    ATC(INPUT2, 0), // a8
    ATC(INPUT3, 0), // b10
    ATC(INPUT4, 0), // b5
    ATC(INPUT5, 0), // b4
    ATC(INPUT6, 0), // b3

    // ATC(INPUT7, 0), // a0
    // ATC(INPUT8, 0), // a1
    // ATC(INPUT9, 0), // a2
    // ATC(INPUT10, 0),
    // ATC(INPUT11, 0),
    // ATC(INPUT12, 0)
};

// Deklarasi array objek ATC OUTPUT
// Tambahi kalo kurang
ATC pinOut[] = {
    ATC(OUTPUT0, 1),
    ATC(OUTPUT1, 1),
    ATC(OUTPUT2, 1),
    ATC(OUTPUT3, 1),
    // ATC(OUTPUT4, 1),
    // ATC(OUTPUT5, 1), // Dipake buzzer
    // ATC(OUTPUT6, 1)
};

// Ganti nama biar gampang di inget
ATC &CW = pinOut[1];
ATC &CCW = pinOut[2];
ATC &LOCK = pinOut[3];
ATC &UNLOCK = pinOut[4];

// ATC &PROXY1         = Triggr[0];
// ATC &PROXY2         = Triggr[1];
// ATC &PROXY3         = Triggr[2];
// ATC &PROXY4         = Triggr[3];
// ATC &PROXY5         = Triggr[4];
// ATC &PROXY6         = Triggr[5];

/*
    milih value berdasarkan proxy yang aktif

    NO  | PROXY3 | PROXY4 | PROXY5 | PROXY6 |
    1   |
    2   |
    dst ...
*/
// int TOOL_POS()
// {
//     if (!data[2] && data[3] && data[4] && data[5])
//         return 1; // T1
//     if (data[2] && !data[3] && data[4] && data[5])
//         return 2; // T2
//     if (data[2] && data[3] && !data[4] && data[5])
//         return 3; // T3
//     if (data[2] && data[3] && data[4] && !data[5])
//         return 4; // T4
//     if (data[2] && !data[3] && !data[4] && !data[5])
//         return 5; // T5
//     if (!data[2] && data[3] && !data[4] && !data[5])
//         return 6; // T6
//     if (!data[2] && !data[3] && data[4] && !data[5])
//         return 7; // T7
//     if (!data[2] && !data[3] && !data[4] && data[5])
//         return 8; // T8
//     return 0;
// }

// void orientation_init(){
//     jumlahData = sizeof(pinOut) / sizeof(pinOut[0]);
// }

// unsigned long waktu;

void loop_orient()
{
    // BOOOWWRRIINGG LOOP FUNCTION

    // baca status pin input, kemudian masukin hasil pembacaan ke array data
    Serial.print("P");
    for (int i = 0; i < 6; i++)
    {
        data[i] = Triggr[i].state();
        Serial.print(data[i]);
        // Serial.print("|");
        // if(Triggr[i].state()){
        //     if(millis() - T[i] > 150){
        //         data[i] = 1;
        //     }
    }
    // Serial.printf("T%d", TOOL_POS());
    Serial.println("");
    // else{
    //     T[i] = millis();
    //     data[i] = 0;
    // }
    // }
    // ORIENTATION_OK = data[0];
    // TOOL_COUNT = data[5];
    // UMBRELLA_POSITION = data[1];
    // SPINDLE_PNEUMATIC_POSITION = data[3];
}
#endif