#include "types.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "st7735.h"
#include "i2c.h"
#include "interrupts.h"
#include "keypad.h"


#define KEYPAD_ADDR 0x20

void mcp23x17_write(i2c_t *i2c, uint8_t reg, uint8_t data){

    do{
      i2c_wait_for_ready(i2c);
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_write(i2c, KEYPAD_ADDR) != 0);

    i2c_write_data(i2c, reg);
    i2c_write_data(i2c, data);
    i2c_send_stop(i2c);
}

uint8_t mcp23x17_read(i2c_t *i2c, uint8_t reg){
    uint8_t ret = 0;

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

    return ret;
}

void keypad_init(i2c_t *i2c, rcc_t *rcc){
    i2c_init(i2c, rcc);

    mcp23x17_write(i2c, 0x00, 0xff);
    mcp23x17_write(i2c, 0x01, 0xff);

    //enable pullups
    mcp23x17_write(i2c, 0x0C, 0xff);
    mcp23x17_write(i2c, 0x0D, 0xff);

    //enable interrupts
    mcp23x17_write(i2c, 0x06, 0xff);
    mcp23x17_write(i2c, 0x07, 0xff);
    // IoC setup
    mcp23x17_write(i2c, 0x0e, 0);
    mcp23x17_write(i2c, 0x0f, 0);

    mcp23x17_write(i2c, 0x08, 0xff);
    mcp23x17_write(i2c, 0x09, 0xff);

    mcp23x17_write(i2c, 0x04, 0xff);
    mcp23x17_write(i2c, 0x05, 0xff);
}

uint16_t keypad_read(i2c_t *i2c){
    uint8_t gpioa = mcp23x17_read(i2c, 0x12);
    uint8_t gpiob = mcp23x17_read(i2c, 0x13);

    return ((gpiob << 8) | gpioa);
}
