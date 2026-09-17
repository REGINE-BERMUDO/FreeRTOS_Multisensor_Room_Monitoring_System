#ifndef SENSORS_H
#define SENSORS_H

#define FiftyMilliseconds 50
#define EightyMicroseconds 80
#define SeventyMicroseconds 70
#define TwentyEightMicroseconds 28

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h" 
#include <driver/gpio.h>

void Pin_DHT22_Init(gpio_num_t pin);

esp_err_t DHT22_send_receive_data(float *temperature, float *humidity);













#endif // SENSORS_H

