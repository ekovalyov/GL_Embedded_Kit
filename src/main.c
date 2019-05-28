/**
  ******************************************************************************
  * @file    LCD_Demo/main.c
  * @author  Yevgen Kovalyov
  * @version V1.0.1
  * @date    23-May-2019
  * @brief   Main program body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32_lcd_2x16.h"

#include <stdio.h>

#include "delay.h"
#include "dht.h"
#include "rgb_leds.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

void gpioa_init() {
    GPIO_InitTypeDef GPIOx;
    /* GPIOD Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Configure PA15 in input mode */
    GPIOx.GPIO_Pin = GPIO_Pin_15;
    GPIOx.GPIO_Mode = GPIO_Mode_IN;
    GPIOx.GPIO_OType = GPIO_OType_OD;
    GPIOx.GPIO_Speed = GPIO_Speed_100MHz;
    GPIOx.GPIO_PuPd = GPIO_PuPd_UP;  // external pullup
    GPIO_Init(GPIOA, &GPIOx);
}

void gpioc_init() {
    GPIO_InitTypeDef GPIOx;
    /* GPIOC Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* Configure PC6, PC8, PC9 and PC11 in input mode */
    GPIOx.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_8;
    GPIOx.GPIO_Mode = GPIO_Mode_IN;
    //GPIOx.GPIO_OType = GPIO_OType_PP;
    GPIOx.GPIO_Speed = GPIO_Speed_100MHz;
    GPIOx.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIOx);
}

void gpiod_init() {
    GPIO_InitTypeDef GPIOx;
    /* GPIOD Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
    GPIOx.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
            | GPIO_RGB_LEDS;
    GPIOx.GPIO_Mode = GPIO_Mode_OUT;
    GPIOx.GPIO_OType = GPIO_OType_PP;
    GPIOx.GPIO_Speed = GPIO_Speed_100MHz;
    GPIOx.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIOx);
}

void gpioe_init() {
    GPIO_InitTypeDef GPIOx;
    /* GPIOE Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    /* Configure PE9 in output pushpull mode */
    GPIOx.GPIO_Pin = GPIO_Pin_9;
    GPIOx.GPIO_Mode = GPIO_Mode_OUT;
    GPIOx.GPIO_OType = GPIO_OType_PP;
    GPIOx.GPIO_Speed = GPIO_Speed_100MHz;
    GPIOx.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIOx);
}

void gpiod_blink() {
    /* PD12 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    Delay(0x3FFFFF);
    
    /* PD13 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    Delay(0x3FFFFF);
  
    /* PD14 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    Delay(0x3FFFFF);
    
    /* PD15 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    Delay(0x7FFFFF);
    
    GPIO_SetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
    
    /* Insert delay */
    Delay(0xFFFFFF);
}


//uint16_t nleds = 24;
uint8_t color_level = 50;
uint8_t color = 0;
uint8_t RGB_auto = 1;

char hex2char[17] = "0123456789ABCDEF";


void LCD_demo(void) {
    int i;
    int j;

    UB_LCD_2x16_String(0, 0, "0123456789ABCDEF");
    UB_LCD_2x16_String(0, 1, "GHIJKLMNOPQRSTUV");

    UB_LCD_2x16_String(0, 1, "STM32F4Discovery");

    UB_LCD_2x16_Clear();
    UB_LCD_2x16_String(0, 0, "LCD 2x16 STM32F4");
    UB_LCD_2x16_String(0, 1, "   GL Demo:     ");

    UB_LCD_2x16_Clear();
    UB_LCD_2x16_String(6, 0, "Start");
    for(j=0; j<2*30; ++j) {
       // for(i=0; i<18000000; ++i);
       // delay_ms(500);
    }
    // UB_LCD_2x16_String(6, 0, "Stop ");
    // for(i=0; i<18000000; ++i) {
     //      ;
     //}

      UB_LCD_2x16_Clear();
      UB_LCD_2x16_String(0, 0, ">");
      UB_LCD_2x16_String(15, 1, "<");
      delay_ms(100);
      for(i=0; i<15; ++i) {
          UB_LCD_2x16_String(i, 0, " >");
          UB_LCD_2x16_String(14-i, 1, "< ");
          delay_ms(100);
      }

      UB_LCD_2x16_Clear();
      //UB_LCD_2x16_String(0, 0, "LCD 2x16 STM32F4");
      //UB_LCD_2x16_String(2, 1, "GL Demo");
      UB_LCD_2x16_String(0, 0,   "GL Demo EMBEDDED");
      UB_LCD_2x16_String(0, 1,   "  STARTER KIT   ");


      for(j=0; j<3; ++j) {
          // LCD Backlight off
        GPIO_ResetBits(GPIOE, GPIO_Pin_9);
        delay_ms(500);
        GPIO_SetBits(GPIOE, GPIO_Pin_9);
        delay_ms(500);
      }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  char buf[64];
  int i;
  int j;
  uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
  uint8_t Rh_byte;
  uint8_t Temp_byte;
  uint8_t sum, sum_exp;
  int DHT_fails = 0;
  int DHT_decimator = 1;
  int RGB_decimator = 1;
  char cmds[] = "RGBKnA";
  int NCMD = sizeof cmds - 1;
  uint8_t ncmd = 0;
  unsigned char RGB_edit = 24;
  uint8_t up, down, left, right, ok;


  gpioa_init();
  gpioc_init();
  gpiod_init();
  gpioe_init();
  TIM3_init(); // used for delay implementation

  UB_LCD_2x16_Init();
  UB_LCD_2x16_Clear();

  gpiod_blink();

  // LCD Backlight on
  GPIO_SetBits(GPIOE, GPIO_Pin_9);

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  //HAL_Init();
  /* Configure the system clock */
  //SystemClock_Config();

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();

  //DWT_Delay_Init ();
  //HAL_Delay (1000);

  LCD_demo();

  RGB_demo1();
  RGB_demo2();
  RGB_demo3();

  for(i=0; ; ++i) {
      if (--RGB_decimator <= 0) {
          if (RGB_edit == 24) {
              RGB_LEDS_fill_all();
          }
          RGB_LEDS_show_all();

          if (RGB_auto) {  // change color
              RGB_edit = NLEDS;
              for (j=0; j<3; ++j) {
                             RGB[j][NLEDS] = (color & (1 << j)) ? color_level : 0;
                   }
              if (color_level > 252) {
                    color = color % 7 + 1;
              }
              color_level += 3;
          }
          RGB_decimator = 1;
      }

      if (--DHT_decimator <= 0) {
         DHT_start();
         if (DHT_check_response()) {
             Rh_byte1 = DHT_read_byte();
             Rh_byte2 = DHT_read_byte();
             Temp_byte1 = DHT_read_byte();
             Temp_byte2 = DHT_read_byte();
             sum = DHT_read_byte();
             sum_exp = Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2;
             if (sum == sum_exp) {
                 Rh_byte = Rh_byte1;
                 Temp_byte = Temp_byte1;
             } else {
                ++DHT_fails;  // checksum is wrong
             }
         }
         // show humidity and temperature
         sprintf(buf, "Rh:  %d%%      ", Rh_byte);
         buf[16] = '\0';
         UB_LCD_2x16_String(0, 0, buf);
         //sprintf(buf, "%d R:%d T:%d %d ", i, Rh_byte1, Temp_byte1, DHT_fails);
         sprintf(buf, "Temp:%dC  ", Temp_byte); //, DHT_fails);
         buf[12] = '\0';
         UB_LCD_2x16_String(0, 1, buf);
         DHT_decimator = 20;
      }

      if (0) {
        buf[0] = GPIOC->IDR & GPIO_Pin_9 ? ' ' : '<';
        buf[1] = GPIOC->IDR & GPIO_Pin_8 ? ' ' : 'v';
        buf[2] = GPIOA->IDR & GPIO_Pin_15 ? ' ' : '*';
        buf[3] = GPIOC->IDR & GPIO_Pin_6 ? ' ' : '^';
        buf[4] = GPIOC->IDR & GPIO_Pin_11 ? ' ' : '>';
        buf[5] = '\0';
      }

      up = !(GPIOC->IDR & GPIO_Pin_6);
      down = !(GPIOC->IDR & GPIO_Pin_8);
      left = !(GPIOC->IDR & GPIO_Pin_9);
      right = !(GPIOC->IDR & GPIO_Pin_11);
      ok = !(GPIOA->IDR & GPIO_Pin_15);

      if (up && down) {
          RGB_auto = !RGB_auto;
      }

      if (right) {
          ncmd = (ncmd + 1) % NCMD;
      }
      if (left) {
          ncmd = ncmd ? ncmd - 1 : NCMD - 1;
      }

      switch (cmds[ncmd]) {
      case 'n':
          if (up) RGB_edit = (RGB_edit % 24) + 1;
          if (down) RGB_edit = RGB_edit ? RGB_edit - 1 : 24;
          break;
      case 'A':
          if (up) {
              RGB[0][RGB_edit] = RGB[1][RGB_edit] = RGB[2][RGB_edit] = RGB[0][RGB_edit] + 1;
          }
          if (down) {
              RGB[0][RGB_edit] = RGB[1][RGB_edit] = RGB[2][RGB_edit] = RGB[0][RGB_edit] - 1;
          }
          break;

      case 'R':
          if (up) {
              RGB[0][RGB_edit]++;
          }
          if (down) {
              RGB[0][RGB_edit]--;
          }
          break;
      case 'G':
          if (up) {
             RGB[1][RGB_edit]++;
          }
          if (down) {
             RGB[1][RGB_edit]--;
          }
          break;
      case 'B':
          if (up) {
             RGB[1][RGB_edit]++;
          }
          if (down) {
             RGB[1][RGB_edit]--;
          }
          break;
      case 'K':
          if (up) {
              RGB[0][RGB_edit]++;
              RGB[1][RGB_edit]++;
              RGB[2][RGB_edit]++;
          }
          if (down) {
              RGB[0][RGB_edit]--;
              RGB[1][RGB_edit]--;
              RGB[2][RGB_edit]--;
          }
          break;
      }

      if (up) {
          if (left)  RGB[0][RGB_edit]++;
          if (ok)    RGB[1][RGB_edit]++;
          if (right) RGB[2][RGB_edit]++;
      }
      if (down) {
          if (left)  --RGB[0][RGB_edit];
          if (ok)    --RGB[1][RGB_edit];
          if (right) --RGB[2][RGB_edit];
      }

      buf[0] = hex2char[RGB[RGB_edit][0] >> 4];
      buf[1] = hex2char[RGB[RGB_edit][0] & 15];
      buf[2] = hex2char[RGB[RGB_edit][1] >> 4];
      buf[3] = hex2char[RGB[RGB_edit][1] & 15];
      buf[4] = hex2char[RGB[RGB_edit][2] >> 4];
      buf[5] = hex2char[RGB[RGB_edit][2] & 15];
      buf[6] = '\0';

      UB_LCD_2x16_String(10, 1, buf);

      sprintf(buf, "%c%c%d ", (RGB_auto ? '.' : ' '), cmds[ncmd], RGB_edit);
      UB_LCD_2x16_String(12, 0, buf);
      delay_ms(50);
   }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


