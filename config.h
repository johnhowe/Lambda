#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <msp430x20x2.h>

#define RED_LED         BIT0
#define GREEN_LED       BIT6

/* SPI pins */
#define CS_PIN          BIT3        // Chip select
#define MOSI_PIN        BIT4        // Master out, slave in
#define CLK_PIN         BIT5        // Clock

#endif
