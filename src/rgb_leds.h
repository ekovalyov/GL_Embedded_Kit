#ifndef _RGB_LEDS_H_
#define _RGB_LEDS_H_

#include <stdio.h>

#include "stm32f4_discovery.h"
#include "rgb_leds.h"

#define NLEDS 24
#define GPIO_RGB_LEDS GPIO_Pin_3
extern uint8_t RGB[3][NLEDS + 1];

void RGB_LEDS_show_all(void);
void RGB_LEDS_fill_all(void);
void RGB_demo1(void);
void RGB_demo2(void);
void RGB_demo3(void);

#endif
