/**
 * Simple AFR gauge using an MSP430 and a 7-segment serial display.
 */

#include <msp430x20x2.h>
#include "config.h"

// A delay of 100000 is roughly one second.
void delay (unsigned int time){
    while (time > 0) {
        time--;
    }
}

int main() {
    WDTCTL = WDTPW + WDTHOLD;
    P1OUT = RED_LED;
    P1DIR |= RX_PIN | RED_LED | GREEN_LED;

    while (1) {
        delay (60000);
        P1OUT ^= GREEN_LED | RED_LED;
    }
    return 0;
}
