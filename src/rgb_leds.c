
#include "stm32f4_discovery.h"
#include "delay.h"
#include "rgb_leds.h"

// https://www.instructables.com/id/How-to-Make-Proper-Rainbow-and-Random-Colors-With-/

const uint8_t lights[360]={
  0,   0,   0,   0,   0,   1,   1,   2,
  2,   3,   4,   5,   6,   7,   8,   9,
 11,  12,  13,  15,  17,  18,  20,  22,
 24,  26,  28,  30,  32,  35,  37,  39,
 42,  44,  47,  49,  52,  55,  58,  60,
 63,  66,  69,  72,  75,  78,  81,  85,
 88,  91,  94,  97, 101, 104, 107, 111,
114, 117, 121, 124, 127, 131, 134, 137,
141, 144, 147, 150, 154, 157, 160, 163,
167, 170, 173, 176, 179, 182, 185, 188,
191, 194, 197, 200, 202, 205, 208, 210,
213, 215, 217, 220, 222, 224, 226, 229,
231, 232, 234, 236, 238, 239, 241, 242,
244, 245, 246, 248, 249, 250, 251, 251,
252, 253, 253, 254, 254, 255, 255, 255,
255, 255, 255, 255, 254, 254, 253, 253,
252, 251, 251, 250, 249, 248, 246, 245,
244, 242, 241, 239, 238, 236, 234, 232,
231, 229, 226, 224, 222, 220, 217, 215,
213, 210, 208, 205, 202, 200, 197, 194,
191, 188, 185, 182, 179, 176, 173, 170,
167, 163, 160, 157, 154, 150, 147, 144,
141, 137, 134, 131, 127, 124, 121, 117,
114, 111, 107, 104, 101,  97,  94,  91,
 88,  85,  81,  78,  75,  72,  69,  66,
 63,  60,  58,  55,  52,  49,  47,  44,
 42,  39,  37,  35,  32,  30,  28,  26,
 24,  22,  20,  18,  17,  15,  13,  12,
 11,   9,   8,   7,   6,   5,   4,   3,
  2,   2,   1,   1,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0};

const uint8_t HSVlights[61] =
{0, 4, 8, 13, 17, 21, 25, 30, 34, 38, 42, 47, 51, 55, 59, 64, 68, 72, 76,
81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123, 127, 132, 136, 140, 144,
149, 153, 157, 161, 166, 170, 174, 178, 183, 187, 191, 195, 200, 204, 208,
212, 217, 221, 225, 229, 234, 238, 242, 246, 251, 255};

const uint8_t HSVpower[121] =
{0, 2, 4, 6, 8, 11, 13, 15, 17, 19, 21, 23, 25, 28, 30, 32, 34, 36, 38, 40,
42, 45, 47, 49, 51, 53, 55, 57, 59, 62, 64, 66, 68, 70, 72, 74, 76, 79, 81,
83, 85, 87, 89, 91, 93, 96, 98, 100, 102, 104, 106, 108, 110, 113, 115, 117,
119, 121, 123, 125, 127, 130, 132, 134, 136, 138, 140, 142, 144, 147, 149,
151, 153, 155, 157, 159, 161, 164, 166, 168, 170, 172, 174, 176, 178, 181,
183, 185, 187, 189, 191, 193, 195, 198, 200, 202, 204, 206, 208, 210, 212,
215, 217, 219, 221, 223, 225, 227, 229, 232, 234, 236, 238, 240, 242, 244,
246, 249, 251, 253, 255};



uint8_t RGB[3][NLEDS + 1];

volatile uint8_t RGB_LED_cnt;
uint16_t tm_leds;
uint8_t nleds = 24;

void RGB_LEDS_tx_bit(uint16_t d1, uint16_t d0) {
    GPIO_SetBits(GPIOD, GPIO_RGB_LEDS);
    Delay(d1);
    GPIO_ResetBits(GPIOD, GPIO_RGB_LEDS);
    Delay(d0);
}

// timings in this function depends on compiler options
// TODO: rewrite it using PWM channel
// 1 is  1 - 700ns 0 - 600ns
// 0 is  1 - 250ns 0 - 800ns
// period is 1.25 us +- 150ns

void RGB_LEDS_show_byte(uint8_t byte) {
    uint8_t bit;
    for (bit=0x80; bit; bit >>= 1) {
        GPIOD->BSRRL = GPIO_RGB_LEDS;
        if (byte & bit) {
            RGB_LED_cnt = 6;
            while(--RGB_LED_cnt) { ; }
            GPIOD->BSRRH = GPIO_RGB_LEDS;
            RGB_LED_cnt = 2;
            while(--RGB_LED_cnt) { ; }
            //RGB_LEDS_tx_bit(1, 5);
        } else {
            RGB_LED_cnt = 2;
            while(--RGB_LED_cnt){ ; }
            GPIOD->BSRRH = GPIO_RGB_LEDS;
            RGB_LED_cnt = 6;
            //RGB_LEDS_tx_bit(3, 3);
        }
        while(--RGB_LED_cnt){
            ;
        }

    }
}

void RGB_LEDS_fill_all(void) {
    int i, led;
    for(i=0; i<3; ++i) {
        for (led=0; led<NLEDS; ++led) {
           RGB[i][led] = RGB[i][NLEDS];
        }
    }
}

void RGB_LEDS_show_all(void) {
    uint16_t t0 = TIM_GetCounter(TIM3);
    uint8_t  led;
    // uint8_t  bit;
    for (led=0; led < NLEDS  &&  led < NLEDS; ++led) {
        RGB_LEDS_show_byte(RGB[0][led]);
        RGB_LEDS_show_byte(RGB[1][led]);
        RGB_LEDS_show_byte(RGB[2][led]);
    }
    tm_leds = TIM_GetCounter(TIM3) - t0;
}


// the real HSV rainbow
void trueHSV(byte LED, int angle)
{
    byte red, green, blue;

    if (angle<60) {red = 255; green = HSVlights[angle]; blue = 0;} else
    if (angle<120) {red = HSVlights[120-angle]; green = 255; blue = 0;} else
    if (angle<180) {red = 0, green = 255; blue = HSVlights[angle-120];} else
    if (angle<240) {red = 0, green = HSVlights[240-angle]; blue = 255;} else
    if (angle<300) {red = HSVlights[angle-240], green = 0; blue = 255;} else
                 {red = 255, green = 0; blue = HSVlights[360-angle];}
    setRGBpoint(LED, red, green, blue);
}

// the 'power-conscious' HSV rainbow
void powerHSV(byte LED, int angle)
{
    byte red, green, blue;
    if (angle<120) {red = HSVpower[120-angle]; green = HSVpower[angle]; blue = 0;} else
    if (angle<240) {red = 0;  green = HSVpower[240-angle]; blue = HSVpower[angle-120];} else
                   {red = HSVpower[angle-240]; green = 0; blue = HSVpower[360-angle];}
    setRGBpoint(LED, red, green, blue);
}



void RGB_demo1(void) {
    int  j, k, m, led;
    // RGB initial demo
    for(m=1; m<8; ++m) {
        for(k=1; k<=NLEDS; ++k) {
            for(j=0; j<3; ++j) {
                int lev = m & (1<<j) ? 30 : 0;
                for (led=0; led<k; ++led) {
                    RGB[j][led] = lev;
                }
            }
            RGB_LEDS_show_all();
            delay_ms(40);
        }
    }
}

void RGB_demo2(void) {
    int j, k, m, led;

    for(m=1; m<8; ++m) {
        for(k=1; k<=NLEDS; ++k) {
            for(j=0; j<3; ++j) {
                int lev = m & (1<<j) ? 255 : 0;
                for(led=0; led<=k; ++led) {
                    RGB[j][led] = lev & led;//lev * led / NLEDS;
                }
            }
            RGB_LEDS_show_all();
            delay_ms(40);
        }
    }
}



// sine brightness
void RGB_demo3(void) {
    int j, k, m, led;

    for(m=1; m<256; m <<= 1) {
        for(k=1; k<=NLEDS; ++k) {
            for(j=0; j<3; ++j) {
                int lev = 1<<m;
                for(led=0; led<=k; ++led) {
                    RGB[j][led] = lights[(led*360/NLEDS + 120*j) % 360] * m / 128; //lev * led / NLEDS;
                }
            }
            RGB_LEDS_show_all();
            delay_ms(40);
        }
    }
}


void RGB_demo4(void) {
    int j, k, m, led;

    for(m=1; m<256; m <<= 1) {
        for(k=1; k<=NLEDS; ++k) {
            for(j=0; j<3; ++j) {
                int lev = 1<<m;
                for(led=0; led<=k; ++led) {
                    RGB[j][led] = lights[(led*360/NLEDS + 120*j) % 360] * m / 128; //lev * led / NLEDS;
                }
            }
            RGB_LEDS_show_all();
            delay_ms(40);
        }
    }
}
