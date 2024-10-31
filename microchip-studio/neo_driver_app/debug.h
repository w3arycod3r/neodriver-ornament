/* File:      debug.h
 * Author:    Garrett Carter
 * Purpose:   Centralized debug defines to turn on/off debug code.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

/********************************** DEFINES **********************************/
// Define these to enable debug code
// #define DEBUG_COV_ANIM_EN
// #define DEBUG_LOW_BATT_EN
// #define DEBUG_BATT_LVL_EN
// #define DEBUG_ADC_VAL_EN
// #define DEBUG_RAND_SEED_EN
// #define DEBUG_BRIGHT_EN

// #define DEBUG_SOFT_RESET_ON_INTERVAL_EN
#define DEBUG_SOFT_RESET_INTERVAL_SEC   (2)


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* DEBUG_H */