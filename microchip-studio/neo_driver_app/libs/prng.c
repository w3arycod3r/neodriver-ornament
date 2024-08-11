/* File:      prng.c
 * Author:    Garrett Carter
 * Purpose:   Simple, code-space efficient 32-bit PRNG
 */

#include <prng.h>
#include <stdint.h>
#include <Arduino.h>

static uint32_t state = 0xDEADBEEF;

void prng_seed(uint32_t seed)
{
    // A zero seed would lock up the LFSR
    if (seed != 0)
    {
        state = seed;
    }
}

// Galois LFSR. See https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Galois_LFSRs for more info.
// Right shift version was 2 bytes smaller :)
uint32_t prng_next()
{
    // Local var to optimize into registers
    uint32_t lfsr = state;

    // Get LSB (i.e., the output bit).
    uint8_t lsb = lfsr & 1u;
    // Shift register
    lfsr >>= 1;
    // If the output bit is 1,
    if (lsb)
    {
        // apply toggle mask.
        lfsr ^= LFSR_TOGGLE_MASK_32BITS_RIGHT;
    }

    state = lfsr;
    return lfsr;
}

// Range [0, upper - 1]
uint32_t prng_upper(uint32_t upper)
{
    if (upper == 0)
    {
        return 0;
    }
    return prng_next() % upper;
}

// Range [lower, upper - 1]
uint32_t prng_range(uint32_t lower, uint32_t upper)
{
    if (lower >= upper)
    {
        return lower;
    }
    uint32_t diff = upper - lower;
    return prng_upper(diff) + lower;
}

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
