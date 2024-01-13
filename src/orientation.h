#ifndef ORIENTATION_H
#define ORIENTATION_H

// #include "cpu_map.h"
#define SPINDLE_ORIENT

int TOOL_POS();

// int jumlahData;

class ATC {
    private:
    int pin;
    int bounce;
    bool status;
    unsigned long timestamp;
    public:
    ATC(int pinNumber,int mode);
    void setInputBounce(int value);
    void OFF();
    void ON();
    bool state();
};

void loop_orient();
void orientation_init();

extern ATC pinOut[];
// int jumlahData;
// unsigned long T[] = {0, 0, 0, 0, 0, 0, 0};

#endif