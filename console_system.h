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

#define SCREEN_WIDTH  ST7735_TFTWIDTH
#define SCREEN_HEIGHT ST7735_TFTHEIGHT

inline void screen_init() {
    st7735_init(GPIOA, SPI1, SYSTICK);
}

inline void screen_reset() {
    st7735_hwreset(GPIOA, SYSTICK);
}

inline void screen_fill(uint16_t color) {
    st7735_fill_screen(color, GPIOA, SPI1);
}

inline void screen_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    st7735_set_addr_window(x0, y0, x1, y1, GPIOA, SPI1);
}

inline void screen_streampixel(uint16_t color) {
    st7735_streampixel(color, SPI1);
}

inline void screen_putpixel(int16_t x, int16_t y, uint16_t color) {
    st7735_draw_pixel(x,  y, color, GPIOA, SPI1);
}

inline void screen_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
    st7735_fill_rect(x, y, w, h, color, GPIOA, SPI1);
}

inline void system_init() {
    rcc_init(RCC);
    systick_init(SYSTICK);

    /*Enable PORT A, PA9 = TX, PA10 = RX*/
    gpio_init(GPIOA, RCC, 9,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
    gpio_init(GPIOA, RCC, 10,  GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT);
    //RES
    gpio_init(GPIOA, RCC, 2, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //D/C
    gpio_init(GPIOA, RCC, 4, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //CE
    gpio_init(GPIOA, RCC, 3, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    gpio_init(GPIOB, RCC, 4, GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT );

    gpio_init(GPIOB, RCC, 5, GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT );

    gpio_init(GPIOB, RCC, 3, GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT );

    gpio_init(GPIOC, RCC, 13, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    gpio_init(GPIOA, RCC, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    gpio_init(GPIOA, RCC, 6,  GPIO_MODE_INPUT | GPIO_CNF_IN_PULL);
    gpio_init(GPIOA, RCC, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);

    gpio_init(GPIOB, RCC, 6,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);              //I2C pins
    gpio_init(GPIOB, RCC, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);

    // Disable Reset
    RCC->apb2rstr &= ~(1 << 14);
    RCC->apb2rstr &= ~(1 << 12);

    //Enable USART1 clock
    RCC->apb2enr |= (1 << 14) | 1;
    //Enable SPI1 clock
    RCC->apb2enr |= (1 << 12) | 1;
    //rcc->apb2enr |= 0x4000;

    usart_init(USART1);

    spi_init(SPI1, SPI_MASTER, SPI_MODE_UNI2 | SPI_OUTPUT_ENABLE, 
            SPI_DFF_8BIT, SPI_CLK_DIV2, SPI_CPOL_0, SPI_CPHA_1, SPI_SLAVE_MGMT_DISABLE);

    keypad_init(I2C1, RCC);

    GPIOB->odr &= ~(1 << 4); //without pullup for pb4
    //gpio_b->odr &= ~(1 << 3); //without pullup for pb3
    GPIOB->odr &= ~(1 << 5); //without pullup for pb5

    //Setting up interrupts
    //AFIO->exticr[0] = 1 << 12;   //PB3 as input
    AFIO->exticr[1] = 1 << 0;   //PB4 as input
    AFIO->exticr[1] |= 1 << 4;   //PB4 as input
    
    //EXTI->ftsr = (1 << 3); //falling edge
    EXTI->ftsr = (1 << 5); //falling edge
    EXTI->ftsr |= (1 << 4); //falling edge

    //EXTI->imr = (1 << 3);  // enable interrupt exti3
    EXTI->imr = (1 << 5);  // enable interrupt exti3
    EXTI->imr |= (1 << 4);  // enable interrupt exti4

    //nvic_enable_irq(EXTI3_IRQ);
    nvic_enable_irq(EXTI9_5_IRQ);
    //nvic_enable_irq(EXTI4_IRQ);

    screen_init();
    screen_fill(Color565(0, 0, 0));
}

#endif

