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
#include <neo_pixel_slim.h>
#include <neo_common.h>
#include <ard_utility.h>

/******************************** PROTOTYPES *********************************/
void shift_in_from_right(uint8_t* data, const uint8_t data_in, const uint8_t num_bits);
bool eep_compressed_chars(bool validate);

/********************************** DEFINES **********************************/

/******************************** GLOBAL VARS ********************************/
uint8_t au8_pixelData[PIX_CNT_MAX*3];  // 3 bytes of color data per pixel

/****************************** GLOBAL OBJECTS *******************************/
NeoPixel_Slim o_strip(au8_pixelData, PIX_CNT_MAX, PIX_CNT_INIT, IO_NP_DATA);


/****************************** FLASH CONSTANTS ******************************/
/*
    Settings Data (7B)
    EEPROM 0-6
*/

/****************************** SETUP FUNCTION *******************************/
void setup() {

    // Setup I/O
    bitSetMask(DDRB, IO_NP_ENABLE);               // Set as o/p
    bitSetMask(PORTB, IO_NP_ENABLE);              // Enable MOSFET for NeoPixel power
    delay(5);                                     // Time for MOSFET to switch on
    bitClearMask(DDRB, IO_SW_LEFT | IO_SW_RIGHT); // Set as i/p
    bitSetMask(PORTB, IO_SW_LEFT | IO_SW_RIGHT);  // Enable pull-ups

    // Init and clear NeoPixels
    o_strip.begin();
    o_strip.show();
    o_strip.set_brightness(CONFIRM_BLINK_BRIGHT_LVL);

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
    o_strip.clear();
    for (int i=0; i<3; i++) {
        o_strip.set_pix_color(0, flash_color);
        o_strip.show();
        delay(500);

        o_strip.clear();
        o_strip.show();
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