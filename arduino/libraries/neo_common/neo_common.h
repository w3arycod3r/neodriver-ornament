/* File:      neo_common.h
 * Author:    Garrett Carter
 * Purpose:   Common def's for eep_data_write and neo_driver_app, so they don't get out of sync
 */

#ifndef NEO_COMMON_H
#define NEO_COMMON_H

// This lib depends on another
#include <ard_utility.h>

/********************************** DEFINES **********************************/

#define MATRIX_WIDTH_PIX    5
#define MATRIX_HEIGHT_PIX   5
#define MATRIX_NUM_PIX (MATRIX_WIDTH_PIX * MATRIX_HEIGHT_PIX)

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
#define EEP_SETT_ANIM   0
#define EEP_SETT_NPIX   1
#define EEP_SETT_RSEED  2   // Addr 2-5, 4 bytes
#define EEP_SETT_FLAGS  6   // 8 Status Flags

// Status Flags (bits in EEP_SETT_FLAGS)
#define STAT_FLG_BATT_DEAD  BIT0


// EEPROM Ranges & Sizes
#define EEP_SIZE_BYTES 512



// Comment out below line to skip writing the cov virus data, if you don't intend to use the animation
// #define EEP_COV_DATA_WRITE_ENABLE
#define EEP_COV_DATA_START_ADDR   432
// 160 Bytes of sequence are available
#define EEP_COV_DATA_NUM_BYTES   80

/*
    5x5 font inspired by: http://batchout.blogspot.com/2018/02/five-by-five-my-5x5-pixel-font.html
    
    Character Data
    EEPROM Addr 16-511
    (Extended) ASCII 32-189

    25 bits per glyph
    With compression, we have space for up to 158 glyphs, if cov animation not used.
*/
const uint8_t PROGMEM charSet[] = {
    // [ASCII: CHAR]

    // 32: Space
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // 33: !
    0b00001000,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00001000,

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
    0b00000001,
    0b00000011,
    0b00000110,
    0b00001100,
    0b00001000,

    // 48: 0
    0b00001110,
    0b00010001,
    0b00010101,
    0b00010001,
    0b00001110,

    // 49: 1
    0b00001100,
    0b00010100,
    0b00000100,
    0b00000100,
    0b00011111,

    // 50: 2
    0b00001110,
    0b00010001,
    0b00000110,
    0b00001000,
    0b00011111,

    // 51: 3
    0b00011110,
    0b00000001,
    0b00000111,
    0b00000001,
    0b00011110,

    // 52: 4
    0b00000010,
    0b00000110,
    0b00001010,
    0b00011111,
    0b00000010,

    // 53: 5
    0b00011111,
    0b00010000,
    0b00011110,
    0b00000001,
    0b00011110,

    // 54: 6
    0b00001111,
    0b00010000,
    0b00011111,
    0b00010001,
    0b00001110,

    // 55: 7
    0b00011111,
    0b00000001,
    0b00000010,
    0b00000100,
    0b00000100,

    // 56: 8
    0b00001110,
    0b00010001,
    0b00001110,
    0b00010001,
    0b00001110,

    // 57: 9
    0b00001110,
    0b00010001,
    0b00011111,
    0b00000001,
    0b00011110,

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
    0b00000010,
    0b00000100,
    0b00001000,
    0b00000100,
    0b00000010,

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
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000100,
    0b00001000,

    // 63: ?
    0b00001110,
    0b00010001,
    0b00000110,
    0b00000000,
    0b00000100,

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
    0b00001110,
    0b00010001,
    0b00011111,
    0b00010001,
    0b00010001,

    // 66: B
    0b00011110,
    0b00010001,
    0b00011110,
    0b00010001,
    0b00011110,

    // 67: C
    0b00001110,
    0b00010001,
    0b00010000,
    0b00010001,
    0b00001110,

    // 68: D
    0b00011110,
    0b00010001,
    0b00010001,
    0b00010001,
    0b00011110,

    // 69: E
    0b00001111,
    0b00010000,
    0b00011110,
    0b00010000,
    0b00001111,

    // 70: F
    0b00011110,
    0b00010000,
    0b00011100,
    0b00010000,
    0b00010000,

    // 71: G
    0b00001111,
    0b00010000,
    0b00010111,
    0b00010001,
    0b00001110,

    // 72: H
    0b00010001,
    0b00010001,
    0b00011111,
    0b00010001,
    0b00010001,

    // 73: I
    0b00011111,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00011111,

    // 74: J
    0b00011111,
    0b00000010,
    0b00010010,
    0b00010010,
    0b00001100,

    // 75: K
    0b00010010,
    0b00010100,
    0b00011000,
    0b00010100,
    0b00010010,

    // 76: L
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00011110,

    // 77: M
    0b00001110,
    0b00010101,
    0b00010101,
    0b00010001,
    0b00010001,

    // 78: N
    0b00010001,
    0b00011001,
    0b00010101,
    0b00010011,
    0b00010001,

    // 79: O
    0b00001110,
    0b00010001,
    0b00010001,
    0b00010001,
    0b00001110,

    // 80: P
    0b00011110,
    0b00010001,
    0b00011110,
    0b00010000,
    0b00010000,

    // 81: Q
    0b00001110,
    0b00010001,
    0b00010101,
    0b00010010,
    0b00001101,

    // 82: R
    0b00011110,
    0b00010001,
    0b00011110,
    0b00010001,
    0b00010001,

    // 83: S
    0b00001111,
    0b00010000,
    0b00011111,
    0b00000001,
    0b00011110,

    // 84: T
    0b00011111,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,

    // 85: U
    0b00010001,
    0b00010001,
    0b00010001,
    0b00010001,
    0b00001110,

    // 86: V
    0b00010001,
    0b00010001,
    0b00010001,
    0b00001010,
    0b00000100,

    // 87: W
    0b00010001,
    0b00010001,
    0b00010101,
    0b00010101,
    0b00001110,

    // 88: X
    0b00010001,
    0b00001010,
    0b00000100,
    0b00001010,
    0b00010001,

    // 89: Y
    0b00010001,
    0b00001010,
    0b00000100,
    0b00000100,
    0b00000100,

    // 90: Z
    0b00011111,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00011111,

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
    0b00010000,
    0b00011000,
    0b00001100,
    0b00000110,
    0b00000010,

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
    
};

#define FLASH_CHAR_SET_SIZE_BYTES (sizeof(charSet) / sizeof(charSet[0]))
#define EEP_CHAR_DATA_NUM_CHARS  (FLASH_CHAR_SET_SIZE_BYTES / MATRIX_HEIGHT_PIX)
#define EEP_CHAR_DATA_START_ADDR 16
#define EEP_CHAR_DATA_NUM_BYTES  (DIV_CEILING((EEP_CHAR_DATA_NUM_CHARS * MATRIX_NUM_PIX) , 8))

// ASCII char set limits
#define ASCII_START 32
#define ASCII_NUM_CHARS (EEP_CHAR_DATA_NUM_CHARS)


#endif