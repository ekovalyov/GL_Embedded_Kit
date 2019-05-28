#ifndef _DHT_H_
#define _DHT_H_

#include <stdio.h>

void DHT_start(void);
uint8_t DHT_check_response(void);
uint8_t DHT_read_byte(void);

#endif
