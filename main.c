#include <math.h>

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


#define ABS(x) (x < 0)?-x:x
#define SGN(x) (x < 0)?-1:1

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

uint8_t ledVal = 0;
volatile uint16_t keypadkeys = 0xffff;

typedef struct coord_t {
    uint16_t x, y;
} coord_t;

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

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    int xdiff = x2 - x1;
    int ydiff = y2 - y1;
    uint16_t error_term = 0;
    int x_inc = 1, y_inc = 1;

    if(ydiff < 0){
        ydiff = ABS(ydiff);
        y_inc = -1;
    }

    if(xdiff < 0){
        xdiff = ABS(xdiff);
        x_inc = -1;
    }

    //y  = mx
    if(xdiff > ydiff){
       for(uint16_t i=0;i < xdiff;i++){
          screen_putpixel(SCREEN_WIDTH - x1, y1, color);
	  x1 += x_inc;
          error_term += ydiff;

          if(error_term > xdiff){
	     y1 += y_inc;
	     error_term -= xdiff;
          } 
        }
    }else if (xdiff < ydiff){
       for(uint16_t i=0;i < ydiff;i++){
          screen_putpixel(SCREEN_WIDTH - x1, y1, color);
	  y1 += y_inc;
          error_term += xdiff;

          if(error_term > ydiff){
	     x1 += x_inc;
	     error_term -= ydiff;
          } 
        }
    }else{
    //xdiff = ydiff
       for(uint16_t i=0;i < xdiff;i++){
          screen_putpixel(SCREEN_WIDTH - x1, y1, color);
	  x1 += x_inc;
	  y1 += y_inc;
        }
    }
}

void draw_polygon(uint16_t count, coord_t *vertices, uint16_t color) { 
    for(int i=0;i<count-1;i++){
        draw_line((vertices + i)->x, (vertices + i)->y, (vertices + i + 1)->x, (vertices + i + 1)->y, color);
    }

    //draw the last connecting line
    draw_line(vertices->x, vertices->y, (vertices + (count-1))->x, (vertices + (count-1))->y, color);
}

void draw_vline(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color) {
    int xdiff = x2 - x1;
    int x_inc = 1;

    if(xdiff < 0){
        xdiff = ABS(xdiff);
        x_inc = -1;
    }
    for(uint16_t i=0;i < xdiff;i++){
       screen_putpixel(SCREEN_WIDTH - x1, y, color);
       x1 += x_inc;
    }
}

void draw_hline(uint16_t x, uint16_t y1, uint16_t y2,  uint16_t color) {
    int ydiff = y2 - y1;
    int y_inc = 1;

    if(ydiff < 0){
        ydiff = ABS(ydiff);
        y_inc = -1;
    }
    for(uint16_t i=0;i < ydiff;i++){
       screen_putpixel(SCREEN_WIDTH - x, y1, color);
       y1 += y_inc;
    }
}

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    draw_vline(x, x+h, y, color);
    draw_hline(x+h, y, y+w, color);
    draw_vline(x+h, x, y+w, color);
    draw_hline(x, y+w, y, color);
}

void draw_circle(int8_t x0, int8_t y0, int8_t r, uint16_t color) {
    int8_t f = 1 - r;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * r;
    int8_t x = 0;
    int8_t y = r;

    screen_putpixel(x0, y0 + r, color);
    screen_putpixel(x0, y0 - r, color);
    screen_putpixel(x0 + r, y0, color);
    screen_putpixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {

            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        screen_putpixel(x0 + x, y0 + y, color);
        screen_putpixel(x0 - x, y0 + y, color);
        screen_putpixel(x0 + x, y0 - y, color);
        screen_putpixel(x0 - x, y0 - y, color);
        screen_putpixel(x0 + y, y0 + x, color);
        screen_putpixel(x0 - y, y0 + x, color);
        screen_putpixel(x0 + y, y0 - x, color);
        screen_putpixel(x0 - y, y0 - x, color);

    }
}

void run_demo() {
    coord_t vertices[3];

    //Triangle
    vertices[0].x = 100, vertices[0].y = 10;
    vertices[1].x = 30,  vertices[1].y = 50;
    vertices[2].x = 100, vertices[2].y = 100;

    draw_polygon(3, vertices, Color565(255,0,0));
    draw_hline(50, 10, 60, Color565(0,255,0));
    draw_vline(50, 10, 60, Color565(0,255,255));
    //draw_rect(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 50), rand() % 100, rand() % 50, Color565(255,255,0));
    //draw_rect(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 50), rand() % 100, rand() % 50, Color565(255,0,255));
    draw_rect(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 50), rand() % 100, rand() % 50, Color565(0,255,255));
    draw_circle(50, 50, 30, Color565(0, 255, 0));
}

int main(){
    uint16_t mcp_data = 0;

    system_init();

    rand_seed(0xABC);


    //Turn off led
    //gpio_out(gpio_c, 13, 0);
    ledVal = 0;

    screen_fill(Color565(0,0,0));

    int x_vel = 0, y_vel = 0;
    uint8_t x_pos = 0, y_pos = 0, x_prev = 0, y_prev = 0;
    
    st7735_tearing_off(GPIOA, SPI1);

    //draw_line(0, 0, 100, 100, Color565(255,0,0));
    //
    run_demo();


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
	    if(KEYPAD_UP(inp)){
	     x_vel = 1;
	    }
	    //down	
	    if(KEYPAD_DOWN(inp)){
	     x_vel = -1;
	    }
	    //left
	    if(KEYPAD_LEFT(inp)){
	     y_vel = -1;
	    }
	    //right
	    if(KEYPAD_RIGHT(inp)){
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
