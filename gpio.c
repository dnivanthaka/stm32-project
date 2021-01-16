#include "types.h"
#include "stm32f411cx.h"
#include "rcc.h"
#include "gpio.h"


void gpio_init(gpio_t *gpio, rcc_t *rcc, uint8_t pin, uint8_t mode, uint8_t afmode) {
 uint8_t i = 0;
 //_disable_irq();

 if(gpio == (struct gpio_t *)GPIOABASE){
  rcc->ahb1enr |= GPIO_IOPAEN;
 }else if(gpio == (struct gpio_t *)GPIOBBASE){
  rcc->ahb1enr |= GPIO_IOPBEN;
 }else if(gpio == (struct gpio_t *)GPIOCBASE){
  rcc->ahb1enr |= GPIO_IOPCEN;
 }

 uint8_t shift = pin * 2;
 //reset mode
 gpio->moder &= ~(3 << shift);
 //set mode
 gpio->moder |= ((mode & 0x03) << shift);
 if(mode & 0x03){
    gpio->otyper &= ~(1 << pin);
    gpio->otyper |= (((mode & 0x04) >> 2) << pin);

    gpio->ospeedr &= ~(3 << shift); 
    gpio->ospeedr |= (((mode & 0x30) >> 4) << shift);
   if(mode & 0x02){
        //ALT
        if(pin < 8){
            shift = pin * 4;
            gpio->afrl &= ~(0xf << shift);
            gpio->afrl |= (afmode << shift);
        }else{
            shift = (pin - 8) * 4;
            gpio->afrh &= ~(0xf << shift);
            gpio->afrh |= (afmode << shift);
        }
   } 
 }else{
     //input
   //pullup
   gpio->pupdr &= ~(3 << shift);
   gpio->pupdr |= (((mode & 0xc0) >> 6) << shift);
 }
 //_enable_irq();
}

void gpio_out(gpio_t *gpio, uint8_t pin, uint8_t value){
 uint8_t reg_val = 0;
 //_disable_irq();

 if(value == 0) reg_val = 16;
 gpio->bsrr = 1 << (pin + reg_val);
 //_enable_irq();
}

uint8_t gpio_in(gpio_t *gpio, uint8_t pin){
    return (gpio->idr & (1 << pin))?1:0;
}
