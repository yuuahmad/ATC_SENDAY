#include <Arduino.h>
#include <orientation.h>
#include <parsing.h>

void setup()
{
  // INISIALISASI AWAL ATC (POSISI LOCK, DAN OFF CW & CCW NYA)
  pinOut[0].ON();
  pinOut[1].OFF();
  pinOut[2].OFF();
  pinOut[3].OFF();
}

void loop()
{
  parsing_function();
  loop_orient();
  delay(50);
}
