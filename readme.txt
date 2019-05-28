/*******************************************************************************
  * @file    STM32_LCD_Demo/readme.txt 
  * @author  Yevgen Kovalyov
  * @version V1.0.1
  * @date    25-May-2019
  * @brief   Description of the GPIO IO Toggle example.
  ******************************************************************************
  
@par Example Description 

This example describes how to use buttons, LCD and Humidity and temperature sensor 
from GL Embedded Starter kit.

@par Directory contents 

  - STM32_LCD_Demo/stm32f4xx_conf.h    Library Configuration file
  - STM32_LCD_Demo/stm32f4xx_it.c      Interrupt handlers
  - STM32_LCD_Demo/stm32f4xx_it.h      Interrupt handlers header file
  - STM32_LCD_Demo/main.c              Main program
  - STM32_LCD_Demo/delay.c             Delay implementations
  - STM32_LCD_Demo/dht.c               DHT11 commnication
  - STM32_LCD_Demo/rgb_leds.c          Control RGB leds
  - STM32_LCD_Demo/system_stm32f4xx.c  STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F4xx Devices Revision A.

  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board

  - STM32F4-Discovery
    - LED4, LED3, LED5 and LED6 are connected respectively to PD.12, PD.13, PD.14 and PD.15.
      

@par How to use it ? 

In order to make the program work, you must do the following :
    - Open the Atollic TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other
    - Browse to TrueSTUDIO workspace directory.
    - Click on File->Import
    - select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project "STM32_LCD_Demo"
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
 */
