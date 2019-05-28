#include "stm32f4_discovery.h"
#include <stdio.h>

void TIM3_init(void) {
    TIM_TimeBaseInitTypeDef tm;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    tm.TIM_Period = 65535;
    tm.TIM_Prescaler = 800;// 80MHz // old 48 MHz;
    tm.TIM_ClockDivision = 0;
    tm.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &tm);
}

void delay_10us(uint16_t us10) {
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
    while (TIM_GetCounter(TIM3) < us10) {
        ;
    }
    //TIM_Cmd(TIM3, DISABLE);
}

void delay_ms(uint16_t ms) {
    delay_10us(ms*100);
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
    while(nCount--) {
    }
}
