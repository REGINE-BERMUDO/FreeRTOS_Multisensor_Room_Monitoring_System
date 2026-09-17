#ifndef SENSORS_H
#define SENSORS_H

/// DHT22 Sensor Definitions
#define FiftyMilliseconds 50
#define EightyMicroseconds 80
#define SeventyMicroseconds 70
#define TwentyEightMicroseconds 28

/// LDR Sensor Definitions
#define Dark  4063
#define Bright 32

/// DHT22 Sensor Definitions
#include <driver/gpio.h>
#include <freeRTOS/FreeRTOS.h>
#include <freeRTOS/task.h>
#include <esp_rom_sys.h>

/// LDR Sensor Definitions
#include <esp_adc/adc_oneshot.h>

/// DHT22 Sensor Function Prototypes
void Pin_DHT22_Init(gpio_num_t pin);
esp_err_t DHT22_send_receive_data(float *temperature, float *humidity);

/// LDR Sensor Function Prototypes
void Pin_LDR_Init();
esp_err_t LDR_receive_data(float *percent, int *raw_value);





#endif // SENSORS_H