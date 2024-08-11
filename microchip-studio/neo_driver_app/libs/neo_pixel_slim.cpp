/*!
 * @file NeoPixel_Slim.cpp
 *
 * @mainpage Arduino Library for driving Adafruit NeoPixel addressable LEDs,
 * FLORA RGB Smart Pixels and compatible devicess -- WS2811, WS2812, WS2812B,
 * SK6812, etc.
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for Adafruit's NeoPixel library for the
 * Arduino platform, allowing a broad range of microcontroller boards
 * (most AVR boards, many ARM devices, ESP8266 and ESP32, among others)
 * to control Adafruit NeoPixels, FLORA RGB Smart Pixels and compatible
 * devices -- WS2811, WS2812, WS2812B, SK6812, etc.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing products
 * from Adafruit!
 *
 * @section author Author
 *
 * Written by Phil "Paint Your Dragon" Burgess for Adafruit Industries,
 * with contributions by PJRC, Michael Miller and other members of the
 * open source community.
 *
 * @section license License
 *
 * This file is part of the Adafruit_NeoPixel library.
 *
 * Adafruit_NeoPixel is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Adafruit_NeoPixel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with NeoPixel. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#include "neo_pixel_slim.h"
// For memset
#include <string.h>

// These two tables are declared outside the Adafruit_NeoPixel class
// because some boards may require oldschool compilers that don't
// handle the C++11 constexpr keyword.

// NOTE: If these are defined in the header file instead, they get placed in flash twice!
// PROGMEM arrays should be defined in .cpp/.c files!

/* A PROGMEM (flash mem) table containing 8-bit unsigned sine wave (0-255).
     Copy & paste this snippet into a Python REPL to regenerate:
import math
for x in range(256):
        print("{:3},".format(int((math.sin(x/128.0*math.pi)+1.0)*127.5+0.5))),
        if x&15 == 15: print
*/
// 16 x 16 table
static const uint8_t PROGMEM _au8_NeoPixelSineTable[256] = {
    128,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,  // 0-15
    176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,  // 16-31
    218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,  // 32-47
    245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,  // 48-63
    255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,  // 64-79
    245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,  // 80-95
    218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,  // 96-111
    176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,  // 112-127
    128,124,121,118,115,112,109,106,103,100, 97, 93, 90, 88, 85, 82,  // 128-143
     79, 76, 73, 70, 67, 65, 62, 59, 57, 54, 52, 49, 47, 44, 42, 40,  // 144-159
     37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11,  // 160-175
     10,  9,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1,  0,  0,  0,  // 176-191
      0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  9,  // 192-207
     10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,  // 208-223
     37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,  // 224-239
     79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,118,121,124   // 240-255
};

/* Similar to above, but for an 8-bit gamma-correction table.
     Copy & paste this snippet into a Python REPL to regenerate:
import math
gamma=2.6
for x in range(256):
        print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
        if x&15 == 15: print
*/
static const uint8_t PROGMEM _au8_NeoPixelGammaTable[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
    13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
    20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
    30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
    58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
    76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
    97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
    122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
    150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
    182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
    218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255
};

/*!
    @brief   NeoPixel constructor when length, pin and pixel type are known
                     at compile-time.
    @param   n  Number of NeoPixels in strand.
    @param   p  Arduino pin number which will drive the NeoPixel data in.
    @return  Adafruit_NeoPixel object. Call the begin() function before use.
*/
NeoPixel_Slim::NeoPixel_Slim(uint8_t* pu8_pixData, uint8_t u8_maxPix,
                             uint8_t u8_numPix, uint8_t u8_pinMask) :
    b_begun(false), u8_brightness(0), pu8_pixels(pu8_pixData), u16_endTime(0),
    u8_numLEDs(u8_numPix), u8_maxLEDs(u8_maxPix), u16_numBytes(3*u8_numPix) {

    // AVR
    pu8_port = &PORTB; // Only PORTB on ATtiny85
    pu8_ddr = &DDRB;
    this->u8_pinMask = u8_pinMask;
}

/*!
    @brief   Configure NeoPixel pin for output.
*/
void NeoPixel_Slim::begin(void) {

    // 1 in the DDR means output
    bitSetMask(*pu8_ddr, u8_pinMask);
    // Set pin LOW initially
    bitClearMask(*pu8_port, u8_pinMask);
    b_begun = true;
}

/*!
    @brief   Change the length of a previously-declared Adafruit_NeoPixel
                     strip object. Old data is deallocated and new data is cleared.
                     Pin number and pixel format are unchanged.
    @param   n  New length of strip, in pixels.
    @note    This function is deprecated, here only for old projects that
                     may still be calling it. New projects should instead use the
                     'new' keyword with the first constructor syntax (length, pin,
                     type).
*/
void NeoPixel_Slim::update_length(uint8_t u8_numPix) {

    // Clip the number if it is larger than we have space for in the array
    if (u8_numPix > u8_maxLEDs) {
        u8_numPix = u8_maxLEDs;
    }

    // Store new values
    u8_numLEDs = u8_numPix;
    u16_numBytes = 3*u8_numLEDs;
    
}


/*!
    @brief   Transmit pixel data in RAM to NeoPixels.
    @note    On most architectures, interrupts are temporarily disabled in
                     order to achieve the correct NeoPixel signal timing. This means
                     that the Arduino millis() and micros() functions, which require
                     interrupts, will lose small intervals of time whenever this
                     function is called (about 30 microseconds per RGB pixel, 40 for
                     RGBW pixels). There's no easy fix for this, but a few
                     specialized alternative or companion libraries exist that use
                     very device-specific peripherals to work around it.
*/
void NeoPixel_Slim::show(void) {

    if (!pu8_pixels) {
        return;
    }

    // Data latch = 300+ microsecond pause in the output stream. Rather than
    // put a delay at the end of the function, the ending time is noted and
    // the function will simply hold off (if needed) on issuing the
    // subsequent round of data until the latch time has elapsed. This
    // allows the mainline code to start generating the next frame of data
    // rather than stalling for the latch.
    while(!can_show()) {}
    // endTime is a private member (rather than global var) so that multiple
    // instances on different pins can be quickly issued in succession (each
    // instance doesn't delay the next).

    // In order to make this code runtime-configurable to work with any pin,
    // SBI/CBI instructions are eschewed in favor of full PORT writes via the
    // OUT or ST instructions. It relies on two facts: that peripheral
    // functions (such as PWM) take precedence on output pins, so our PORT-
    // wide writes won't interfere, and that interrupts are globally disabled
    // while data is being issued to the LEDs, so no other code will be
    // accessing the PORT. The code takes an initial 'snapshot' of the PORT
    // state, computes 'pin high' and 'pin low' values, and writes these back
    // to the PORT register as needed.

    noInterrupts(); // Need 100% focus on instruction timing

// AVR MCUs -- ATmega & ATtiny (no XMEGA) ---------------------------------

    volatile uint16_t
         i    =  u16_numBytes; // Loop counter
    volatile uint8_t
        *ptr  =  pu8_pixels,   // Pointer to next byte
         b    = *ptr++,        // Current byte value
         hi,                   // PORT w/output bit set high
         lo;                   // PORT w/output bit set low

    // Hand-tuned assembly code issues data to the LED drivers at a specific
    // rate. There's separate code for different CPU speeds (8, 12, 16 MHz)
    // for both the WS2811 (400 KHz) and WS2812 (800 KHz) drivers. The
    // datastream timing for the LED drivers allows a little wiggle room each
    // way (listed in the datasheets), so the conditions for compiling each
    // case are set up for a range of frequencies rather than just the exact
    // 8, 12 or 16 MHz values, permitting use with some close-but-not-spot-on
    // devices (e.g. 16.5 MHz DigiSpark). The ranges were arrived at based
    // on the datasheet figures and have not been extensively tested outside
    // the canonical 8/12/16 MHz speeds; there's no guarantee these will work
    // close to the extremes (or possibly they could be pushed further).
    // Keep in mind only one CPU speed case actually gets compiled; the
    // resulting program isn't as massive as it might look from source here.

// 8 MHz(ish) AVR ---------------------------------------------------------

        volatile uint8_t n1, n2 = 0;  // First, next bits out

        // Squeezing an 800 KHz stream out of an 8 MHz chip requires code
        // specific to each PORT register.

        // 10 instruction clocks per bit: HHxxxxxLLL
        // OUT instructions:              ^ ^    ^   (T=0,2,7)

        // PORTB OUTPUT ----------------------------------------------------

            hi = PORTB |  u8_pinMask;
            lo = PORTB & ~u8_pinMask;
            n1 = lo;
            if(b & 0x80) n1 = hi;

            // Dirty trick: RJMPs proceeding to the next instruction are used
            // to delay two clock cycles in one instruction word (rather than
            // using two NOPs). This was necessary in order to squeeze the
            // loop down to exactly 64 words -- the maximum possible for a
            // relative branch.

            asm volatile(
             "headD:"                      "\n\t" // Clk  Pseudocode
                // Bit 7:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
                "out   %[port] , %[n1]"    "\n\t" // 1    PORT = n1
                "rjmp  .+0"                "\n\t" // 2    nop nop
                "sbrc  %[byte] , 6"        "\n\t" // 1-2  if(b & 0x40)
                "mov   %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "rjmp  .+0"                "\n\t" // 2    nop nop
                // Bit 6:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
                "out   %[port] , %[n2]"    "\n\t" // 1    PORT = n2
                "rjmp  .+0"                "\n\t" // 2    nop nop
                "sbrc  %[byte] , 5"        "\n\t" // 1-2  if(b & 0x20)
                "mov   %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "rjmp  .+0"                "\n\t" // 2    nop nop
                // Bit 5:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
                "out   %[port] , %[n1]"    "\n\t" // 1    PORT = n1
                "rjmp  .+0"                "\n\t" // 2    nop nop
                "sbrc  %[byte] , 4"        "\n\t" // 1-2  if(b & 0x10)
                "mov   %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "rjmp  .+0"                "\n\t" // 2    nop nop
                // Bit 4:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
                "out   %[port] , %[n2]"    "\n\t" // 1    PORT = n2
                "rjmp  .+0"                "\n\t" // 2    nop nop
                "sbrc  %[byte] , 3"        "\n\t" // 1-2  if(b & 0x08)
                "mov   %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "rjmp  .+0"                "\n\t" // 2    nop nop
                // Bit 3:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
                "out   %[port] , %[n1]"    "\n\t" // 1    PORT = n1
                "rjmp  .+0"                "\n\t" // 2    nop nop
                "sbrc  %[byte] , 2"        "\n\t" // 1-2  if(b & 0x04)
                "mov   %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "rjmp  .+0"                "\n\t" // 2    nop nop
                // Bit 2:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
                "out   %[port] , %[n2]"    "\n\t" // 1    PORT = n2
                "rjmp  .+0"                "\n\t" // 2    nop nop
                "sbrc  %[byte] , 1"        "\n\t" // 1-2  if(b & 0x02)
                "mov   %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "rjmp  .+0"                "\n\t" // 2    nop nop
                // Bit 1:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
                "out   %[port] , %[n1]"    "\n\t" // 1    PORT = n1
                "rjmp  .+0"                "\n\t" // 2    nop nop
                "sbrc  %[byte] , 0"        "\n\t" // 1-2  if(b & 0x01)
                "mov   %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "sbiw  %[count], 1"        "\n\t" // 2    i-- (don't act on Z flag yet)
                // Bit 0:
                "out   %[port] , %[hi]"    "\n\t" // 1    PORT = hi
                "mov   %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
                "out   %[port] , %[n2]"    "\n\t" // 1    PORT = n2
                "ld    %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++
                "sbrc  %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80)
                "mov   %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
                "out   %[port] , %[lo]"    "\n\t" // 1    PORT = lo
                "brne headD"               "\n"   // 2    while(i) (Z flag set above)
            : [byte]    "+r" (b),
                [n1]    "+r" (n1),
                [n2]    "+r" (n2),
                [count] "+w" (i)
            : [port]    "I"  (_SFR_IO_ADDR(PORTB)),
                [ptr]   "e"  (ptr),
                [hi]    "r"  (hi),
                [lo]    "r"  (lo));


    interrupts();
    u16_endTime = micros(); // Save EOD time for latch on next call
}

/*!
    @brief   Set a pixel's color using separate red, green and blue
                     components.
    @param   n  Pixel index, starting from 0.
    @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
    @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
    @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
*/
void NeoPixel_Slim::set_pix_color(uint8_t u8_index, uint8_t u8_red, uint8_t u8_green,
                                  uint8_t u8_blue) {
    
    // Verify that the index is in range
    if (u8_index < u8_numLEDs) {

        // Scale brightness of each color
        if (u8_brightness) { // See notes in setBrightness()
            u8_red =   (u8_red   * u8_brightness)   >> 8;
            u8_green = (u8_green * u8_brightness)   >> 8;
            u8_blue =  (u8_blue  * u8_brightness)   >> 8;
        }

        // Lookup location in pixel data array
        uint8_t* pu8_pixStart;
        pu8_pixStart = &pu8_pixels[u8_index * 3];    // 3 bytes per pixel

        // GRB transmit order
        pu8_pixStart[0] = u8_green;
        pu8_pixStart[1] = u8_red;
        pu8_pixStart[2] = u8_blue;
    }
}


/*!
    @brief   Set a pixel's color using a 32-bit 'packed' RGB value.
    @param   n  Pixel index, starting from 0.
    @param   c  32-bit color value. Most significant byte is ignored (for RGB pixels), 
                            next is red, then green, and least significant byte is blue.
*/
void NeoPixel_Slim::set_pix_color(uint8_t u8_index, uint32_t u32_color) {
    
    // Extract each color
    uint8_t u8_red =   (uint8_t)(u32_color >> 16);
    uint8_t u8_green = (uint8_t)(u32_color >>  8);
    uint8_t u8_blue =  (uint8_t)(u32_color >>  0);

    // Call the other overload of this function
    this->set_pix_color(u8_index, u8_red, u8_green, u8_blue);

}

/*!
    @brief   Fill all or part of the NeoPixel strip with a color.
    @param   c      32-bit color value. Most significant byte is white (for
                                    RGBW pixels) or ignored (for RGB pixels), next is red,
                                    then green, and least significant byte is blue. If all
                                    arguments are unspecified, this will be 0 (off).
    @param   first  Index of first pixel to fill, starting from 0. Must be
                                    in-bounds, no clipping is performed. 0 if unspecified.
    @param   count  Number of pixels to fill, as a positive value. Passing
                                    0 or leaving unspecified will fill to end of strip.
*/
void NeoPixel_Slim::fill(uint32_t u32_color, uint8_t u8_firstIndex, uint8_t u8_count) {

    // Local Vars
    uint8_t u8_index;
    uint8_t u8_endPix;

    // If first LED is past end of strip, nothing to do
    if (u8_firstIndex >= u8_numLEDs) {
        return;
    }

    // Calculate the index ONE AFTER the last pixel to fill
    if (u8_count == 0) {
        // Fill to end of strip
        u8_endPix = u8_numLEDs;
    } else {
        // Ensure that the loop won't go past the last pixel
        u8_endPix = u8_firstIndex + u8_count;
        if (u8_endPix > u8_numLEDs) {
            u8_endPix = u8_numLEDs;
        }
    }

    for (u8_index = u8_firstIndex; u8_index < u8_endPix; u8_index++) {
        this->set_pix_color(u8_index, u32_color);
    }
}

/*!
    @brief   Convert hue, saturation and value into a packed 32-bit RGB color
                     that can be passed to setPixelColor() or other RGB-compatible
                     functions.
    @param   hue  An unsigned 16-bit value, 0 to 65535, representing one full
                                loop of the color wheel, which allows 16-bit hues to "roll
                                over" while still doing the expected thing (and allowing
                                more precision than the wheel() function that was common to
                                prior NeoPixel examples).
    @param   sat  Saturation, 8-bit value, 0 (min or pure grayscale) to 255
                                (max or pure hue). Default of 255 if unspecified.
    @param   val  Value (brightness), 8-bit value, 0 (min / black / off) to
                                255 (max or full brightness). Default of 255 if unspecified.
    @return  Packed 32-bit RGB with the most significant byte set to 0 -- the
                     white element of WRGB pixels is NOT utilized. Result is linearly
                     but not perceptually correct, so you may want to pass the result
                     through the gamma32() function (or your own gamma-correction
                     operation) else colors may appear washed out. This is not done
                     automatically by this function because coders may desire a more
                     refined gamma-correction function than the simplified
                     one-size-fits-all operation of gamma32(). Diffusing the LEDs also
                     really seems to help when using low-saturation colors.
*/
uint32_t NeoPixel_Slim::hsv_to_pack(uint16_t u16_hue, uint8_t u8_sat, uint8_t u8_val) {

    uint8_t u8_red;
    uint8_t u8_green;
    uint8_t u8_blue;

    // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
    // 0 is not the start of pure red, but the midpoint...a few values above
    // zero and a few below 65536 all yield pure red (similarly, 32768 is the
    // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
    // each for red, green, blue) really only allows for 1530 distinct hues
    // (not 1536, more on that below), but the full unsigned 16-bit type was
    // chosen for hue so that one's code can easily handle a contiguous color
    // wheel by allowing hue to roll over in either direction.
    u16_hue = (u16_hue * 1530L + 32768) / 65536;
    // Because red is centered on the rollover point (the +32768 above,
    // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
    // where 0 and 1530 would yield the same thing. Rather than apply a
    // costly modulo operator, 1530 is handled as a special case below.

    // So you'd think that the color "hexcone" (the thing that ramps from
    // pure red, to pure yellow, to pure green and so forth back to red,
    // yielding six slices), and with each color component having 256
    // possible values (0-255), might have 1536 possible items (6*256),
    // but in reality there's 1530. This is because the last element in
    // each 256-element slice is equal to the first element of the next
    // slice, and keeping those in there this would create small
    // discontinuities in the color wheel. So the last element of each
    // slice is dropped...we regard only elements 0-254, with item 255
    // being picked up as element 0 of the next slice. Like this:
    // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
    // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
    // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
    // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
    // the constants below are not the multiples of 256 you might expect.

    // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
    if (u16_hue < 510) {               // Red to Green-1
        u8_blue = 0;
        if (u16_hue < 255) {           // Red to Yellow-1
            u8_red = 255;
            u8_green = u16_hue;        // g = 0 to 254
        } else {                       // Yellow to Green-1
            u8_red = 510 - u16_hue;    // r = 255 to 1
            u8_green = 255;
        }
    } else if (u16_hue < 1020) {       // Green to Blue-1
        u8_red = 0;
        if (u16_hue <  765) {          // Green to Cyan-1
            u8_green = 255;
            u8_blue = u16_hue - 510;   // b = 0 to 254
        } else {                       // Cyan to Blue-1
            u8_green = 1020 - u16_hue; // g = 255 to 1
            u8_blue = 255;
        }
    } else if (u16_hue < 1530) {       // Blue to Red-1
        u8_green = 0;
        if (u16_hue < 1275) {          // Blue to Magenta-1
            u8_red = u16_hue - 1020;   // r = 0 to 254
            u8_blue = 255;
        } else {                       // Magenta to Red-1
            u8_red = 255;
            u8_blue = 1530 - u16_hue;  // b = 255 to 1
        }
    } else {                           // Last 0.5 Red (quicker than % operator)
        u8_red = 255;
        u8_green = u8_blue = 0;
    }

    // Apply saturation and value to R,G,B, pack into 32-bit result:
    uint32_t u32_valPlus1 =   1 + u8_val;  // 1 to 256; allows >>8 instead of /255
    uint16_t u16_satPlus1 =   1 + u8_sat;  // 1 to 256; same reason
    uint8_t  u8_satInvert = 255 - u8_sat;  // 255 to 0

    return ((((((u8_red   * u16_satPlus1) >> 8) + u8_satInvert) * u32_valPlus1) & 0xff00)  << 8)  |
            (((((u8_green * u16_satPlus1) >> 8) + u8_satInvert) * u32_valPlus1) & 0xff00)         |
            (((((u8_blue  * u16_satPlus1) >> 8) + u8_satInvert) * u32_valPlus1)            >> 8);
}


/*!
    @brief   Adjust output brightness. Does not immediately affect what's
                     currently displayed on the LEDs. The next call to show() will
                     refresh the LEDs at this level.
    @param   b  Brightness setting, 0=minimum (off), 255=brightest.
    @note    This was intended for one-time use in one's setup() function,
                     not as an animation effect in itself. Because of the way this
                     library "pre-multiplies" LED colors in RAM, changing the
                     brightness is often a "lossy" operation -- what you write to
                     pixels isn't necessary the same as what you'll read back.
                     Repeated brightness changes using this function exacerbate the
                     problem. Smart programs therefore treat the strip as a
                     write-only resource, maintaining their own state to render each
                     frame of an animation, not relying on read-modify-write.
*/
void NeoPixel_Slim::set_brightness(uint8_t u8_brightness) {
    // Stored brightness value is different than what's passed.
    // This simplifies the actual scaling math later, allowing a fast
    // 8x8-bit multiply and taking the MSB. 'brightness' is a uint8_t,
    // adding 1 here may (intentionally) roll over...so 0 = max brightness
    // (color values are interpreted literally; no scaling), 1 = min
    // brightness (off), 255 = just below max brightness.
    this->u8_brightness = u8_brightness + 1;

    // Removed the rescaling code because I don't need it in my application.
}

/*!
    @brief   Retrieve the last-set brightness value for the strip.
    @return  Brightness value: 0 = minimum (off), 255 = maximum.
*/
uint8_t NeoPixel_Slim::get_brightness(void) const {
    return u8_brightness - 1;
}

/*!
    @brief   Fill the whole NeoPixel strip with 0 / black / off.
*/
void NeoPixel_Slim::clear(void) {
    memset(pu8_pixels, 0, u16_numBytes);
}

  /*!
    @brief   A gamma-correction function for 32-bit packed RGB or WRGB
             colors. Makes color transitions appear more perceptially
             correct.
    @param   x  32-bit packed RGB or WRGB color.
    @return  Gamma-adjusted packed color, can then be passed in one of the
             setPixelColor() functions. Like gamma8(), this uses a fixed
             gamma correction exponent of 2.6, which seems reasonably okay
             for average NeoPixels in average tasks. If you need finer
             control you'll need to provide your own gamma-correction
             function instead.
  */
uint32_t NeoPixel_Slim::get_gamma_32(uint32_t u32_color) {

    // Get a pointer to the start of the 32 bit int so we can address the bytes directly
    uint8_t* pu8_color = (uint8_t*)&u32_color;

    // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
    // to avoid a bunch of shifting and masking that would be necessary for
    // properly handling different endianisms (and each byte is a fairly
    // trivial operation, so it might not even be wasting cycles vs a check
    // and branch for the RGB case). In theory this might cause trouble *if*
    // someone's storing information in the unused most significant byte
    // of an RGB value, but this seems exceedingly rare and if it's
    // encountered in reality they can mask values going in or coming out.
    for (uint8_t u8_i = 0; u8_i < 4; u8_i++) {
        pu8_color[u8_i] = get_gamma_8(pu8_color[u8_i]);
    }

    return u32_color; // Packed 32-bit return
}

uint8_t NeoPixel_Slim::get_sine_8(uint8_t u8_x) {
    return pgm_read_byte(&_au8_NeoPixelSineTable[u8_x]); // 0-255 in, 0-255 out
}

uint8_t NeoPixel_Slim::get_gamma_8(uint8_t u8_x) {
    return pgm_read_byte(&_au8_NeoPixelGammaTable[u8_x]); // 0-255 in, 0-255 out
}
