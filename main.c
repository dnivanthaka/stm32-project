#include "globals.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "st7735.h"
#include "i2c.h"
#include "interrupts.h"


#define KEYPAD_ADDR 0x20


struct exti *EXTI = (struct exti *)EXTIBASE;
struct gpio_t *GPIOC = (struct gpio_t *)GPIOCBASE;
struct afio *AFIO = (struct afio *)AFIOBASE;

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

uint8_t ledVal = 0;

void spi_mcp23s17_write(spi_t *spi, uint8_t addr, uint8_t reg, uint8_t val){
     spi_transfer(spi, addr);
     spi_transfer(spi, reg);
     spi_transfer(spi, val);
}

void interrupt_handler(){
    gpio_out(GPIOC, 13, ledVal);
    ledVal = (~ledVal) & 1;

    EXTI->pr = (1 << 4); //Clearing the pending flag
}

void nvic_enable_irq(uint32_t irq){
    struct nvic *ptr = (struct nvic *)NVICBASE;

    if(irq > MAX_IRQ){
       return; 
    }

    ptr->iser[irq / 32] = 1 << (irq % 32);
}

void nvic_disable_irq(uint32_t irq){
    struct nvic *ptr = (struct nvic *)NVICBASE;

    if(irq > MAX_IRQ){
       return; 
    }

    ptr->iser[irq / 32] &= ~(1 << (irq % 32));
}

void mcp23s17_write(i2c_t *i2c, uint8_t reg, uint8_t data){

    do{
      i2c_wait_for_ready(i2c);
      i2c_send_start(i2c);
    }while(i2c_send_addr_for_write(i2c, KEYPAD_ADDR) != 0);

    i2c_write_data(i2c, reg);
    i2c_write_data(i2c, data);
    i2c_send_stop(i2c);
}

uint8_t mcp23s17_read(i2c_t *i2c, uint8_t reg){
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

int startup(){
    uint16_t mcp_data = 0;
    gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
    gpio_t *gpio_b = (struct gpio_t *) GPIOBBASE;
    gpio_t *gpio_a = (struct gpio_t *) GPIOABASE;
    rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;
    systick_t *syt = (struct systick_t *) SYSTICKBASE;
 
    usart_t *usart = (usart_t *) USART1BASE;

    spi_t   *spi1 = (spi_t *)SPI1BASE;

    i2c_t *i2c1 = (i2c_t *)I2C1BASE;

    //exti_t *exti = (struct exti_t *)EXTIBASE;

    rcc_init(rcc);
    systick_init(syt);
 
    /*Enable PORT A, PA9 = TX, PA10 = RX*/
    gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
    gpio_init(gpio_a, rcc, 10,  GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT);
    //RES
    gpio_init(gpio_a, rcc, 2, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //D/C
    gpio_init(gpio_a, rcc, 1, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //CE
    gpio_init(gpio_a, rcc, 3, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    gpio_init(gpio_a, rcc, 4, GPIO_MODE_INPUT | GPIO_CNF_IN_PULL );
    gpio_init(gpio_b, rcc, 4, GPIO_MODE_INPUT | GPIO_CNF_IN_PULL );

    gpio_init(gpio_c, rcc, 13, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    gpio_init(gpio_a, rcc, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    gpio_init(gpio_a, rcc, 6,  GPIO_MODE_INPUT | GPIO_CNF_IN_PULL);
    gpio_init(gpio_a, rcc, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);

    gpio_init(gpio_b, rcc, 6,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);              //I2C pins
    gpio_init(gpio_b, rcc, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);

    //rcc->apb1enr |= 1;

    // Disable Reset
    rcc->apb2rstr &= ~(1 << 14);
    rcc->apb2rstr &= ~(1 << 12);

    //Enable USART1 clock
    rcc->apb2enr |= (1 << 14) | 1;
    //Enable SPI1 clock
    rcc->apb2enr |= (1 << 12) | 1;
    //rcc->apb2enr |= 0x4000;

    //gpio_out(gpio_a, 4, 1);
    //
    //Setting up interrupts
    //AFIO->exticr[1] = 1 << 0;   //PB4 as input
    //EXTI->ftsr = (1 << 4); //falling edge
    //EXTI->imr = (1 << 4);  // enable interrupt exti4

    //nvic_enable_irq(EXTI4_IRQ);


    usart_init(usart);

    spi_init(spi1, SPI_MASTER, SPI_MODE_UNI2 | SPI_OUTPUT_ENABLE, 
            SPI_DFF_8BIT, SPI_CLK_DIV2, SPI_CPOL_0, SPI_CPHA_1, SPI_SLAVE_MGMT_DISABLE);

    uint8_t i2c_addr = 0x20;

    i2c_init(i2c1, rcc);

    mcp23s17_write(i2c1, 0x00, 0xff);
    mcp23s17_write(i2c1, 0x01, 0xff);
    //mcp23s17_write(i2c1, 0x14, 0xf0);
    //mcp23s17_write(i2c1, 0x15, 0xf0);
    //mcp23s17_write(i2c1, 0x01, 0xff);

    //enable pullups
    mcp23s17_write(i2c1, 0x0C, 0xff);
    mcp23s17_write(i2c1, 0x0D, 0xff);

    //enable interrupts
    mcp23s17_write(i2c1, 0x06, 0xff);
    mcp23s17_write(i2c1, 0x07, 0xff);

    mcp23s17_write(i2c1, 0x0e, 0);
    mcp23s17_write(i2c1, 0x0f, 0);

    mcp23s17_write(i2c1, 0x08, 0xff);
    mcp23s17_write(i2c1, 0x09, 0xff);

    mcp23s17_write(i2c1, 0x04, 0xff);
    mcp23s17_write(i2c1, 0x05, 0xff);


    //st7735_init(gpio_a, spi1, syt);
    //Turn off led
    gpio_out(gpio_c, 13, 1);
    ledVal = 0;

    //interrupt_handler();


    //st7735_fill_screen(Color565(255,0,0), gpio_a, spi1);
    //gpio_out(gpio_a, 2, 1);
    //gpio_out(gpio_a, 3, 1);

    //st7735_test(gpio_a, spi1, syt);
    //gpio_out(gpio_a, 3, 0);
    //spi_transfer(spi1, 0x40);
    //spi_transfer(spi1, 0x00);
    //spi_transfer(spi1, 0x00);
    //gpio_out(gpio_a, 3, 1);
    //delay_ms(syt, 200);
    //gpio_out(gpio_a, 3, 0);
    //spi_transfer(spi1, 0x40);
    //spi_transfer(spi1, 0x14);
    //spi_transfer(spi1, 0xff);
    //gpio_out(gpio_a, 3, 1);

    while(1){
	uint8_t inp = mcp23s17_read(i2c1, 0x12);
	inp = ~inp;
        if(inp){	
	    gpio_out(gpio_c, 13, 0);
	    continue;
	}
	inp = mcp23s17_read(i2c1, 0x13);
	inp = ~inp;
        if(inp){	
	    gpio_out(gpio_c, 13, 0);
	    continue;
	}
	//gpio_out(gpio_c, 13, 0);
    	//gpio_out(gpio_a, 2, 0);
	//delay_ms(syt, 10);
	gpio_out(gpio_c, 13, 1);
    	//gpio_out(gpio_a, 2, 1);
	//delay_ms(syt, 10);
    }

 return 0;
}
