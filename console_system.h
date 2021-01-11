#ifndef CONSOLE_SYSTEM_H
#define CONSOLE_SYSTEM_H

#include "types.h"
#include "stm32f411cx.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "st7735.h"
#include "i2c.h"
#include "interrupts.h"
#include "keypad.h"
#include "timer.h"
#include "ili9341.h"


//screen routines

#define SCREEN_WIDTH ili9341_get_width()
#define SCREEN_HEIGHT ili9341_get_height()

inline void screen_init() {
    ili9341_init(SPI1, GPIOA);
}

inline void screen_reset() {
    ili9341_hwreset(GPIOA);
}

inline void screen_fill(uint16_t color) {
    ili9341_fill_screen(color, SPI1, GPIOA);
}

inline void screen_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    ili9341_set_addr_window(x0, y0, x1, y1, SPI1, GPIOA);
}

inline void screen_streampixel(uint16_t color) {
    ili9341_streampixel(color, SPI1);
}

inline void screen_putpixel(int16_t x, int16_t y, uint16_t color) {
    ili9341_draw_pixel(x,  y, color, SPI1, GPIOA);
}

inline void screen_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
    ili9341_fill_rect(x, y, w, h, color, SPI1, GPIOA);
}

inline void system_init() {
    rcc_init(RCC);
    systick_init(SYSTICK);

    /*Enable PORT A, PA9 = TX, PA10 = RX*/
    gpio_init(GPIOA, RCC, 9,  GPIO_CNF_ALT | GPIO_MODE_OUT_HIGH | GPIO_CNF_OUT_PUSH, GPIO_AF7_USART1_2);
    //gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
    gpio_init(GPIOA, RCC, 10,  GPIO_CNF_ALT | GPIO_PULL_NONE, GPIO_AF7_USART1_2);
    //RES
    gpio_init(GPIOA, RCC, 2, GPIO_CNF_GP_OUT | GPIO_CNF_OUT_PUSH | GPIO_MODE_OUT_HIGH, GPIO_AF0_SYS);
    //D/C
    gpio_init(GPIOA, RCC, 4, GPIO_CNF_GP_OUT | GPIO_CNF_OUT_PUSH | GPIO_MODE_OUT_HIGH, GPIO_AF0_SYS);
    //CE
    gpio_init(GPIOA, RCC, 3,  GPIO_CNF_GP_OUT | GPIO_CNF_OUT_PUSH | GPIO_MODE_OUT_HIGH, GPIO_AF0_SYS);
    //sound
    gpio_init(GPIOA, RCC, 0, GPIO_CNF_ALT | GPIO_PULL_NONE, GPIO_AF1_TIM1_2);
    //gpio_init(GPIOA, RCC, 0, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    gpio_init(GPIOB, RCC, 4, GPIO_CNF_IN | GPIO_PULL_NONE,  GPIO_AF0_SYS);

    gpio_init(GPIOB, RCC, 5, GPIO_CNF_IN | GPIO_PULL_NONE, GPIO_AF0_SYS );

    gpio_init(GPIOB, RCC, 3, GPIO_CNF_IN | GPIO_PULL_NONE, GPIO_AF0_SYS );

    gpio_init(GPIOC, RCC, 13, GPIO_CNF_GP_OUT | GPIO_CNF_OUT_PUSH | GPIO_MODE_OUT_HIGH, GPIO_AF0_SYS);
    //gpio_out(GPIOC, 13, 0);

    gpio_init(GPIOA, RCC, 5,   GPIO_CNF_GP_OUT | GPIO_CNF_OUT_PUSH | GPIO_MODE_OUT_HIGH, GPIO_AF0_SYS);
    //gpio_init(gpio_a, rcc, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //gpio_init(GPIOA, RCC, 6,  GPIO_MODE_INPUT | GPIO_CNF_IN_PULL);
    gpio_init(GPIOA, RCC, 6,  GPIO_CNF_IN | GPIO_PULL_NONE, GPIO_AF0_SYS);
    gpio_init(GPIOA, RCC, 7,  GPIO_CNF_GP_OUT | GPIO_CNF_OUT_PUSH | GPIO_MODE_OUT_HIGH, GPIO_AF0_SYS);

    gpio_init(GPIOB, RCC, 6,  GPIO_CNF_ALT | GPIO_PULL_NONE, GPIO_AF4_I2C1_3);              //I2C pins
    gpio_init(GPIOB, RCC, 7,  GPIO_CNF_ALT | GPIO_PULL_NONE, GPIO_AF4_I2C1_3);

    // Disable Reset
    RCC->apb2rstr &= ~(1 << 14);
    RCC->apb2rstr &= ~(1 << 12);

    //Enable USART1 clock
    RCC->apb2enr |= (1 << 4);
    //Enable SPI1 clock
    RCC->apb2enr |= (1 << 12);
    //rcc->apb2enr |= 0x4000;

    usart_init(USART1);

    spi_init(SPI1, SPI_MASTER, SPI_MODE_UNI2 | SPI_OUTPUT_ENABLE, 
            SPI_DFF_8BIT, SPI_CLK_DIV2, SPI_CPOL_0, SPI_CPHA_1, SPI_SLAVE_MGMT_DISABLE);


    GPIOB->odr &= ~(1 << 4); //without pullup for pb4
    //gpio_b->odr &= ~(1 << 3); //without pullup for pb3
    GPIOB->odr &= ~(1 << 5); //without pullup for pb5

    //Setting up interrupts
    //AFIO->exticr[0] = 1 << 12;   //PB3 as input
    //AFIO->exticr[1] = 1 << 0;   //PB4 as input
    //AFIO->exticr[1] |= 1 << 4;   //PB4 as input
    
    //EXTI->ftsr = (1 << 3); //falling edge
    EXTI->ftsr = (1 << 5); //falling edge
    EXTI->ftsr |= (1 << 4); //falling edge

    //EXTI->imr = (1 << 3);  // enable interrupt exti3
    EXTI->imr = (1 << 5);  // enable interrupt exti3
    EXTI->imr |= (1 << 4);  // enable interrupt exti4

    //nvic_enable_irq(EXTI3_IRQ);
    //nvic_enable_irq(EXTI9_5_IRQ);
    //nvic_enable_irq(EXTI4_IRQ);


    //need to do it after initializing interrupts, would freeze since the mcp23017 intrrupt fires immediately
    keypad_init(I2C1, RCC);

    screen_init();
    ili9341_set_rotation(3, SPI1, GPIOA);
    //st7735_set_rotation(0xA8, GPIOA, SPI1);            //MY=1, MX=0, MV=1, RGB=1 (for backtab RGB=0)
    screen_fill(Color565(0, 0, 0));

    //st7735_tearing_off(GPIOA, SPI1);

    //we start interrupts
    systick_interrupt_start(SYSTICK);
}

#endif

