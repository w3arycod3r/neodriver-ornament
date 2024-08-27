/* File:      wiring_analog.h
 * Author:    Garrett Carter
 * Purpose:   Analog I/O functions
 */

#ifndef WIRING_ANALOG_H
#define WIRING_ANALOG_H

#include <Arduino.h>
#include <stdbool.h>
#include <ard_utility.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************** DEFINES ******************************/
#define ADC_NUM_BITS            (10)
#define ADC_MAX_VALUE           ((1 << ADC_NUM_BITS) - 1)
#define MILLIVOLTS_PER_VOLT     (1000)
#define ADC_INT_1V1_REF_VOLTAGE (1.1)

// Below defines are the possible values for the ADMUX_REFS bits in the ADMUX register.
#define ADMUX_REFS1_REFS0_MASK           (0b011)
#define ADMUX_REFS2_MASK                 (0b100)
// VCC used as Voltage Reference, disconnected from PB0 (AREF).
#define ADMUX_REFS_VCC_AS_REF            (0b000)
// External Voltage Reference at PB0 (AREF) pin, Internal Voltage Reference turned off.
#define ADMUX_REFS_EXT_PB0_AREF          (0b001)
// Internal 1.1V Voltage Reference.
#define ADMUX_REFS_INT_1V1_REF           (0b010)
// Internal 2.56V Voltage Reference without external bypass capacitor, disconnected from PB0 (AREF).
// The device requires a supply voltage of 3V in order to generate 2.56V reference voltage.
#define ADMUX_REFS_INT_2V56_NO_BYP_CAP   (0b110)
// Internal 2.56V Voltage Reference with external bypass capacitor at PB0 (AREF) pin.
// The device requires a supply voltage of 3V in order to generate 2.56V reference voltage.
#define ADMUX_REFS_INT_2V56_WITH_BYP_CAP (0b111)

//                  ATMEL ATTINY85
//
//                      +-\/-+
//  ADC0  !RESET  PB5  1|    |8  VCC
//  ADC3  PCINT3  PB3  2|    |7  PB2  PCINT2  ADC1
//  ADC2          PB4  3|    |6  PB1  PCINT1     
//                GND  4|    |5  PB0  PCINT0  AREF
//                      +----+
// Below defines are the possible values for the MUX bits in the ADMUX register.
// "SE" stands for Single Ended.
#define ADMUX_MUX_MASK          (0x0f)

#define ADMUX_MUX_SE_ADC0_PB5     (0b0000)
#define ADMUX_MUX_SE_ADC1_PB2     (0b0001)
#define ADMUX_MUX_SE_ADC2_PB4     (0b0010)
#define ADMUX_MUX_SE_ADC3_PB3     (0b0011)
// 0b0100 - 0b1011 are used for differential channels.
#define ADMUX_MUX_SE_VBG_INT_1V1  (0b1100)  // Internal bandgap (1.1V) voltage reference
#define ADMUX_MUX_SE_GND          (0b1101)
#define ADMUX_MUX_SE_ADC4         (0b1111)  // Temperature sensor, must be used with 1.1V internal reference.

// This macro computes the ADMUX register value based on the channel and reference selection bits, at compile time.
// This assumes ADLAR is always 0.
#define COMPOSE_ADMUX(ch, ref) (((ref & ADMUX_REFS1_REFS0_MASK) << REFS0) | (((ref & ADMUX_REFS2_MASK) >> 2) << REFS2) | ((ch & ADMUX_MUX_MASK) << MUX0))

/****************************** PROTOTYPES ******************************/
uint16_t adc_read(uint8_t ch, uint8_t analog_reference);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* WIRING_ANALOG_H */
