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
#include <wiring_analog.h>

/****************************** DEFINES ******************************/

void adc_init()
{
    // Initialize the ADC
    // set a2d prescale factor, enable a2d conversions
    ADCSRA = (ADC_CLK_PRESCALER << ADPS0) | (1<<ADEN);

}

// Pass in the channel (ADMUX_MUX selection) and reference (ADMUX_REFS selection)
uint16_t adc_read(uint8_t ch, uint8_t analog_reference)
{
    // VREF can be selected as either VCC, or internal 1.1V / 2.56V voltage
    // reference, or external AREF pin. The first ADC conversion result after switching voltage reference source may
    // be inaccurate, and the user is advised to discard this result.

    // select the channel and reference
    ADMUX = ((analog_reference & ADMUX_REFS1_REFS0_MASK) << REFS0) | ((ch & ADMUX_MUX_MASK) << MUX0);
    // some have an extra reference bit in a weird position.
    ADMUX |= (((analog_reference & ADMUX_REFS2_MASK) >> 2) << REFS2);

    // After switching to internal voltage reference (as measurement channel)
    // the ADC requires a settling time of 1ms before measurements are stable.
    // Conversions starting before this may not be reliable. The ADC must be enabled during the settling time.
    if (ch == ADMUX_MUX_SE_VBG_INT_1V1)
    {
        // Wait for Vref to settle
        delay_msec(2);
    }

    // Start conversion
    sbi(ADCSRA, ADSC);

    // Wait for conversion to complete.
    while(bit_is_set(ADCSRA, ADSC)) {}

    // Assemble the 10-bit reading. Must read ADCL first.
    uint8_t low = ADCL;
    uint8_t high = ADCH;
    return ((high << 8) | low);
}
