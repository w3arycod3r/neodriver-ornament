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

/****************************** DEFINES ******************************/

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
                         Static function, so it is independent of an object of this class.
        @param   x  Input angle, 0-255; 256 would loop back to zero, completing
                                the circle (equivalent to 360 degrees or 2 pi radians).
                                One can therefore use an unsigned 8-bit variable and simply
                                add or subtract, allowing it to overflow/underflow and it
                                still does the expected contiguous thing.
        @return  Sine result, 0 to 255, or -128 to +127 if type-converted to
                         a signed int8_t, but you'll most likely want unsigned as this
                         output is often used for pixel brightness in animation effects.
    */
    static uint8_t    get_sine_8(uint8_t u8_x);
    /*!
        @brief   An 8-bit gamma-correction function for basic pixel brightness
                         adjustment. Makes color transitions appear more perceptually
                         correct.
                         Static function, so it is independent of an object of this class.
        @param   x  Input brightness, 0 (minimum or off/black) to 255 (maximum).
        @return  Gamma-adjusted brightness, can then be passed to one of the
                         setPixelColor() functions. This uses a fixed gamma correction
                         exponent of 2.6, which seems reasonably okay for average
                         NeoPixels in average tasks. If you need finer control you'll
                         need to provide your own gamma-correction function instead.
    */
    static uint8_t    get_gamma_8(uint8_t u8_x);
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