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

#define KEYPAD_ADDR 0x20

#define KEYPAD_I2C I2C1
#define KEYPAD_RCC RCC

void mcp23x17_write(i2c_t *i2c, uint8_t reg, uint8_t data){

    _disable_irq();

    do{
      i2c_wait_for_ready(i2c);
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_write(i2c, KEYPAD_ADDR) != 0);

    i2c_write_data(i2c, reg);
    i2c_write_data(i2c, data);
    i2c_send_stop(i2c);

    _enable_irq();
}

uint8_t mcp23x17_read(i2c_t *i2c, uint8_t reg){
    uint8_t ret = 0;

    _disable_irq();

    do{
      i2c_wait_for_ready(i2c);
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_write(i2c, KEYPAD_ADDR) != 0);

    i2c_write_data(i2c, reg);

    do{
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_read(i2c, KEYPAD_ADDR) != 0);

    ret = i2c_read_data(i2c, 0);
    i2c_send_stop(i2c);

    _enable_irq();

    return ret;
}

void keypad_init(){
    i2c_init(KEYPAD_I2C, KEYPAD_RCC);


    //reset iocon to default values
    mcp23x17_write(KEYPAD_I2C, IOCON_0, 0x00);

    //set ports as inputs
    mcp23x17_write(KEYPAD_I2C, IODIRA,   0xff);
    mcp23x17_write(KEYPAD_I2C, IODIRB_0, 0xff);

    //enable pullups
    mcp23x17_write(KEYPAD_I2C, GPPUA_0, 0xff);
    mcp23x17_write(KEYPAD_I2C, GPPUB_0, 0xff);

    //enable interrupts, defval
    mcp23x17_write(KEYPAD_I2C, DEFVALA_0, 0xff);
    mcp23x17_write(KEYPAD_I2C, DEFVALB_0, 0xff);
    // IoC setup
    //mcp23x17_write(KEYPAD_I2C, INTFA_0, 0);
    //mcp23x17_write(KEYPAD_I2C, INTFB_0, 0);
    //trigger IoC on previous value
    mcp23x17_write(KEYPAD_I2C, INTCONA_0, 0);
    mcp23x17_write(KEYPAD_I2C, INTCONB_0, 0);

    mcp23x17_write(KEYPAD_I2C, GPINTENA_0, 0xff);
    mcp23x17_write(KEYPAD_I2C, GPINTENB_0, 0xff);

}

uint16_t keypad_read(){
    uint8_t gpioa = mcp23x17_read(KEYPAD_I2C, 0x12);
    uint8_t gpiob = mcp23x17_read(KEYPAD_I2C, 0x13);

    return ((gpiob << 8) | gpioa);
}
