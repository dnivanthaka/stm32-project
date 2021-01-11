#ifndef GPIO_H
#define GPIO_H

#define GPIO_CNF_IN          0x00
#define GPIO_CNF_GP_OUT      0x01
#define GPIO_CNF_ALT         0x02
#define GPIO_CNF_ANALOG      0x03

#define GPIO_CNF_OUT_PUSH   (0x0 << 2)
#define GPIO_CNF_OUT_OPEN   (0x1 << 2)

#define GPIO_MODE_OUT_LOW   (0x0 << 4)
#define GPIO_MODE_OUT_MED   (0x1 << 4)
#define GPIO_MODE_OUT_FAST  (0x2 << 4)
#define GPIO_MODE_OUT_HIGH  (0x3 << 4)

#define GPIO_PULL_NONE      (0x0 << 6)
#define GPIO_PULL_UP        (0x1 << 6)
#define GPIO_PULL_DOWN      (0x2 << 6)
#define GPIO_PULL_RESERVED  (0x3 << 6)

#define GPIO_IOPAEN           1
#define GPIO_IOPBEN           1 << 1
#define GPIO_IOPCEN           1 << 2

#define GPIO_AF0_SYS          0
#define GPIO_AF1_TIM1_2       1
#define GPIO_AF2_TIM3_5       2 
#define GPIO_AF3_TIM9_11      3 
#define GPIO_AF4_I2C1_3       4
#define GPIO_AF5_SPI1_4       5
#define GPIO_AF6_SPI3_5       6
#define GPIO_AF7_USART1_2     7
#define GPIO_AF8_USART6       8
#define GPIO_AF9_I2C2_3       9
#define GPIO_AF10_OTG_FS      10
#define GPIO_AF11             11
#define GPIO_AF12_SDIO        12
#define GPIO_AF13             13
#define GPIO_AF14             14
#define GPIO_AF15_EVENT       15


typedef struct gpio_t {
    volatile uint32_t moder;
    volatile uint32_t otyper;
    volatile uint32_t ospeedr;
    volatile uint32_t pupdr;
    volatile uint32_t idr;
    volatile uint32_t odr;
    volatile uint32_t bsrr;
    volatile uint32_t lckr;
    volatile uint32_t afrl;
    volatile uint32_t afrh;
} gpio_t;

void gpio_init(gpio_t *gpio, rcc_t *rcc, uint8_t pin, uint8_t mode, uint8_t afmode);
void gpio_out(gpio_t *gpio, uint8_t pin, uint8_t value);
uint8_t gpio_in(gpio_t *gpio, uint8_t pin);

#endif
