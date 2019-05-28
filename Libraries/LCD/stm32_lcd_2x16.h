//--------------------------------------------------------------
// File     : stm32_lcd_2x16.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_LCD_2X16_H
#define __STM32F4_LCD_2X16_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


//--------------------------------------------------------------
// ����� �������
//--------------------------------------------------------------
typedef enum {
  TLCD_OFF =0,
  TLCD_ON,
  TLCD_CURSOR,
  TLCD_BLINK
} TLCD_MODE_t;


//--------------------------------------------------------------
//--------------------------------------------------------------
#define  TLCD_INIT_PoffE  100000  // �������� ��� ������������� (>=70000)
#define  TLCD_PoffE        50000  // �������� (>=20000)
#define  TLCD_CLK_PoffE     1000  // �������� ��������� �������� (>=500)
#define  TLCD_MAXX            16  // max x-������� (0...15)
#define  TLCD_MAXY             2  // max y-������� (0...1)


//--------------------------------------------------------------
//--------------------------------------------------------------
#define  TLCD_CMD_INIT_DISPLAY  0x28
#define  TLCD_CMD_ENTRY_MODE    0x06
#define  TLCD_CMD_DISP_M0       0x08
#define  TLCD_CMD_DISP_M1       0x0C
#define  TLCD_CMD_DISP_M2       0x0E
#define  TLCD_CMD_DISP_M3       0x0F
#define  TLCD_CMD_CLEAR         0x01

#define TLCD_PAUSE 100000
#define TLCD_PAUSE_1UC 10000
#define TLCD_CLK_PAUSE 1000
#define TLCD_INIT_PAUSE 150000

typedef enum 
{
  TLCD_RS = 0,  // RS-Pin
  TLCD_E  = 1,  // E-Pin
  TLCD_D4 = 2,  // DB4-Pin
  TLCD_D5 = 3,  // DB5-Pin
  TLCD_D6 = 4,  // DB6-Pin
  TLCD_D7 = 5,  // DB7-Pin
  TLCD_RW = 6,  // R/nW-Pin
  TLCD_ANZ = 7
}TLCD_NAME_t;

//--------------------------------------------------------------
//--------------------------------------------------------------
typedef struct {
  TLCD_NAME_t TLCD_NAME;   // Name
  GPIO_TypeDef* TLCD_PORT; // Port
  const uint16_t TLCD_PIN; // Pin
  const uint32_t TLCD_CLK; // Clock
  BitAction TLCD_INIT;     // Init
}LCD_2X16_t;

//--------------------------------------------------------------
//--------------------------------------------------------------
void UB_LCD_2x16_Init(void);
void UB_LCD_2x16_Clear(void);
void UB_LCD_2x16_SetMode(TLCD_MODE_t mode);
void UB_LCD_2x16_String(uint8_t x, uint8_t y, char *ptr);



//--------------------------------------------------------------
#endif // __STM32F4_UB_LCD_2X16_H
