#ifndef STM32F103_H
#define STM32F103_H

#include "types.h"
#include "interrupts.h"
#include "rcc.h"
#include "gpio.h"
#include "spi.h"
#include "i2c.h"
#include "usart.h"

#define GPIOABASE   0x40010800
#define GPIOBBASE   0x40010C00
#define GPIOCBASE   0x40011000

#define RCCBASE     0x40021000
#define PLL_LOCK    0x02000000
#define SYSTICKBASE 0xE000E010

#define EXTIBASE    0x40010400 
#define SCBBASE     0xE000E008 
#define NVICBASE    0xE000E100
#define AFIOBASE    0x40010000

#define SPI1BASE    0x40013000
#define SPI2BASE    0x40003800
#define SPI3BASE    0x40003C00

#define I2C1BASE    0x40005400
#define I2C2BASE    0x40005800

#define USART1BASE  0x40013800
#define USART2BASE  0x40004400

#define RCC     ((rcc_t *) RCCBASE)
#define SYSTICK ((systick_t *) SYSTICKBASE)

#define EXTI    ((exti_t *) EXTIBASE)
#define AFIO    ((afio_t *) AFIOBASE)

#define GPIOA ((gpio_t *) GPIOABASE)
#define GPIOB ((gpio_t *) GPIOBBASE)
#define GPIOC ((gpio_t *) GPIOCBASE)

#define SPI1 ((spi_t *) SPI1BASE)
#define SPI2 ((spi_t *) SPI2BASE)

#define I2C1 ((i2c_t *) I2C1BASE)
#define I2C2 ((i2c_t *) I2C2BASE)

#define USART1 ((usart_t *) USART1BASE)
#define USART2 ((usart_t *) USART2BASE)

#endif
