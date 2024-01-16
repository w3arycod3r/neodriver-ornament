/* File:      eep_data_write.ino
 * Author:    Garrett Carter
 * Purpose:   Write the EEPROM data needed for the main NeoDriver program
 */

/*  
    EEPROM MEMORY MAP (512B total):
    0-6:     (7B)   Settings Data
    7-431:   (425B) Character Data (85 chars) (ASCII 32-116)
    432-511: (80B)  SARS-CoV-2 Sequence Data (320 bases)
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <eep_common.h>

/********************************** DEFINES **********************************/

// NeoPixel Config
#define MAX_PIX 25
#define INIT_BRIGHT 50

// I/O Mapping
#define NP_EN      2    // PB2 - NeoPixel Enable
#define LED_PIN    3    // PB3

/****************************** GLOBAL OBJECTS *******************************/
Adafruit_NeoPixel strip(MAX_PIX, LED_PIN, NEO_GRB + NEO_KHZ800);

/******************************** GLOBAL VARS ********************************/



/****************************** FLASH CONSTANTS ******************************/
/*
    Settings Data (7B)
    EEPROM 0-6
*/

/*
    5x5 font inspired by: http://batchout.blogspot.com/2018/02/five-by-five-my-5x5-pixel-font.html
    
    Character Data (425B)
    EEPROM 7-431
    ASCII 32-116
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
const uint8_t PROGMEM covSeqData[] = {
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


/****************************** SETUP FUNCTION *******************************/
void setup() {

    // Enable NeoPixel Power
    pinMode(NP_EN, OUTPUT);
    digitalWrite(NP_EN, HIGH);

    // Init and clear NeoPixels
    strip.begin();
    strip.show();
    strip.setBrightness(INIT_BRIGHT);

    // Update EEPROM Data
    for (uint16_t i = 0; i < EEP_CHAR_DATA_NUM_BYTES; i++) {
        EEPROM.update(EEP_CHAR_DATA_START_ADDR+i, pgm_read_byte(&charSet[i]));
    }

    #ifdef EEP_COV_DATA_WRITE_ENABLE
    for (uint16_t i = 0; i < EEP_COV_DATA_NUM_BYTES; i++) {
        EEPROM.update(EEP_COV_DATA_START_ADDR+i, pgm_read_byte(&covSeqData[i]));
    }
    #endif

    // Verify EEPROM Data
    bool dataOK = true;
    for (uint16_t i = 0; i < EEP_CHAR_DATA_NUM_BYTES; i++) {
        if (EEPROM.read(EEP_CHAR_DATA_START_ADDR+i) != pgm_read_byte(&charSet[i])) { dataOK = false; }
    }

    #ifdef EEP_COV_DATA_WRITE_ENABLE
    for (uint16_t i = 0; i < EEP_COV_DATA_NUM_BYTES; i++) {
        if (EEPROM.read(EEP_COV_DATA_START_ADDR+i) != pgm_read_byte(&covSeqData[i])) { dataOK = false; }
    }
    #endif


    // Confirm success to user
    if (dataOK)
    {
        strip.clear();
        for (int i=0; i<3; i++) {
            strip.setPixelColor(0, 0, 255, 0);  // Green
            strip.show();
            delay(500);

            strip.clear();
            strip.show();
            delay(500);
        }
    }

}

/********************************* MAIN LOOP *********************************/
void loop() {

}

