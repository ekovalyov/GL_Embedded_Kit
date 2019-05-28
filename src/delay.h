#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdio.h>

void TIM3_init(void);
void delay_10us(uint16_t us10);
void delay_ms(uint16_t ms);

void Delay(__IO uint32_t nCount);

#endif
