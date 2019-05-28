
#include "stm32f4_discovery.h"

#include "delay.h"
#include "dht.h"


const uint16_t GPIO_Pin_DHT = GPIO_Pin_11;
const unsigned int DHT_wait0_cycles = 720000;
const unsigned int DHT_wait1_cycles = 720000;

//uint8_t check = 0;

void DHT_set_output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* Configure GPIO pin output: PD11 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_DHT;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void DHT_set_input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pin input: PD11 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_DHT;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void DHT_start(void)
{
    DHT_set_output();  // set the pin as output
    GPIO_ResetBits(GPIOD, GPIO_Pin_DHT);   // pull the pin low
    delay_ms(18);   // wait for 18ms
    DHT_set_input();   // set as input
}

int DHT_get_bit(void) {
    return GPIOD->IDR & GPIO_Pin_DHT;
}

int tm_response_1;
int tm_response_0;

uint8_t DHT_check_response(void)
{
    int k;
    uint8_t rv = 0;

    delay_10us(4);
    if (!DHT_get_bit())
    {
        delay_10us(8);
        if (DHT_get_bit()) {
            tm_response_1 = TIM_GetCounter(TIM3);
            rv = 1;
        }
    }
    for (k=0; k<DHT_wait0_cycles; ++k) {
        if (!DHT_get_bit()) {
            // wait for the pin to go low
            tm_response_1 = TIM_GetCounter(TIM3);
            break;
        }
    }
    return rv;
}

uint8_t DHT_read_byte(void)
{
    uint8_t i = 0;
    uint8_t j;
    int k;
    for (j = 0x80; j; j >>= 1)
    {
        for (k=0; k<DHT_wait1_cycles  &&  !DHT_get_bit(); ++k);   // wait for the pin to go high
        delay_10us(4);   // wait for 40 us
        if (DHT_get_bit()) {   // if the pin is low
            i |= j;  // if the pin is high, write 1
        }
        for (k=0; k<DHT_wait0_cycles  &&  DHT_get_bit(); ++k);   // wait for the pin to go low
    }
    return i;
}

uint8_t DHT_read_word(void)
{
    uint16_t i = 0;
    uint16_t j;
    int k;
    for (j = 0x8000; j; j >>= 1)
    {
        for (k=0; k<DHT_wait1_cycles  &&  !DHT_get_bit(); ++k);   // wait for the pin to go high
        delay_10us(4);   // wait for 40 us
        if (DHT_get_bit()) {   // if the pin is low
            i |= j;  // if the pin is high, write 1
        }
        for (k=0; DHT_get_bit(); ++k) {
            // wait for the pin to go low
            if (k >= DHT_wait0_cycles) {
                return i;
            }
        }
    }
    return i;
}
