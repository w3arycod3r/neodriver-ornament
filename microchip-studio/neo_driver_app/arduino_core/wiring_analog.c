/*
  wiring_analog.c - analog input and output
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

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $

  Modified  28-08-2009 for attiny84 R.Wiersma
  Modified  14-10-2009 for attiny45 Saposoft
  Corrected 17-05-2010 for ATtiny84 B.Cook
*/

#include <Arduino.h>
#include <stdint.h>

/****************************** DEFINES ******************************/
// VCC used as Voltage Reference, disconnected from PB0 (AREF).
#define ADMUX_REFS_VCC_AS_REF   (0b000)
#define ADMUX_REFS_MASK         (0x03)

#define ADMUX_MUX_MASK          (0x0f)


// Pass in the channel (ADMUX selection)
// Maybe add reference as a param later.
// Could we get "random" read by using PB0 (AREF) as external ref to ADC?
// PB0 goes to left switch, with no hardware pullup.
uint16_t analogRead(uint8_t ch)
{
    uint8_t analog_reference = ADMUX_REFS_VCC_AS_REF;

    // select the channel and reference
    ADMUX = ((analog_reference & ADMUX_REFS_MASK) << REFS0) | ((ch & ADMUX_MUX_MASK) << MUX0);
    // some have an extra reference bit in a weird position.
    ADMUX |= (((analog_reference & 0x04) >> 2) << REFS2);

    // Start conversion
    sbi(ADCSRA, ADSC);

    // Wait for conversion to complete.
    while(ADCSRA & (1<<ADSC))
    {
        // Do nothing
    }

    // Assemble the 10-bit reading
    uint8_t low = ADCL;
    uint8_t high = ADCH;
    return ((high << 8) | low);
}
