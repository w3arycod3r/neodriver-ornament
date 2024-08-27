/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.c 970 2010-05-25 20:16:15Z dmellis $

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 14-10-2009 for attiny45 Saposoft
  Modified 20-11-2010 - B.Cook - Rewritten to use the various Veneers.
*/

#include <Arduino.h>
#include <avr/boot.h>

/****************************** DEFINES ******************************/
// Timer0 is running at 8 MHz / 64 = 125 KHz
#define TIMER0_PRESCALER        (0b011)
#define TIMER0_PRESCALER_VALUE  (64)

// These prescaler bits correspond to "divide by 64"
//----> 8 MHz (Sys Clk) / 64 = 125 KHz (ADC Clk)
#define ADC_CLK_PRESCALER   (0b110)

// the prescaler is set so that the millis timer ticks every TIMER0_PRESCALER_VALUE (64) clock cycles, and the
// the overflow handler is called every 256 ticks. (Timer0 is an 8-bit timer/counter)
// The key is never to compute (F_CPU / 1000000L), which may lose precision.
// The formula below is correct for all F_CPU times that evenly divide by 10,
// at least for prescaler values up and including 64 as used in this file.
// Computes to 2048 usec
#define MICROSECONDS_PER_MILLIS_OVERFLOW \
    (TIMER0_PRESCALER_VALUE * 256UL * 1000UL * 100UL / ((F_CPU + 5UL) / 10UL))

// Computes to 8.
#define MICROSECONDS_PER_TIMER0_TICK (TIMER0_PRESCALER_VALUE / clockCyclesPerMicrosecond())


// The whole number of milliseconds per millis timer overflow.
// Computes to 2 msec.
#define MILLIS_INC (MICROSECONDS_PER_MILLIS_OVERFLOW / 1000U)

// the fractional number of milliseconds per millis timer overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
// Computes to 6.
#define FRACT_INC ((MICROSECONDS_PER_MILLIS_OVERFLOW % 1000U) >> 3)
// Computes to 125.
#define FRACT_MAX (1000U >> 3)


/****************************** GLOBALS ******************************/
static volatile uint32_t millis_timer_overflow_count = 0;
static volatile uint32_t millis_timer_millis = 0;
static volatile uint8_t millis_timer_fract = 0;


/****************************** FUNCTIONS ******************************/
// Timer0 overflow interrupt
ISR(TIMER0_OVF_vect)
{
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t m = millis_timer_millis;
    uint8_t f = millis_timer_fract;

    f += FRACT_INC;

    if (f >= FRACT_MAX)
    {
        f -= FRACT_MAX;
        m += MILLIS_INC + 1;
    }
    else
    {
        m += MILLIS_INC;
    }

    millis_timer_fract = f;
    millis_timer_millis = m;
    millis_timer_overflow_count++;
}

uint32_t millis()
{
    uint32_t m;
    uint8_t oldSREG = SREG;

    // disable interrupts while we read millis_timer_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to millis_timer_millis)
    cli();
    m = millis_timer_millis;
    // Restores interrupt flag
    SREG = oldSREG;

    return m;
}

uint32_t micros()
{

    uint32_t m;
    uint8_t t, oldSREG = SREG;

    cli();

    m = millis_timer_overflow_count;
    // Get current counter value.
    t = TCNT0;

    // Timer0 overflow has occurred, but we haven't handled the interrupt yet.
    if ((TIFR & _BV(TOV0)) && (t < 255))
    {
        m++;
    }

    SREG = oldSREG;

    // Can we just do it the naive way? If so great!
    return ((m << 8) + t) * (MICROSECONDS_PER_TIMER0_TICK);
}

static void __empty()
{
    // Empty
}
void yield(void) __attribute__ ((weak, alias("__empty")));

/* Delay for the given number of milliseconds. */
void delay_msec(uint32_t ms)
{
    uint16_t start = (uint16_t)micros();

    while (ms > 0)
    {
        yield();
        while (((uint16_t)micros() - start) >= 1000 && ms)
        {
            ms--;
            start += 1000;
        }
    }
}


/* Delay for the given number of microseconds.  Assumes a 1, **8**, 12, 16, 20 or 24 MHz clock. */
void delay_usec(uint16_t us)
{
    #define _MORENOP_ "" // redefine to include NOPs depending on frequency. No more were needed for 8MHz clock.

    // call = 4 cycles + 2 to 4 cycles to init us(2 for constant delay, 4 for variable)

    // calling avrlib's delay_us() function with low values (e.g. 1 or
    // 2 microseconds) gives delays longer than desired.
    //delay_us(us);


    // for the 8 MHz internal clock

    // for a 1 and 2 microsecond delay, simply return.  the overhead
    // of the function call takes 14 (16) cycles, which is 2us
    if (us <= 2) return; //  = 3 cycles, (4 when true)

    // the following loop takes 1/2 of a microsecond (4 cycles)
    // per iteration, so execute it twice for each microsecond of
    // delay requested.
    us <<= 1; //x2 us, = 2 cycles

    // account for the time taken in the preceding commands.
    // we just burned 17 (19) cycles above, remove 4, (4*4=16)
    // us is at least 6 so we can subtract 4
    us -= 4; // = 2 cycles


    // busy wait
    __asm__ __volatile__
    (
        "1: sbiw %0,1" "\n\t" // 2 cycles
            _MORENOP_         // more cycles according to definition
        "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
    );
    // return = 4 cycles
}


void init(void)
{

    // Timer0 Setup for millis() and micros():
    // Use the Millis Timer for fast PWM (unless it doesn't have an output).
    // if Timer0 has PWM
    TCCR0A = (1<<WGM01) | (1<<WGM00);
    TCCR0B = (TIMER0_PRESCALER << CS00);

    // this needs to be called before setup() or some functions won't work there
    sei();

    // Enable the Timer0 overflow interrupt (this is the basic system tic-toc for millis)
    sbi(TIMSK, TOIE0);


    // Initialize the ADC
    // set a2d prescale factor, enable a2d conversions
    ADCSRA = (ADC_CLK_PRESCALER << ADPS0) | (1<<ADEN);
}
