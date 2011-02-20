/**
 * Simple AFR gauge using an MSP430 and a 7-segment serial display.
 */

#include <msp430x20x2.h>
#include "config.h"

#define CLK_PIN_DELAY  ( 10 )  // Magical SPI fudge number

void spiBang (char byte);

static void __inline__ BusyWait(register unsigned int n) 
{ 
    __asm__ __volatile__ ( 
            "1: \n" 
            " dec      %[n] \n" 
            " jne      1b \n" 
            : [n] "+r"(n)); 
} 

/**
 * CPOL = 0, CPHA = 0
 */
void spiBang (char byte) {
    P1OUT |= CS_PIN | GREEN_LED;           
    short bit;
    for (bit = 0; bit < 8; bit++) {
        if (byte & 0x80) { 
            P1OUT |= MOSI_PIN; 
        }
        else {
            P1OUT &= ~MOSI_PIN; 
        }
        byte <<= 1;
        BusyWait (CLK_PIN_DELAY);
        P1OUT |= CLK_PIN;           // Slave latches on rising clock edge
        BusyWait (CLK_PIN_DELAY);  
        P1OUT &= ~CLK_PIN;        
    }   
    P1OUT &= ~(CS_PIN | GREEN_LED);
}

int main() {
    WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

    P1OUT = 0;
    P1DIR = ( RED_LED | GREEN_LED | CS_PIN | MOSI_PIN | CLK_PIN );


    while (TRUE) { 
        spiBang (0x01);
        spiBang (0x02);
        spiBang (0x03);
        spiBang (0x04);
        P1OUT ^= RED_LED;

        /* A long pause */
        long i = 30000;
        while (i != 0) {
            i--;
            BusyWait (CLK_PIN_DELAY);
        }
    }
    return 0;
}

