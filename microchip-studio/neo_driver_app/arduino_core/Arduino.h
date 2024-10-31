#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdbool.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#ifdef __cplusplus
extern "C"{
#endif

/****************************** DEFINES ******************************/
#ifndef _NOPNOP
  #define _NOPNOP() do { __asm__ volatile ("rjmp .+0"); } while (0)
#endif


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
// Computes to 8.
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000UL )
// Computes to 125.
#define nanosecondsPerClockCycle() ( 1000000000UL / F_CPU )

#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


#define bit(b) (1UL << (b))

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

typedef void (*voidFuncPtr)(void);


/****************************** PROTOTYPES ******************************/
void init(void);

uint32_t millis(void);
uint32_t micros(void);
void delay_msec(uint32_t ms);
void delay_usec(uint16_t us);

void setup(void);
void loop(void);

void yield(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* ARDUINO_H */
