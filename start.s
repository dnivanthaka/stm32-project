.section .vectors
.cpu cortex-m4
.thumb

/*---------------- Setup vector table entries -----------------*/

.word 0x20020000 /*SP 131xxxbytes = 0x20000*/
.word _reset     /*Reset*/
.word hang       /*NMI*/
.word hang       /*Hard fault*/
.word hang       /*MM fault*/
.word hang       /*Bus fault*/
.word hang       /*Usage fault*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*SVCall*/
.word hang       /*Debug - reserved*/
.word hang       /*Reserved*/
.word hang       /*Pend SV*/
.word systick_handler       /*Systick*/

.word hang       /*Watchdog window*/
.word hang       /*EXTI16/PVD*/
.word hang       /*EXTI21/TAMPER*/
.word hang       /*EXTI22/RTC global*/
.word hang       /*Flash global*/
.word hang       /*RCC global*/
.word hang       /*EXTI0*/
.word hang       /*EXTI1*/
.word hang       /*EXTI2*/
.word exti3_irq_handler       /*EXTI3*/
.word exti4_irq_handler       /*EXTI4*/
.word hang       /*DMA 0*/
.word hang       /*DMA 1*/
.word hang       /*DMA 2*/
.word hang       /*DMA 3*/
.word hang       /*DMA 4*/
.word hang       /*DMA 5*/
.word hang       /*DMA 6*/
.word hang       /*ADC*/

.word hang
.word hang
.word hang
.word hang

.word exti9_5_irq_handler  /*EXTI 9 -5*/
.word hang       /*TIM1_BRK_TIM9*/
.word hang       /*TIM1_UP_TIM10*/
.word hang       /*TIM1_TRG_COM_TIM11*/
.word hang       /*TIM1_CC*/
.word hang       /*TIM2*/
.word hang       /*TIM3*/
.word hang       /*TIM4*/
.word hang       /*I2C1_EV*/
.word hang       /*I2C1_ER*/
.word hang       /*I2C2_EV*/
.word hang       /*I2C2_ER*/
.word hang       /*SPI1*/
.word hang       /*SPI2*/
.word hang       /*USART1*/
.word hang       /*USART2*/

.word hang

.word hang       /*EXTI15_10*/
.word hang       /*EXTI17*/
.word hang       /*EXTI18*/

.word hang
.word hang
.word hang
.word hang

.word hang       /*DMA1_7*/

.word hang

.word hang       /*SDIO*/
.word hang       /*TIM5*/
.word hang       /*SPI3*/

.word hang
.word hang
.word hang
.word hang

.word hang       /*DMA2_Stream0*/
.word hang       /*DMA2_Stream1*/
.word hang       /*DMA2_Stream2*/
.word hang       /*DMA2_Stream3*/
.word hang       /*DMA2_Stream4*/

.word hang
.word hang
.word hang
.word hang
.word hang
.word hang

.word hang       /*OTG_FS*/
.word hang       /*DMA2_Stream5*/
.word hang       /*DMA2_Stream6*/
.word hang       /*DMA2_Stream7*/
.word hang       /*USART6*/
.word hang       /*I2C3_EV*/
.word hang       /*I2C3_ER*/

.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang

.word hang       /*FPU*/

.word hang
.word hang

.word hang       /*SPI4*/
.word hang       /*SPI5*/  

.section .text

.thumb_func
hang: b hang

.thumb_func
_reset:
 bl startup
 b .             /*Should not reach here*/

.align

.thumb_func
.globl PUT16
PUT16:
 strh r1, [r0]
 bx lr

.thumb_func
.globl PUT32
PUT32:
 str r1, [r0]
 bx lr

.thumb_func
.globl GET32
GET32:
 ldr r0, [r0]
 bx lr

.end
