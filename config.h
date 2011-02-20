#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <msp430x20x2.h>

#define RED_LED         BIT0
#define GREEN_LED       BIT6

/* SPI pins */
#define CS_PIN          BIT3        // Chip select
#define MOSI_PIN        BIT4        // Master out, slave in
#define CLK_PIN         BIT5        // Clock

/* LCD Commands */
#define DECIMAL         (0x77)
#define BRIGHTNESS      (0x7a)
#define RESET           (0x76)
#define DIGIT1          (0x7b)
#define DIGIT2          (0x7c)
#define DIGIT3          (0x7d)
#define DIGIT4          (0x7e)

/* Decimal points */
#define DECIMAL0        (1<<0)
#define DECIMAL1        (1<<1)
#define DECIMAL2        (1<<2)
#define DECIMAL3        (1<<3)
#define COLON           (1<<4)
#define APOSTROPHE      (1<<5)

#endif
