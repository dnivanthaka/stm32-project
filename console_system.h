#ifndef CONSOLE_SYSTEM_H
#define CONSOLE_SYSTEM_H

#include "types.h"
#include "stmf103xxx.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "st7735.h"
#include "i2c.h"
#include "interrupts.h"
#include "keypad.h"

//delay routines
#define _delay_ms(y) delay_ms(SYSTICK, y)

//screen routines
inline void screen_init() {
    st7735_init(GPIOA, SPI1, SYSTICK);
}

inline void screen_reset() {
    st7735_hwreset(GPIOA, SYSTICK);
}

inline void screen_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

    st7735_set_addr_window(x0, y0, x1, y1, GPIOA, SPI1);
}

#endif

