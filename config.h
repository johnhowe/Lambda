#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <msp430x20x2.h>

#define TRUE        1
#define FALSE       0

#define RED_LED     0
#define GREEN_LED   6

#define TICK_HZ     100000  // Delay loops (ticks) per second TODO calibrate

/* SPI attributes */
#define CS_PIN          3       // Chip select
#define MOSI_PIN        4       // Master out, slave in
#define CLK_PIN         5       // Clock
#define BAUD        10    // Baud rate for SPI

/* Bit twiddling */
#define BitSet(arg,pos) ((arg) |= (1 << (pos)))
#define BitClr(arg,pos) ((arg) &= ~(1 << (pos)))
#define BitFlp(arg,pos) ((arg) ^= (1 << (pos)))
#define BitTst(arg,pos) ((arg) & (1 << (pos)))

#endif
