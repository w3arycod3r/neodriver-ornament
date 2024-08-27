#include <Arduino.h>
#include <stdint.h>
#include <avr/wdt.h>

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

// Need linker flag/option "-Wl,-u,get_mcusr" added when link-time optimization (LTO) is on (Release config).
// This prevents the linker from discarding the function get_mcusr.
// https://www.avrfreaks.net/s/topic/a5C3l000000Uc88EAC/t160364
void get_mcusr(void) \
    __attribute__((naked)) \
    __attribute__((section(".init3")));

/*  https://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_softreset
    Note that for newer devices (ATmega88 and newer, effectively any AVR that has the option to also generate interrupts), 
    the watchdog timer remains active even after a system reset (except a power-on condition), 
    using the fastest prescaler value (approximately 15 ms). 
    It is therefore required to turn off the watchdog early during program startup, the datasheet recommends a sequence like the following: */
void get_mcusr(void)
{
    mcusr_mirror = MCUSR;
    // Clears WDRF in MCUSR
    MCUSR = 0;
    // Clears WDE in WDTCR
    wdt_disable();
}

int main(void)
{
    init();

    setup();

    for (;;)
    {
        loop();
    }

    return 0;
}
