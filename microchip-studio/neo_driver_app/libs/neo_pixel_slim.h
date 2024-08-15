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

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

/****************************** DEFINES ******************************/
#define NP_PIXEL_COUNT  (25)
#define NP_PIN          (BIT3)                          // PB3, ADC3 - NeoPixel Data

/*
 * Internal defines
 */
#if !defined(CONCAT)
#define CONCAT(a, b)            a ## b
#endif

#if !defined(CONCAT_EXP)
#define CONCAT_EXP(a, b)   CONCAT(a, b)
#endif

// Timing in ns
#define w_zeropulse   350
#define w_onepulse    900
#define w_totalperiod 1250

// Fixed cycles used by the inner loop
// AVR core
#define w_fixedlow    3
#define w_fixedhigh   6
#define w_fixedtotal  10


// Insert NOPs to match the timing, if possible
#define w_zerocycles    (((F_CPU/1000)*w_zeropulse          )/1000000)
#define w_onecycles     (((F_CPU/1000)*w_onepulse    +500000)/1000000)
#define w_totalcycles   (((F_CPU/1000)*w_totalperiod +500000)/1000000)

// w1 - nops between rising edge and falling edge - low
#define w1 (w_zerocycles-w_fixedlow)
// w2   nops between fe low and fe high
#define w2 (w_onecycles-w_fixedhigh-w1)
// w3   nops to complete loop
#define w3 (w_totalcycles-w_fixedtotal-w1-w2)

#if w1>0
  #define w1_nops w1
#else
  #define w1_nops  0
#endif

// The only critical timing parameter is the minimum pulse length of the "0"
// Warn or throw error if this timing can not be met with current F_CPU settings.
#define w_lowtime ((w1_nops+w_fixedlow)*1000000)/(F_CPU/1000)
#if w_lowtime>550
   #error "Light_ws2812: Sorry, the clock speed is too low. Did you set F_CPU correctly?"
#elif w_lowtime>450
   #warning "Light_ws2812: The timing is critical and may only work on WS2812B, not on WS2812(S)."
   #warning "Please consider a higher clockspeed, if possible"
#endif   

#if w2>0
#define w2_nops w2
#else
#define w2_nops  0
#endif

#if w3>0
#define w3_nops w3
#else
#define w3_nops  0
#endif

#define w_nop1  "nop      \n\t"
#define w_nop2  "brid .+0 \n\t"
#define w_nop4  w_nop2 w_nop2
#define w_nop8  w_nop4 w_nop4
#define w_nop16 w_nop8 w_nop8

/*! 
        @brief  Class that stores state and functions for interacting with
                        Adafruit NeoPixels and compatible devices.
*/
void np_init(void);
void np_show(void);
void np_set_pix_color(uint8_t u8_index, uint8_t u8_red, uint8_t u8_green, uint8_t u8_blue);
void np_set_pix_color_pack(uint8_t u8_index, uint32_t u32_color);
void np_fill(uint32_t u32_color, uint8_t u8_firstIndex, uint8_t u8_count);
// Many applications will fill the entire strip
void np_fill_all(uint32_t u32_color);
void np_set_brightness(uint8_t u8_brightness);
void np_clear(void);

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
bool              np_can_show(void);
uint8_t           np_get_brightness(void);

/*!
    @brief   Return the number of pixels in an Adafruit_NeoPixel strip object.
    @return  Pixel count (0 if not set).
*/
uint8_t           np_get_length(void);

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
uint8_t    np_get_sine_8(uint8_t u8_x);
/*!
    @brief   An 8-bit gamma-correction function for basic pixel brightness
                        adjustment. Makes color transitions appear more perceptually
                        correct.
    @param   x  Input brightness, 0 (minimum or off/black) to 255 (maximum).
    @return  Gamma-adjusted brightness, can then be passed to one of the
                        setPixelColor() functions. This uses a fixed gamma correction
                        exponent of 2.6, which seems reasonably okay for average
                        NeoPixels in average tasks. If you need finer control you'll
                        need to provide your own gamma-correction function instead.
*/
uint8_t    np_get_gamma_8(uint8_t u8_x);
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
uint32_t   np_rgb_to_pack(uint8_t u8_red, uint8_t u8_green, uint8_t u8_blue);

// Many applications will only vary the hue
uint32_t   np_hsv_to_pack_hue(uint16_t u16_hue);
uint32_t   np_hsv_to_pack(uint16_t u16_hue, uint8_t u8_sat, uint8_t u8_val);

uint32_t   np_get_gamma_32(uint32_t u32_color);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEOPIXEL_SLIM_H