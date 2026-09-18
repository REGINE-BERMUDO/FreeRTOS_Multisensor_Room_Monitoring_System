#ifndef DISPLAY_H
#define DISPLAY_H

#define SDAPIN GPIO_NUM_21
#define SCLPIN GPIO_NUM_22

#include <driver/i2c_master.h>
#include <ssd1306.h>


void Initialize_I2CFOR_SSD1306(void);

void Display_Clear(void);

void Display_DrawText(int x, int y, const char *text);

void Display_Show(void);

#endif // DISPLAY_H