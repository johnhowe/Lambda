/**
 * Simple AFR gauge using an MSP430 and a 7-segment serial display.
 */

#include <msp430x20x2.h>
#include "config.h"

#define CLK_PIN_DELAY  1000 //( TICK_HZ / BAUD )

void Delay (unsigned long ticks);
void spiBang (unsigned char data);

// A delay of 100000 is very roughly one second.
void Delay (unsigned long ticks){
    while (ticks > 0) {
        ticks--;
    }
}

/**
 * Currently only supporting CPOL = 0, CPHA = 0
 */
void spiBang (unsigned char data) {
    BitSet (P1OUT, CS_PIN);             // Enable chip
    static short bit;
    for (bit = 0; bit < 8; bit++) {
        if (BitTst(data, bit)) {
            BitSet (P1OUT, MOSI_PIN);   // Set MOSI_PIN high
            BitSet (P1OUT, GREEN_LED);// Enable TX LED
        }
        else {
            BitSet (P1OUT, MOSI_PIN);   // Set MOSI_PIN low
            BitClr (P1OUT, GREEN_LED);// Disable TX LED
        }
        BitSet (P1OUT, CLK_PIN);        // One clock cycle
        Delay (CLK_PIN_DELAY);
        BitClr (P1OUT, CLK_PIN);
        Delay (CLK_PIN_DELAY);
    }   
    BitClr (P1OUT, CS_PIN);             // Disable chip
}

int main() {
    WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog ticksr

    BitSet (P1DIR, RED_LED);
    BitSet (P1DIR, GREEN_LED);      // Enable LED outputs
    BitSet (P1DIR, CS_PIN);
    BitSet (P1DIR, MOSI_PIN);
    BitSet (P1DIR, CLK_PIN);            // Enable SPI output pins


    while (TRUE) {

        spiBang ('1');
        //        spiBang ('2');
        //        spiBang ('3');
        //        spiBang ('4');
        //
        //        Delay (60000);
        //
        //        spiBang ('4');
        //        spiBang ('3');
        //        spiBang ('2');
        //        spiBang ('1');

        Delay (60000);
        BitFlp (P1OUT, RED_LED);
    }
    return 0;
}


