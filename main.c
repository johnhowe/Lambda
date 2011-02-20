/**
 * Simple AFR gauge using an MSP430 and a 7-segment serial display.
 */

#include <msp430x20x2.h>
#include "config.h"

#define CLK_PIN_DELAY  ( 1 )  // Magical SPI fudge number

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
    P1OUT |= GREEN_LED;           
    P1OUT &= ~CS_PIN;
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
    P1OUT &= ~(GREEN_LED);
    P1OUT |= CS_PIN;
}

void display (unsigned short number) {
    char first = 0, second = 0, third = 0, fourth = 0;
    while (number > 0) {
        if (number > 9999) {
            number -= 10000;
        }
        if (number > 999) {
            first++;
            number -= 1000;
        } else if (number > 99) {
            second++;
            number -= 100;
        } else if (number > 9) {
            third++;
            number -= 10;
        } else {
            fourth++;
            number -= 1;
        }
    }
    spiBang (RESET);
    spiBang (DECIMAL);
    spiBang (DECIMAL1);

    spiBang (first);
    spiBang (second);
    spiBang (third);
    spiBang (fourth);
}

int main() {
    WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

    P1OUT = 0;
    P1DIR = ( RED_LED | GREEN_LED | CS_PIN | MOSI_PIN | CLK_PIN );


    static short count = 8888;
    for (;;) {
        display (count++);

        long i = 1000;
        while (i != 0) {
            i--;
            BusyWait (CLK_PIN_DELAY);
        }
    }
    return 0;
}

