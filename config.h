#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <msp430x20x2.h>

#define TRUE            1
#define FALSE           0

#define RED_LED         BIT0
#define GREEN_LED       BIT6

#define TICK_HZ     65000  // Delay loops (ticks) per second TODO calibrate

/* SPI attributes */
#define CS_PIN          BIT3        // Chip select
#define MOSI_PIN        BIT4        // Master out, slave in
#define CLK_PIN         BIT5        // Clock
#define BAUD            ( 9600 )    // Baud rate for SPI


#endif
