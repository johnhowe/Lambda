/**
 * Simple AFR gauge using an MSP430 and a 7-segment serial display.
 */

#include <msp430x20x2.h>
#include <signal.h>
#include "config.h"

#define CLK_PIN_DELAY  ( 1 )  // Magical SPI fudge number

#define true 1
#define false 0

#define CHAN INCH_6

void spiBang (char byte);

int ADCDone; 
unsigned int ADCValue;

/*
 *  Busy waits a specified number of clock cycles.
 */
static void __inline__ BusyWait(register unsigned int n) 
{ 
        __asm__ __volatile__ ( 
                        "1: \n" 
                        " dec      %[n] \n" 
                        " jne      1b \n" 
                        : [n] "+r"(n)); 
} 

/*
 * Transmits a byte over the bit-bang SPI
 *
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

/*
 * Displays a number on the serial 7-seg display
 *
 * The number to be displayed is separated out into individual digits to each
 * be sent to the display.
 */
void display (unsigned short number) {
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


/*
 * Starts an ADC reading
 */
void readADC () {
        ADC10CTL0 &= ~ENC; // Disable ADC
        ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // 16 clock ticks, ADC On, enable ADC interrupt
        ADC10CTL1 = ADC10SSEL_3 + CHAN; // Set 'chan', SMCLK
        ADC10CTL0 |= ENC + ADC10SC; // Enable and start conversion
}

/*
 * ADC interrupt routine. Pulls CPU out of sleep mode for the main loop.
 */
interrupt (ADC10_VECTOR) ADC10_ISR (void) //msp430-gcc in linux

{
        //ADCValue = ADC10MEM;			// Saves measured value.
        //ADCDone = true;  			// Sets flag for main loop.
        //__bic_SR_register_on_exit(CPUOFF);	// Enable CPU so the main while loop continues
}


/*
 * Takes an analogue reading from the O2 controller and displays on the 7seg
 * display, possibly after some smoothing.
 *
 */
int main() {
        /* Init */
        WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

        P1OUT = 0;
        P1DIR = ( RED_LED | GREEN_LED | CS_PIN | MOSI_PIN | CLK_PIN );

        /* Reset and clear the display of any decomal points */
        spiBang (RESET);
        spiBang (DECIMAL);
        spiBang (0);

        //IE1 |= WDTIE; //enable interrupt
        //_BIS_SR(LPM0_bits + GIE); //not low power mode and enable interrupts
        readADC();

        for (;;) {
                BusyWait (1000); 
               
                //if(ADCDone) {
                //ADCDone = false;
                ADCValue = ADC10MEM;
                display (ADCValue); 
                readADC();
                //}
                //if (~(ADCDone))
                //__bis_SR_register(CPUOFF + GIE);// LPM0, the ADC interrupt will wake the processor up.
        }
        return 0;
}

