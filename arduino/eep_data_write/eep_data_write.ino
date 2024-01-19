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
#include <neo_common.h>
#include <ard_utility.h>

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
    eep_compressed_chars(false);

    #ifdef EEP_COV_DATA_WRITE_ENABLE
    for (uint16_t i = 0; i < EEP_COV_DATA_NUM_BYTES; i++) {
        EEPROM.update(EEP_COV_DATA_START_ADDR+i, pgm_read_byte(&covSeqData[i]));
    }
    #endif

    // Verify EEPROM Data
    bool dataOK = true;
    if (!eep_compressed_chars(true)) { dataOK = false; }

    #ifdef EEP_COV_DATA_WRITE_ENABLE
    for (uint16_t i = 0; i < EEP_COV_DATA_NUM_BYTES; i++) {
        if (EEPROM.read(EEP_COV_DATA_START_ADDR+i) != pgm_read_byte(&covSeqData[i])) { dataOK = false; }
    }
    #endif

    uint32_t flash_color;
    if (dataOK)
    {
        flash_color = COLOR_GREEN;
    }
    else
    {
        flash_color = COLOR_RED;
    }
    
    // Indicate verification status
    strip.clear();
    for (int i=0; i<3; i++) {
        strip.setPixelColor(0, flash_color);
        strip.show();
        delay(500);

        strip.clear();
        strip.show();
        delay(500);
    }


}

/********************************* MAIN LOOP *********************************/
void loop() {

}

// Shift in upper num_bits from data_in, into data, from the right
// num_bits should be <= 8
void shift_in_from_right(uint8_t* data, const uint8_t data_in, const uint8_t num_bits)
{
    if (num_bits == 0)
    {
        return;
    }
    
    // A bit mask with num_bits of 1's in lower positions
    uint8_t bitmask = (1 << num_bits) - 1;
    // Handle 8 bits specially, overflows the above left shift..
    if (num_bits == 8)
    {
        bitmask = 0xFF;
    }
    
    *data <<= num_bits;
    *data |= ((data_in >> (8-num_bits)) & bitmask);
}

/*
    validate = false -> "Update" EEPROM with correct values, possibly performing writes
    validate = true  -> Check EEPROM for correct values, performing no writes, returning status
*/
bool eep_compressed_chars(bool validate)
{
    uint16_t eep_addr = EEP_CHAR_DATA_START_ADDR;
    uint8_t input_byte = 0;
    uint16_t input_byte_cnt = 0;
    uint8_t input_bits_rem = 0;
    uint8_t output_byte = 0;
    uint8_t output_bits_rem = 8;
    uint8_t bits_to_copy = 0;
    bool input_exhausted = false;
    bool dataOK = true;

    // Each iteration does a copy of some bits from input to output byte
    do
    {
        // Read a new input byte
        if (input_bits_rem == 0)
        {
            if (input_byte_cnt == FLASH_CHAR_SET_SIZE_BYTES)
            {
                // Copy zeros into remaining lower bits in output, flush, and terminate loop
                input_exhausted = true;
                input_byte = 0;
                input_bits_rem = output_bits_rem;
            }
            else
            {
                input_byte = pgm_read_byte(&charSet[input_byte_cnt]);
                input_byte <<= (8-MATRIX_WIDTH_PIX);
                input_bits_rem = MATRIX_WIDTH_PIX;
                input_byte_cnt++;
            }
        }
        
        // Copy bits
        bits_to_copy = min(input_bits_rem, output_bits_rem);
        shift_in_from_right(&output_byte, input_byte, bits_to_copy);

        // Prepare for next copy
        input_byte <<= bits_to_copy;
        input_bits_rem -= bits_to_copy;
        output_bits_rem -= bits_to_copy;

        // Flush the output byte
        if (output_bits_rem == 0)
        {
            if (validate)
            {
                if (EEPROM.read(eep_addr) != output_byte) { dataOK = false; }
            }
            else
            {
                EEPROM.update(eep_addr, output_byte);
            }
            output_byte = 0;
            output_bits_rem = 8;
            eep_addr++;
        }

    } while (!input_exhausted);
    
    return dataOK;
}