#ifndef DISPLAY_H
#define DISPLAY_H

#define SDAPIN GPIO_NUM_21
#define SCLPIN GPIO_NUM_22

#include <driver/i2c_master.h>
#include <OLED/ssd1306.h>
#include "sensors.h"
#include "rtos_objects.h"
#include "input.h"

void Initialize_I2CFOR_SSD1306(void);

void Display_Clear(void);
void Display_DrawText(int x, int y, const char *text);
void Display_Show(void);
void Display_Process_Update(SensorData *display_dataReceived, uint8_t current_State, char *data_print, size_t data_print_size);
void Display_Update(uint8_t current_State, bool isActive, const char *data_print);

#endif