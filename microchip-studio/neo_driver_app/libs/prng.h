/* File:      prng.h
 * Author:    Garrett Carter
 * Purpose:   Simple, code-space efficient 32-bit PRNG
 */

#ifndef PRNG_H
#define PRNG_H

#include <stdbool.h>
#include <utility.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************** DEFINES ******************************/
// Note: Taps are one-indexed while bits in the toggle mask are zero-indexed
// Subtract one to convert tap to bit
// Left shift masks are bit-reversed versions of the normal right shift ones 

// Reference: https://docs.amd.com/v/u/en-US/xapp052
// Taps: 32, 22, 2, 1
#define LFSR_TOGGLE_MASK_32BITS_RIGHT (BIT31 | BIT21 | BIT1 | BIT0)
// Taps: 32, 31, 11, 1
#define LFSR_TOGGLE_MASK_32BITS_LEFT  (BIT31 | BIT30 | BIT10 | BIT0)

/****************************** PROTOTYPES ******************************/
void prng_seed(uint32_t);
uint32_t prng_next();
uint32_t prng_upper(uint32_t);
uint32_t prng_range(uint32_t, uint32_t);
uint32_t map(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* PRNG_H */
