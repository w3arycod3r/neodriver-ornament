/*!
 * @file NeoPixel_Slim.h
 *
 * This is part of Adafruit's NeoPixel library for the Arduino platform,
 * allowing a broad range of microcontroller boards (most AVR boards,
 * many ARM devices, ESP8266 and ESP32, among others) to control Adafruit
 * NeoPixels, FLORA RGB Smart Pixels and compatible devices -- WS2811,
 * WS2812, WS2812B, SK6812, etc.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing products
 * from Adafruit!
 *
 * Written by Phil "Paint Your Dragon" Burgess for Adafruit Industries,
 * with contributions by PJRC, Michael Miller and other members of the
 * open source community.
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
 * License along with NeoPixel.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NEOPIXEL_SLIM_H
#define NEOPIXEL_SLIM_H

#include <Arduino.h>
#include <stdint.h>
#include <ard_utility.h>

// These two tables are declared outside the Adafruit_NeoPixel class
// because some boards may require oldschool compilers that don't
// handle the C++11 constexpr keyword.

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
     79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,118,121,124}; // 240-255

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
    218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255};

/*! 
        @brief  Class that stores state and functions for interacting with
                        Adafruit NeoPixels and compatible devices.
*/
class NeoPixel_Slim {
    public:

                      NeoPixel_Slim(uint8_t* pu8_pixData, uint8_t u8_maxPix,
                                    uint8_t u8_numPix, uint8_t u8_pin);

    void              begin(void);
    void              show(void);
    void              set_pix_color(uint8_t u8_index, uint8_t u8_red, uint8_t u8_green, uint8_t u8_blue);
    void              set_pix_color(uint8_t u8_index, uint32_t u32_color);
    void              fill(uint32_t u32_color, uint8_t u8_firstIndex=0, uint8_t u8_count=0);
    void              set_brightness(uint8_t u8_brightness);
    void              clear(void);
    void              update_length(uint8_t u8_numPix);

    /*!
        @brief   Check whether a call to show() will start sending data
                         immediately or will 'block' for a required interval. NeoPixels
                         require a short quiet time (about 300 microseconds) after the
                         last bit is received before the data 'latches' and new data can
                         start being received. Usually one's sketch is implicitly using
                         this time to generate a new frame of animation...but if it
                         finishes very quickly, this function could be used to see if
                         there's some idle time available for some low-priority
                         concurrent task.
        @return  1 or true if show() will start sending immediately, 0 or false
                         if show() would block (meaning some idle time is available).
    */
    bool              can_show(void) const { return ((uint16_t)micros()-u16_endTime) >= 300; }
    uint8_t           get_brightness(void) const;

    /*!
        @brief   Return the number of pixels in an Adafruit_NeoPixel strip object.
        @return  Pixel count (0 if not set).
    */
    uint8_t           get_length(void) const { return u8_numLEDs; }

    /*!
        @brief   An 8-bit integer sine wave function, not directly compatible
                         with standard trigonometric units like radians or degrees.
        @param   x  Input angle, 0-255; 256 would loop back to zero, completing
                                the circle (equivalent to 360 degrees or 2 pi radians).
                                One can therefore use an unsigned 8-bit variable and simply
                                add or subtract, allowing it to overflow/underflow and it
                                still does the expected contiguous thing.
        @return  Sine result, 0 to 255, or -128 to +127 if type-converted to
                         a signed int8_t, but you'll most likely want unsigned as this
                         output is often used for pixel brightness in animation effects.
    */
    static uint8_t    get_sine_8(uint8_t u8_x) {
        return pgm_read_byte(&_au8_NeoPixelSineTable[u8_x]); // 0-255 in, 0-255 out
    }
    /*!
        @brief   An 8-bit gamma-correction function for basic pixel brightness
                         adjustment. Makes color transitions appear more perceptially
                         correct.
        @param   x  Input brightness, 0 (minimum or off/black) to 255 (maximum).
        @return  Gamma-adjusted brightness, can then be passed to one of the
                         setPixelColor() functions. This uses a fixed gamma correction
                         exponent of 2.6, which seems reasonably okay for average
                         NeoPixels in average tasks. If you need finer control you'll
                         need to provide your own gamma-correction function instead.
    */
    static uint8_t    get_gamma_8(uint8_t u8_x) {
        return pgm_read_byte(&_au8_NeoPixelGammaTable[u8_x]); // 0-255 in, 0-255 out
    }
    /*!
        @brief   Convert separate red, green and blue values into a single
                         "packed" 32-bit RGB color.
        @param   r  Red brightness, 0 to 255.
        @param   g  Green brightness, 0 to 255.
        @param   b  Blue brightness, 0 to 255.
        @return  32-bit packed RGB value, which can then be assigned to a
                         variable for later use or passed to the setPixelColor()
                         function. Packed RGB format is predictable, regardless of
                         LED strand color order.
    */
    static uint32_t   rgb_to_pack(uint8_t u8_red, uint8_t u8_green, uint8_t u8_blue) {
        return ((uint32_t)u8_red << 16) | ((uint32_t)u8_green <<  8) | u8_blue;
    }
    // Default values must be given *only* in the function prototype
    static uint32_t   hsv_to_pack(uint16_t u16_hue, uint8_t u8_sat=255, uint8_t u8_val=255);
    static uint32_t   get_gamma_32(uint32_t u32_color);


    protected:

    bool              b_begun;       ///< true if begin() previously called
    uint8_t           u8_numLEDs;    ///< Current number of RGB LEDs in strip
    uint8_t           u8_maxLEDs;    ///< Max number of RGB LEDs (max size of buffer/3) 
    uint16_t          u16_numBytes;  ///< Curr size of 'pixels' buffer below
    uint8_t           u8_brightness; ///< Strip brightness 0-255 (stored as +1)
    uint8_t*          pu8_pixels;    ///< Holds LED color values (3 bytes each)
    uint16_t          u16_endTime;   ///< Latch timing reference

    // AVR Specific
    volatile uint8_t* pu8_port;      ///< Output PORT register address
    volatile uint8_t* pu8_ddr;       ///< Output PORT data direction register
    uint8_t           u8_pinMask;    ///< Output PORT bitmask
    


};

#endif // NEOPIXEL_SLIM_H