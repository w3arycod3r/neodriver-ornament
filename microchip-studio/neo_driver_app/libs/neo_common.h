/* File:      neo_common.h
 * Author:    Garrett Carter
 * Purpose:   Common def's for eep_data_write and neo_driver_app, so they don't get out of sync
 */

#ifndef NEO_COMMON_H
#define NEO_COMMON_H

// This lib depends on another
#include <Arduino.h>
#include <ard_utility.h>
#include <avr/pgmspace.h>
#include <wiring_analog.h>

/********************************** DEFINES **********************************/

/*
 *  FUSE SETTINGS:
 *  EESAVE    ENABLE
 *  BODLEVEL  ENABLE at VCC = 1.8V
 *  CKDIV8    DISABLE
 *  SUT_CKSEL INTRCOSC_8MHZ_6CK_14CK_64MS
 *
 *  LOW       0xE2
 *  HIGH      0xD6
 *  EXTENDED  0xFF
 * 
 *  fuses.txt = e2d6ff
 */

#define MATRIX_WIDTH_PIX    5
#define MATRIX_HEIGHT_PIX   5
#define MATRIX_NUM_PIX (MATRIX_WIDTH_PIX * MATRIX_HEIGHT_PIX)

// I/O Mapping
#define IO_SW_LEFT       (BIT0)                       // PB0, AREF - Left switch
#define IO_SW_RIGHT      (BIT1)                       // PB1       - Right switch
#define IO_NP_ENABLE     (BIT2)                       // PB2, ADC1 - NeoPixel Enable
#define IO_NP_DATA       (BIT3)                       // PB3, ADC3 - NeoPixel Data
#define IO_POT_ADC_CH    (ADMUX_MUX_SE_ADC2_PB4)      // PB4, ADC2 - Brightness Potentiometer
#define IO_POT_ADC_REF   (ADMUX_REFS_VCC_AS_REF)      // PB4, ADC2 - Brightness Potentiometer
                                                      // PB5, ADC0 - Reset

// Analog channel and reference for VCC measurement
#define VCC_AN_MEAS_CH   (ADMUX_MUX_SE_VBG_INT_1V1)
#define VCC_AN_MEAS_REF  (ADMUX_REFS_VCC_AS_REF)

// 

/*  Analog channel and reference for "random noise" measurement
    Could we get "random" read by using PB0 (AREF) as external ref to ADC?
    PB0 goes to left switch, with no external pullup.
    Could measure any constant channel, with the "random" reference.
    Probably need to do consecutive reads, and composite the least significant bits.
*/
#define RAND_AN_MEAS_CH  (IO_POT_ADC_CH)
#define RAND_AN_MEAS_REF (ADMUX_REFS_EXT_PB0_AREF)

// Pixel params
#define BRIGHT_MIN   24
#define BRIGHT_INIT  8
#define CONFIRM_BLINK_BRIGHT_LVL 50     // Brightness used for EEP program confirm blinking
#define PIX_CNT_MIN  1
#define PIX_CNT_MAX  25
#define PIX_CNT_INIT 25                 // Cannot be greater than PIX_CNT_MAX

// Colors
#define COLOR_TEAL   0x00FFFFL
#define COLOR_YELLOW 0xFFFF00L
#define COLOR_WHITE  0xFFFFFFL
#define COLOR_PURPLE 0xFF00FFL
#define COLOR_RED    0xFF0000L
#define COLOR_GREEN  0x00FF00L
#define COLOR_BLUE   0x0000FFL

#define COLOR_WHEEL  0x000000L // Dynamic color wheel effect

// EEPROM Addresses

// First 16 bytes reserved for settings storage
#define EEP_SETT_FIRST_ADDR     0
#define EEP_SETT_ANIM           0
#define EEP_SETT_NPIX           1   // No longer needed, since we are using a fixed 5x5 matrix
#define EEP_SETT_RSEED          2   // Addr 2-5, 4 bytes
#define EEP_SETT_FLAGS          6   // 8 Status Flags
// Runtime since last charge in minutes. Includes "on" and "off" time. Stops counting once battery is "dead".
#define EEP_SETT_RUNTIME_MIN    7   // 2 bytes
// Number of animation cycles since stat reset.
#define EEP_SETT_NUM_CYCLES     9   // 2 bytes
// Number of power-on's since stat reset.
#define EEP_SETT_NUM_POWER_ON   11   // 2 bytes
#define EEP_SETT_LAST_ADDR      15

// Status Flags (bits in EEP_SETT_FLAGS)
#define STAT_FLG_BATT_DEAD  BIT0


// EEPROM Ranges & Sizes
#define EEP_SIZE_BYTES 512


// Comment out below line to skip writing the cov virus data, if you don't intend to use the animation
// #define EEP_COV_DATA_WRITE_ENABLE
#define EEP_COV_DATA_START_ADDR   432
// 160 Bytes of sequence are available
#define EEP_COV_DATA_NUM_BYTES   80

#define EEP_CHAR_DATA_START_ADDR 16

// These below progmem arrays are OK to define in .h file, as they're only used by the eep_data_write program.
// Double allocation in flash wouldn't be an issue there.
/*
    5x5 font inspired by: http://batchout.blogspot.com/2018/02/five-by-five-my-5x5-pixel-font.html
    
    Character Data
    EEPROM Addr 16-511
    (Extended) ASCII 32-189

    25 bits per glyph
    With compression, we have space for up to 158 glyphs, if cov animation not used.
*/
static const uint8_t PROGMEM charSet[] = {
    // [ASCII: CHAR]

    // 32: Space
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,

    // 33: !
    0b01000,
    0b01000,
    0b01000,
    0b00000,
    0b01000,

    // 34: "  ghost1
    // 0b00001010,
    // 0b00001010,
    // 0b00000000,
    // 0b00000000,
    // 0b00000000,
    0b01110,
    0b11111,
    0b10101,
    0b11111,
    0b10101,

    // 35: #  batt1
    // 0b01010,
    // 0b11111,
    // 0b01010,
    // 0b11111,
    // 0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b10000,

    // 36: $  batt2
    // 0b00001111,
    // 0b00010100,
    // 0b00011111,
    // 0b00000101,
    // 0b00011110,
    0b00000,
    0b00000,
    0b00000,
    0b01000,
    0b11000,

    // 37: %  batt3
    // 0b00011001,
    // 0b00010010,
    // 0b00000100,
    // 0b00001001,
    // 0b00010011,
    0b00000,
    0b00000,
    0b00100,
    0b01100,
    0b11100,

    // 38: &  batt4
    // 0b00001100,
    // 0b00010010,
    // 0b00001100,
    // 0b00010011,
    // 0b00001101,
    0b00000,
    0b00010,
    0b00110,
    0b01110,
    0b11110,

    // 39: '  batt5
    // 0b00010000,
    // 0b00010000,
    // 0b00000000,
    // 0b00000000,
    // 0b00000000,
    0b00001,
    0b00011,
    0b00111,
    0b01111,
    0b11111,

    // 40: (  batt6
    // 0b00000110,
    // 0b00001100,
    // 0b00001000,
    // 0b00001100,
    // 0b00000110,
    0b00100,
    0b01110,
    0b01010,
    0b01010,
    0b01110,

    // 41: )  snow1
    // 0b00001100,
    // 0b00000110,
    // 0b00000010,
    // 0b00000110,
    // 0b00001100,
    0b10001,
    0b00000,
    0b00000,
    0b00010,
    0b00000,

    // 42: *  snow2
    // 0b00010101,
    // 0b00001110,
    // 0b00011111,
    // 0b00001110,
    // 0b00010101,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b00100,

    // 43: +  snow3
    // 0b00000000,
    // 0b00000100,
    // 0b00001110,
    // 0b00000100,
    // 0b00000000,
    0b00100,
    0b00000,
    0b10001,
    0b00000,
    0b00000,

    // 44: ,  snow4
    // 0b00000000,
    // 0b00000000,
    // 0b00000000,
    // 0b00000100,
    // 0b00001000,
    0b00000,
    0b01000,
    0b00000,
    0b01010,
    0b00000,

    // 45: -
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000,

    // 46: .  snow5
    // 0b00000000,
    // 0b00000000,
    // 0b00000000,
    // 0b00000000,
    // 0b00001000,
    0b00000,
    0b00000,
    0b00100,
    0b00000,
    0b10001,

    // 47: /
    0b00001,
    0b00011,
    0b00110,
    0b01100,
    0b01000,

    // 48: 0
    0b01110,
    0b10001,
    0b10101,
    0b10001,
    0b01110,

    // 49: 1
    0b01100,
    0b10100,
    0b00100,
    0b00100,
    0b11111,

    // 50: 2
    0b01110,
    0b10001,
    0b00110,
    0b01000,
    0b11111,

    // 51: 3
    0b11110,
    0b00001,
    0b00111,
    0b00001,
    0b11110,

    // 52: 4
    0b00010,
    0b00110,
    0b01010,
    0b11111,
    0b00010,

    // 53: 5
    0b11111,
    0b10000,
    0b11110,
    0b00001,
    0b11110,

    // 54: 6
    0b01111,
    0b10000,
    0b11111,
    0b10001,
    0b01110,

    // 55: 7
    0b11111,
    0b00001,
    0b00010,
    0b00100,
    0b00100,

    // 56: 8
    0b01110,
    0b10001,
    0b01110,
    0b10001,
    0b01110,

    // 57: 9
    0b01110,
    0b10001,
    0b11111,
    0b00001,
    0b11110,

    // 58: :  field1
    // 0b00000000,
    // 0b00001000,
    // 0b00000000,
    // 0b00001000,
    // 0b00000000,
    0b10101,
    0b00000,
    0b10101,
    0b00000,
    0b10101,

    // 59: ;  field2
    // 0b00000000,
    // 0b00000100,
    // 0b00000000,
    // 0b00000100,
    // 0b00001000,
    0b01010,
    0b10101,
    0b01010,
    0b10101,
    0b01010,

    // 60: <
    0b00010,
    0b00100,
    0b01000,
    0b00100,
    0b00010,

    // 61: =  field3
    // 0b00000000,
    // 0b00001110,
    // 0b00000000,
    // 0b00001110,
    // 0b00000000,
    0b10101,
    0b01010,
    0b10101,
    0b01010,
    0b10101,

    // 62: >
    0b01000,
    0b00100,
    0b00010,
    0b00100,
    0b01000,

    // 63: ?
    0b01110,
    0b10001,
    0b00110,
    0b00000,
    0b00100,

    // 64: @  field4
    // 0b00001110,
    // 0b00011111,
    // 0b00011001,
    // 0b00011101,
    // 0b00001110,
    0b11011,
    0b10001,
    0b00000,
    0b10001,
    0b11011,

    // 65: A
    0b01110,
    0b10001,
    0b11111,
    0b10001,
    0b10001,

    // 66: B
    0b11110,
    0b10001,
    0b11110,
    0b10001,
    0b11110,

    // 67: C
    0b01110,
    0b10001,
    0b10000,
    0b10001,
    0b01110,

    // 68: D
    0b11110,
    0b10001,
    0b10001,
    0b10001,
    0b11110,

    // 69: E
    0b01111,
    0b10000,
    0b11110,
    0b10000,
    0b01111,

    // 70: F
    0b11110,
    0b10000,
    0b11100,
    0b10000,
    0b10000,

    // 71: G
    0b01111,
    0b10000,
    0b10111,
    0b10001,
    0b01110,

    // 72: H
    0b10001,
    0b10001,
    0b11111,
    0b10001,
    0b10001,

    // 73: I
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b11111,

    // 74: J
    0b11111,
    0b00010,
    0b10010,
    0b10010,
    0b01100,

    // 75: K
    0b10010,
    0b10100,
    0b11000,
    0b10100,
    0b10010,

    // 76: L
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11110,

    // 77: M
    0b01110,
    0b10101,
    0b10101,
    0b10001,
    0b10001,

    // 78: N
    0b10001,
    0b11001,
    0b10101,
    0b10011,
    0b10001,

    // 79: O
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b01110,

    // 80: P
    0b11110,
    0b10001,
    0b11110,
    0b10000,
    0b10000,

    // 81: Q
    0b01110,
    0b10001,
    0b10101,
    0b10010,
    0b01101,

    // 82: R
    0b11110,
    0b10001,
    0b11110,
    0b10001,
    0b10001,

    // 83: S
    0b01111,
    0b10000,
    0b11111,
    0b00001,
    0b11110,

    // 84: T
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,

    // 85: U
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01110,

    // 86: V
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,

    // 87: W
    0b10001,
    0b10001,
    0b10101,
    0b10101,
    0b01110,

    // 88: X
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,

    // 89: Y
    0b10001,
    0b01010,
    0b00100,
    0b00100,
    0b00100,

    // 90: Z
    0b11111,
    0b00010,
    0b00100,
    0b01000,
    0b11111,

    // 91: [  field5
    // 0b00001100,
    // 0b00001000,
    // 0b00001000,
    // 0b00001000,
    // 0b00001100,
    0b11011,
    0b10101,
    0b01010,
    0b10101,
    0b11011,

    // 92: '\'
    0b10000,
    0b11000,
    0b01100,
    0b00110,
    0b00010,

    // 93: ]  ball1
    // 0b00000110,
    // 0b00000010,
    // 0b00000010,
    // 0b00000010,
    // 0b00000110,
    0b00000,
    0b10000,
    0b00000,
    0b00000,
    0b00000,

    // 94: ^
    0b00100,
    0b01010,
    0b10001,
    0b00000,
    0b00000,

    // 95: _  ball2
    // 0b00000000,
    // 0b00000000,
    // 0b00000000,
    // 0b00000000,
    // 0b00011111,
    0b00000,
    0b00000,
    0b01000,
    0b00000,
    0b00000,

    /***** Animation Frames *****/

    // 96: `  spin1
    0b10011,
    0b11010,
    0b00100,
    0b01011,
    0b11001,

    // 97: a  spin2
    0b11001,
    0b01011,
    0b00100,
    0b11010,
    0b10011,

    // 98: b  vert_bar
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,

    // 99: c  ball3
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b00000,

    // 100: d  ball4
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00100,

    // 101: e  dna1
    0b10001,
    0b01010,
    0b10001,
    0b01010,
    0b10001,

    // 102: f  dna2
    0b01010,
    0b10001,
    0b01010,
    0b10001,
    0b01010,

    // 103: g  pac1
    0b01110,
    0b11100,
    0b11000,
    0b11100,
    0b01110,

    // 104: h  pac2
    0b01110,
    0b11111,
    0b11111,
    0b11111,
    0b01110,

    // 105: i  ball6
    0b00000,
    0b00000,
    0b00010,
    0b00000,
    0b00000,

    // 106: j  star1
    0b00000,
    0b00100,
    0b01110,
    0b00100,
    0b00000,

    // 107: k  star2
    0b00000,
    0b01010,
    0b00100,
    0b01010,
    0b00000,

    // 108: l  star3
    0b00000,
    0b00100,
    0b01010,
    0b00100,
    0b00000,

    // 109: m  star4
    0b00100,
    0b00100,
    0b11011,
    0b00100,
    0b00100,

    // 110: n  star5
    0b00100,
    0b01010,
    0b10001,
    0b01010,
    0b00100,

    // 111: o  star6
    0b10001,
    0b00100,
    0b01110,
    0b00100,
    0b10001,

    // 112: p  star7
    0b10001,
    0b01010,
    0b00000,
    0b01010,
    0b10001,

    // 113: q  star8
    0b10001,
    0b00000,
    0b00000,
    0b00000,
    0b10001,

    // 114: r  frog1
    0b11011,
    0b01110,
    0b00100,
    0b01110,
    0b11011,

    // 115: s  frog2
    0b00000,
    0b11011,
    0b01110,
    0b11011,
    0b00000,

    // 116: t  ball7
    0b00000,
    0b00001,
    0b00000,
    0b00000,
    0b00000,

    // 117: u  LOST numbers "cursor" >:
    0b10000,
    0b01001,
    0b00100,
    0b01001,
    0b10000,
    
    // 118: v  heart
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,

    // 119: w  tree
    0b00000,
    0b00100,
    0b01110,
    0b01110,
    0b11111,
};

#define FLASH_CHAR_SET_SIZE_BYTES (sizeof(charSet) / sizeof(charSet[0]))
#define EEP_CHAR_DATA_NUM_CHARS  (FLASH_CHAR_SET_SIZE_BYTES / MATRIX_HEIGHT_PIX)
#define EEP_CHAR_DATA_NUM_BYTES  (DIV_CEILING((EEP_CHAR_DATA_NUM_CHARS * MATRIX_NUM_PIX) , 8))

// ASCII char set limits
#define ASCII_START 32
#define ASCII_NUM_CHARS (EEP_CHAR_DATA_NUM_CHARS)

/* 
    *****************************************************************************
    SARS-CoV-2 Data (80B)
    EEPROM 432-511
    2 bits per base, 00:A, 01:C, 10:G, 11:U
    4 bases per byte
    *****************************************************************************
    Sequence encoding from
    https://github.com/PaulKlinger/freeform-virus-blinky

    MIT License

    Copyright (c) 2020 Paul Klinger

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    Original sequence .fasta file from
    https://www.ncbi.nlm.nih.gov/nuccore/NC_045512
    *****************************************************************************
*/
static const uint8_t PROGMEM covSeqData[] = {
 60,  10, 252, 197, 245,  74, 193,   1,  65,  65, 253, 141, 223, 178,  55, 189,
220,   6,   7, 240,  13, 238, 233, 237,  29, 167, 147, 159,  46,  71,  70,  75,
 48, 240, 193, 195, 199, 182, 248,  74,  17, 139,   7, 109, 205, 247, 146, 158,
124, 107, 246, 214, 239, 146,  88, 211,  73,  19, 114, 191, 109, 106, 238,  22,
  2, 176, 142, 136, 151, 237,  94, 191,  65, 136,   1,  17, 181,   7,  75, 249,
123, 252,  74, 246,  97, 185, 219,  27, 167, 250,  33, 214, 232, 162, 183, 205,
 34, 145, 180,  19, 124,   8, 233,  31, 186, 124, 178,  11, 224,   2, 155, 254,
 93,   7, 224,  73,  92, 238, 244, 208,  27, 218,  57, 216,  30,  69, 211, 173,
 59, 206, 190,  39, 172, 146,   7,  96, 164, 244, 177, 173, 178, 235, 136,  71,
235, 181, 251,  87,  78, 234,  96,  49,  75, 167, 197, 144, 175, 125, 246, 194, };



#endif