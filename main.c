/**
 * Simple AFR gauge using an MSP430 and a 7-segment serial display.
 *
 * John Howe
 */

#include <msp430x20x2.h>
#include <signal.h>
#include "config.h"

#define CLK_PIN_DELAY  ( 1 )  // SPI bitbang half clock period

#define true 1
#define false 0

#define ADCPIN INCH_7

void spiBang (unsigned char byte);
short analogRead(unsigned short);

/*
 *  Busy waits a specified number of clock cycles.
 */
static void __inline__ BusyWait(register unsigned short n) 
{ 
        __asm__ __volatile__ ( 
                "1: \n" 
                " dec      %[n] \n" 
                " jne      1b \n" 
                : [n] "+r"(n)); 
} 

/*
 * Bit-bangs a byte over SPI
 *
 * CPOL = 0, CPHA = 0
 */
void spiBang (unsigned char byte) 
{
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
                //BusyWait (CLK_PIN_DELAY);
                P1OUT |= CLK_PIN;           // Slave latches on rising clock edge
                //BusyWait (CLK_PIN_DELAY);  
                P1OUT &= ~CLK_PIN;        
        }   
        P1OUT &= ~(GREEN_LED);
        P1OUT |= CS_PIN;
}

/*
 * Displays a number on the serial 7-seg display
 *
 * The number to be displayed is separated out into individual digits to each
 * be sent to the display. Any decimal point needs to be managed elsewhere.
 */
void display(unsigned short number) 
{
        char first = 0, second = 0, third = 0, fourth = 0;
        while (number > 0) {
                if (number > 9999) {
                        number -= 10000;
                } else if (number > 999) {
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
        spiBang (first);
        spiBang (second);
        spiBang (third);
        spiBang (fourth);
}

short analogRead(unsigned short pin) 
{
        ADC10CTL0 = ADC10ON + ADC10SHT_1 + SREF_0; // ACD10 on, 8 clock cycles per sample, Use Vcc/Vss references
        ADC10CTL1 = ADC10SSEL_0 + pin; // Select internal ADC clock (~5MHz) and Input channel (pin)
        ADC10CTL0 |= ENC + ADC10SC; // enable and start conversion
        while (!((ADC10CTL1 ^ ADC10BUSY) & ((ADC10CTL0 & ADC10IFG)==ADC10IFG))) { // ensure conversion is complete
                continue;
        }
        ADC10CTL0 &= ~(ADC10IFG +ENC); // disable conversion and clear flag

        short adcReading = ADC10MEM;
        return adcReading;
}

/*
 * Takes an analogue reading from the O2 controller and displays on the 7seg
 * display, possibly after some smoothing.
 */
int main() 
{
        WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

        P1OUT = 0;
        P1DIR |= ( RED_LED | GREEN_LED | CS_PIN | MOSI_PIN | CLK_PIN ); // Set output pins
        P1DIR &= ~( ADCPIN ); // Set input pins
        ADC10AE0 |= ADCPIN; // Enable ADC

        /* Reset and clear the display of any decomal points */
        spiBang (RESET);
        spiBang (DECIMAL);
        spiBang (4);

        for (;;) {
                BusyWait (38782);   // FIXME 
                static int i = 0;
                display (i++);
                
                //display (analogRead(ADCPIN)); 
        }
        return 0;
}

