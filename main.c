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
#include "console_system.h"


void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

uint8_t ledVal = 0;
volatile uint16_t keypadkeys = 0xffff;


void exti3_irq_handler(){
    //keypadkeys = keypad_read();

    EXTI->pr = (1 << 3); //Clearing the pending flag
}

void exti4_irq_handler(){
    keypadkeys = keypad_read();

    EXTI->pr = (1 << 4); //Clearing the pending flag
}

void exti9_5_irq_handler(){
    keypadkeys = keypad_read();

    EXTI->pr = (1 << 5); //Clearing the pending flag
}


int main(){
    uint16_t mcp_data = 0;

    system_init();

    //Turn off led
    //gpio_out(gpio_c, 13, 0);
    ledVal = 0;

    screen_fill(Color565(255,0,0));

    int x_vel = 0, y_vel = 0;
    uint8_t x_pos = 0, y_pos = 0, x_prev = 0, y_prev = 0;
    
    st7735_tearing_off(GPIOA, SPI1);

    while(1){

	//if(keypadkeys == 0xffff){
	    //continue;
	//}
	
	//uint8_t inp = (keypad_read() & 0x00ff);
	//uint8_t inp = (keypadkeys & 0x00ff);
	uint8_t inp = keypadkeys & 0x00ff;
	//uint8_t inp = gpio_in(gpio_b, 5);
	inp = ~inp;
        if(inp){
	    //NB display is horizontal
	    //up	
	    if(inp & 0x1){
	     x_vel = 1;
	    }
	    //down	
	    if(inp & 0x2){
	     x_vel = -1;
	    }
	    //left
	    if(inp & 0x4){
	     y_vel = -1;
	    }
	    //right
	    if(inp & 0x8){
	     y_vel = 1;
	    }
	}else{
	    x_vel = 0;
	    y_vel = 0;
	    continue;
	}

        x_prev = x_pos;
        y_prev = y_pos;
	/*
	inp = ((keypad_read(i2c1) & 0xff00) >> 8);
	inp = ~inp;
        if(inp){	
	    gpio_out(gpio_c, 13, 0);
	    continue;
	}
	gpio_out(gpio_c, 13, 1);
	*/
        if(x_pos + x_vel + 8 > SCREEN_WIDTH)
        {
            x_vel = 0;
        }
        if(x_pos + x_vel < 0)
        {
            x_vel = 0;
        }

        if(y_pos + y_vel + 8 > SCREEN_HEIGHT)
        {
            y_vel = 0;
        }

        if(y_pos + y_vel < 0)
        {
            y_vel = 0;
        }

        x_pos += x_vel;
        y_pos += y_vel;

        screen_fill_rect(x_prev, y_prev, 8, 8, Color565(0,0,255));
        screen_fill_rect(x_pos, y_pos, 8, 8, Color565(0,0,0));
        _delay_ms(50);

    }
 return 0;
}
