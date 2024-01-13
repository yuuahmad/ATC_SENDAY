#include "Parsing.h"
// #include "Report.h"
// #include "Config.h"
// #include "cpu_map.h"
// #include "Oil.h"
// #include "Alarm.h"
#include "orientation.h"

// parsing
// String line;
static char line[LINE_BUFFER_SIZE];
String dt[8];
int i;
bool parsing = false;
uint8_t action = 0;
static size_t buffer_pos = 0;
GC_Values_t gc_state;

void parsing_function()
{
  if (Serial.available() > 0)
  {
    /*
    delay(5);
    char inChar = (char)Serial.read();
    inChar = toUpperCase(inChar);
    line[] = inChar;
    if (inChar == '\n') {
      parsing = true;
    }
    */
    char c = Serial.read();
    // delay(10);
    c = toUpperCase(c);
    if (c == '\n')
    {                          // end of message
      line[buffer_pos] = '\0'; // terminate the string
      // process(buffer);
      parsing = true;
      if (parsing)
      {
        Parsing_data();
        // Report_Respond(Parsing_data());
        action = NO_ACTION_BIT;
        parsing = false;
        buffer_pos = 0; // reset for next message
        // line[] = "";
      }
    }
    else if (buffer_pos < sizeof line - 1)
    {
      line[buffer_pos++] = c; // buffer the character
    }
  }

  // delay(5);
}

uint8_t Parsing_data()
{
  uint8_t char_counter = 0;
  char letter = 0;
  float value = 0.0;
  uint8_t int_value = 0;
  // Serial.println(sizeof line);
  while (line[char_counter] != 0) // Loop until no more g-code words in line.
  {

    // Import the next g-code word, expecting a letter followed by a value. Otherwise, error out.
    letter = line[char_counter];

    char_counter++;
    if (!Read_Float(line, &char_counter, &value))
    {
      // return STATUS_ERROR_CODE;
    } // [Expected word value]
    int_value = trunc(value);
    // Serial.println(letter);
    // Serial.println(value);

    switch (letter)
    {
    case 'L': // LOCK ATC
      pinOut[0].ON();
      pinOut[1].OFF();
      // Serial.println(pinOut[0].state());
      delay(500);
      break;
    case 'U':          // UNLOCK ATC
      pinOut[0].OFF(); // PIN LOCK
      pinOut[1].ON();  // PIN UNLOCK
      // Serial.println("HAI");
      // Serial.println(pinOut[0].state());
      delay(500);
      break;
    case 'A':          // BERPUTAR CW
      pinOut[2].ON();  // PIN CW
      pinOut[3].OFF(); // PIN CCW
      delay(500);
      break;
    case 'B': // BERPUTAR CCW
      pinOut[2].OFF();
      pinOut[3].ON();
      delay(500);
      break;
    case 'C': // BERHENTI BERPUTAR
      pinOut[2].OFF();
      pinOut[3].OFF();
      delay(500);
      break;
    case 'N':
      break;
#ifdef SPINDLE_ORIENT
    case 'P': // ORIENTATION START
      break;
    case 'Q':
      break;
    case 'R':
      break;
    case 'S':
      break;
    case 'T':
      break;
#endif

    default:
      // Serial.println(letter);
      if (line[char_counter] == 0)
      {
        //;//Serial.println("same");
      }
      // else return STATUS_ERROR_CODE; // [Unsupported G command]
      // return 0;
      /*
      switch(letter)
      {
        //case 'I': action += OIL_CHANGE_IN_BIT;  gc_state.i = int_value; break;
        //case 'N': action += OIL_CHANGE_SEC_BIT; gc_state.n = int_value; break;

        default:
        return 0;//STATUS_ERROR_CODE;
      }
      */
    }
  } // complete parsing
    // Serial.println(gc_state.i);
  // Serial.println(gc_state.i);
  if (action != 0)
  {
    // Serial.println(gc_state.i);
    if (action == OIL_CHANGE_IN_BIT)
    {
      // Serial.println("int");
      //  if(Oil_Int_Sync(gc_state.i)){
      //    return STATUS_ERROR_CODE;
      //  }
    }
    else if (action == OIL_CHANGE_SEC_BIT)
    {
      // if(Oil_On_Sync(gc_state.n)){
      //   return STATUS_ERROR_CODE;
      // }
    }
    else if (action == OIL_ON_BIT)
    {
      // Oil_On();
    }
#ifdef Alarm
    else if (action == ALARM_CLEAR_BIT)
    {
      Alarm_clear(gc_state.c);
    }
#endif
  }

  return 1;
}

// Extracts a floating point value from a string. The following code is based loosely on
// the avr-libc strtod() function by Michael Stumpf and Dmitry Xmelkov and many freely
// available conversion method examples, but has been highly optimized for Grbl. For known
// CNC applications, the typical decimal value is expected to be in the range of E0 to E-4.
// Scientific notation is officially not supported by g-code, and the 'E' character may
// be a g-code word on some CNC systems. So, 'E' notation will not be recognized.
// NOTE: Thanks to Radu-Eosif Mihailescu for identifying the issues with using strtod().
uint8_t Read_Float(char *line, uint8_t *char_counter, float *float_ptr)
{
  char *ptr = line + *char_counter;
  unsigned char c;

  // Grab first character and increment pointer. No spaces assumed in line.
  c = *ptr++;

  // Capture initial positive/minus character
  bool isnegative = false;
  if (c == '-')
  {
    isnegative = true;
    c = *ptr++;
  }
  else if (c == '+')
  {
    c = *ptr++;
  }

  // Extract number into fast integer. Track decimal in terms of exponent value.
  uint32_t intval = 0;
  int8_t exp = 0;
  uint8_t ndigit = 0;
  bool isdecimal = false;

  while (1)
  {
    c -= '0';

    if (c <= 9)
    {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS)
      {
        if (isdecimal)
        {
          exp--;
        }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
      }
      else
      {
        if (!(isdecimal))
        {
          exp++;
        } // Drop overflow digits
      }
    }
    else if (c == (('.' - '0') & 0xff) && !(isdecimal))
    {
      isdecimal = true;
    }
    else
    {
      break;
    }
    c = *ptr++;
  }
  // Serial.println(ndigit);
  //  Return if no digits have been read.
  if (!ndigit)
  {
    return (false);
  };

  // Convert integer into floating point.
  float fval;
  fval = (float)intval;

  // Apply decimal. Should perform no more than two floating point multiplications for the
  // expected range of E0 to E-4.
  if (fval != 0)
  {
    while (exp <= -2)
    {
      fval *= 0.01;
      exp += 2;
    }
    if (exp < 0)
    {
      fval *= 0.1;
    }
    else if (exp > 0)
    {
      do
      {
        fval *= 10.0;
      } while (--exp > 0);
    }
  }

  // Assign floating point value with correct sign.
  if (isnegative)
  {
    *float_ptr = -fval;
  }
  else
  {
    *float_ptr = fval;
  }

  *char_counter = ptr - line - 1; // Set char_counter to next statement

  return (true);
}
