#include "sound.h"
#include "stmf103xxx.h"

void sound_init() {
   //enable TIM2 clock
   RCC->apb2enr |= 0xFC;
   RCC->apb1enr |= (1 << 0);

   TIM2->ccer = 1 << 0; //CC1P = 0, CC1E = 1
   TIM2->ccmr[0] = 0x0068; //PWM1, OC1PE = 1
   TIM2->cr1 = 0x80;

   TIM2->psc = 720 - 1; //prescalar 72
   TIM2->arr = 1100 - 1; //ARR = 999
   TIM2->ccr[0] = 1000; //duty cycle (300 / 1000) * 100
   TIM2->egr = 1; // UG = 1 generate update
   TIM2->cr1 |= 0x01; //timer enable
}

void sound_on() {
   TIM2->cr1 |= 0x01; //timer enable
}

void sound_off() {
   TIM2->cr1 &= ~(0x01); //timer disable
}