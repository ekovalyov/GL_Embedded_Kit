//--------------------------------------------------------------
// File     : stm32_ub_lcd_2x16.c
// Datum    : 15.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO
// Funktion : Text-LCD Funktionen (2x16 Zeichen)
//            im 4Bit-Mode
//            Chip : ST7066U/HD44780/SED1278/KS0066U/S6A0069X
// 
// Hinweis  : Das Display benutzt die CPU-Pins :
//             PE5  -> LCD_RS
//             PE6  -> LCD_E
//             PE7  -> LCD_DB4
//             PE8  -> LCD_DB5
//             PE9  -> LCD_DB6
//             PE10 -> LCD_DB7
//--------------------------------------------------------------



//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_lcd_2x16.h"



//--------------------------------------------------------------
// ���������� �������
//--------------------------------------------------------------
void P_LCD_2x16_InitIO(void);
void P_LCD_2x16_PinLo(TLCD_NAME_t lcd_pin);
void P_LCD_2x16_PinHi(TLCD_NAME_t lcd_pin);
void P_LCD_2x16_Clk(void);
void P_LCD_2x16_InitSequenz(void);
void P_LCD_2x16_Cmd(uint8_t wert);
void P_LCD_2x16_Data(uint8_t wert);
void P_LCD_2x16_Cursor(uint8_t x, uint8_t y);
void P_LCD_2x16_Delay(volatile uint32_t nCount);



//--------------------------------------------------------------
// ����������� ���� �������, ������������ � �������
// ������������� � TLCD_NAME_t
//
// ������������� : [Bit_SET,Bit_RESET]
//
// TLCD_RW was appended to original library
// Pin numbers were changes for GL Embedded Starter Kit
//--------------------------------------------------------------
LCD_2X16_t LCD_2X16[] = {
 // Name   ,PORT , PIN       , CLOCK              , Init
  {TLCD_RS, GPIOE, GPIO_Pin_7,  RCC_AHB1Periph_GPIOE, Bit_RESET},
  {TLCD_E , GPIOE, GPIO_Pin_11, RCC_AHB1Periph_GPIOE, Bit_RESET},
  {TLCD_D4, GPIOE, GPIO_Pin_12, RCC_AHB1Periph_GPIOE, Bit_RESET},
  {TLCD_D5, GPIOE, GPIO_Pin_13, RCC_AHB1Periph_GPIOE, Bit_RESET},
  {TLCD_D6, GPIOE, GPIO_Pin_14, RCC_AHB1Periph_GPIOE, Bit_RESET},
  {TLCD_D7, GPIOE, GPIO_Pin_15, RCC_AHB1Periph_GPIOE, Bit_RESET},
  {TLCD_RW, GPIOE, GPIO_Pin_10, RCC_AHB1Periph_GPIOE, Bit_RESET},
};

//--------------------------------------------------------------
// ������������� ������� � ��������� ������
//--------------------------------------------------------------
void UB_LCD_2x16_Init(void)
{
  P_LCD_2x16_InitIO();
  P_LCD_2x16_Delay(TLCD_INIT_PAUSE);
  P_LCD_2x16_InitSequenz();
  P_LCD_2x16_Cmd(TLCD_CMD_INIT_DISPLAY);
  P_LCD_2x16_Cmd(TLCD_CMD_ENTRY_MODE);
  P_LCD_2x16_Cmd(TLCD_CMD_DISP_M1);
  P_LCD_2x16_Cmd(TLCD_CMD_CLEAR);
  P_LCD_2x16_Delay(TLCD_PAUSE);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void UB_LCD_2x16_Clear(void)
{
  P_LCD_2x16_Cmd(TLCD_CMD_CLEAR);
  P_LCD_2x16_Delay(TLCD_PAUSE);
}


//--------------------------------------------------------------
// MODE : [TLCD_OFF, TLCD_ON, TLCD_CURSOR, TLCD_BLINK]
//--------------------------------------------------------------
void UB_LCD_2x16_SetMode(TLCD_MODE_t mode)
{
  if (mode == TLCD_OFF) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M0);
  if (mode == TLCD_ON) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M1);
  if (mode == TLCD_CURSOR) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M2);
  if (mode == TLCD_BLINK) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M3);
}


//--------------------------------------------------------------
// x : 0 .. 15
// y : 0 .. 1
//--------------------------------------------------------------
void UB_LCD_2x16_String(uint8_t x, uint8_t y, char *ptr)
{
  P_LCD_2x16_Cursor(x,y);
  while (*ptr != 0) {
    P_LCD_2x16_Data(*ptr);
    ptr++;
  }
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_InitIO(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  TLCD_NAME_t lcd_pin;
  
  for(lcd_pin=0; lcd_pin<TLCD_ANZ; lcd_pin++) {
    RCC_AHB1PeriphClockCmd(LCD_2X16[lcd_pin].TLCD_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LCD_2X16[lcd_pin].TLCD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // old 50MHz;
    GPIO_Init(LCD_2X16[lcd_pin].TLCD_PORT, &GPIO_InitStructure);

    if(LCD_2X16[lcd_pin].TLCD_INIT==Bit_RESET) {
      P_LCD_2x16_PinLo(lcd_pin);
    } else {
      P_LCD_2x16_PinHi(lcd_pin);
    }
  }  
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_PinLo(TLCD_NAME_t lcd_pin)
{
  LCD_2X16[lcd_pin].TLCD_PORT->BSRRH = LCD_2X16[lcd_pin].TLCD_PIN;
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_PinHi(TLCD_NAME_t lcd_pin)
{
  LCD_2X16[lcd_pin].TLCD_PORT->BSRRL = LCD_2X16[lcd_pin].TLCD_PIN;
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_Clk(void)
{
  P_LCD_2x16_PinHi(TLCD_E);
  P_LCD_2x16_Delay(TLCD_CLK_PAUSE);
  P_LCD_2x16_PinLo(TLCD_E);
  P_LCD_2x16_Delay(TLCD_CLK_PAUSE);  
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_InitSequenz(void)
{
  P_LCD_2x16_PinHi(TLCD_D4);
  P_LCD_2x16_PinHi(TLCD_D5);
  P_LCD_2x16_PinLo(TLCD_D6);
  P_LCD_2x16_PinLo(TLCD_D7);

  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE_1UC*4100);  // > 4.1 ms
  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE_1UC*100);   // > 100 us
  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE);
  P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_PinHi(TLCD_D5);
  P_LCD_2x16_PinLo(TLCD_D6);
  P_LCD_2x16_PinLo(TLCD_D7);
  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_Cmd(uint8_t wert)
{
  // RS=Lo
  P_LCD_2x16_PinLo(TLCD_RS);
      
  if (wert&0x80) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if (wert&0x40) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if (wert&0x20) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if (wert&0x10) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();
        
  if (wert&0x08) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if (wert&0x04) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if (wert&0x02) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if (wert&0x01) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_Data(uint8_t wert)
{
  // RS=Hi
  P_LCD_2x16_PinHi(TLCD_RS);
       
  if((wert&0x80)!=0) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if((wert&0x40)!=0) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if((wert&0x20)!=0) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if((wert&0x10)!=0) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();
       
  if((wert&0x08)!=0) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if((wert&0x04)!=0) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if((wert&0x02)!=0) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if((wert&0x01)!=0) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();  
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_Cursor(uint8_t x, uint8_t y)
{
  uint8_t wert;

  if (x >= TLCD_MAXX) x=0;
  if (y >= TLCD_MAXY) y=0;

  wert = 0x80 | (y<<6) | x;
  P_LCD_2x16_Cmd(wert);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void P_LCD_2x16_Delay(volatile uint32_t nCount)
{
  while(nCount--)
  {
  }
}
