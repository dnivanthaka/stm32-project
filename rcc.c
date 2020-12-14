#include "types.h"
#include "stmf103xxx.h"
#include "timer.h"
#include "rcc.h"


/*
 This function would initialize the registers but not activate the counter
 Counter would be started by the delay function
*/
void systick_init(systick_t *syt){
 //Here the calibration value should be set according the cpu clock, for now its set to 9Mhz
 syt->ctrl &= ~SYT_ENABLE;
 syt->ctrl &= ~SYT_COUNTF;
 syt->ctrl &= ~SYT_SRC;

 syt->calib = 0x0002328;        //9Mhz

 syt->load  = 0;
}

void systick_interrupt_start(systick_t *syt) {
 syt->load = ((SYSTEM_CORE_CLOCK / 8) / 1000);  //every millisecond
 syt->val = 0;
 syt->ctrl &= ~SYT_COUNTF;
 syt->ctrl |= SYT_INT;
 syt->ctrl |= SYT_ENABLE;
}

void rcc_init(rcc_t *rcc){
 rcc_setup_cpu(rcc, PLL_9, PPRE2_8);
}

void delay_ms_old(systick_t *syt, uint32_t val){
 //syt->load = 0xffff - 1;
 syt->load = 0xfff - 1;
 syt->val = 0;
 syt->ctrl &= ~SYT_COUNTF;
 syt->ctrl |= SYT_ENABLE;

 while(val--){
  while(! (syt->ctrl & SYT_COUNTF));
 }

 //Stop the counter
 syt->ctrl &= ~SYT_ENABLE;
}

/**
 *  
 */
void _delay_us(timer_t *tim, uint32_t delay) {
    if(tim == (struct timer_t *)TIM3BASE){
        RCC->apb1enr |= 1 << 1; 
    } 

    tim->psc = 0;
    tim->arr = 71;
    tim->sr = 0;
    tim->cr1 = 1;
    while(delay--){
        while(! (tim->sr & 1));
    }

    tim->cr1 = 0; //stop counter
}

void _delay_ms(timer_t *tim, uint32_t delay) {
    if(tim == (struct timer_t *)TIM3BASE){
        RCC->apb1enr |= 1 << 1; 
    } 

    tim->psc = 7200 - 1;   //72Mhz / 7200 => 10Khz = 1/100 = 0.1ms
    tim->arr = 10 - 1; //0.1ms * 10 = 1ms
    tim->sr = 0;
    tim->cr1 = 1;
    while(delay--){
        while(! (tim->sr & 1));
    }

    tim->cr1 = 0; //stop counter
}

void rcc_setup_cpu(rcc_t *rcc, uint32_t pll_clk, uint32_t apb_clk){
 //Enable HSE
 rcc->cr |= (1 << 16);
 while(! (rcc->cr & (1 << 17)));

 rcc->cfgr |= 1;  //Switch to HSE
 rcc->cr &= ~1;   // Disable HSI
 rcc->cfgr |= pll_clk; //PLL = 9
 rcc->cfgr |= (1 << 16); //HSE as PLL_SRC

 *FLASH_ACR = 0b10010;

 rcc->cr |= (1 << 24);
 while(! (rcc->cr & (1 << 25)));

 rcc->cfgr = (rcc->cfgr | 0b10) & ~1 | apb_clk; // ABP = CLK / 8
 while(! (rcc->cfgr & (1 << 3)));
}
