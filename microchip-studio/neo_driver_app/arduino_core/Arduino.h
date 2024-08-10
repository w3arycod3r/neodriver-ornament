#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#ifdef __cplusplus
extern "C"{
#endif

#ifndef _NOPNOP
  #define _NOPNOP() do { __asm__ volatile ("rjmp .+0"); } while (0)
#endif

void yield(void);

#define HIGH 0x1
#define LOW  0x0

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105



// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

// We are using F_CPU = 8,000,000 (8 MHz)
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000UL )

#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


#define bit(b) (1UL << (b))

void init(void);

int analogRead(uint8_t);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);


void setup(void);
void loop(void);

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.

#define analogInPinToBit(P) (P)



#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2

#include "pins_arduino.h"


#define HAVE_ADC                  1


#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus


// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#endif

/*=============================================================================
  Aliases for the interrupt service routine vector numbers so the code
  doesn't have to be riddled with #ifdefs.
=============================================================================*/

#ifndef SIGRD
#define SIGRD 5
#endif



#endif /* Arduino_h */
