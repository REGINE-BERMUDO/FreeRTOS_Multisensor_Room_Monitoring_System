#ifndef SENSORS_H
#define SENSORS_H

#include <esp_log.h> // ESP32 Logging Library


/// DHT22 Sensor Definitions
#define FiftyMilliseconds 50
#define EightyMicroseconds 80
#define SeventyMicroseconds 70
#define TwentyEightMicroseconds 28
#define DHT22PIN GPIO_NUM_18 // DHT22 Sensor Pin Configuration

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
esp_err_t DHT22_send_receive_data(float *temperature, float *humidity);

/// LDR Sensor Function Prototypes
esp_err_t LDR_receive_data(float *percent, int *raw_value);

// One-Call Function for DHT22 & LDR Configurations
void Initialize_DHT22_LDR_PINS(void);
void DHT22_Print(float *temperature, float *humidity);
void LDR_Print(float *percent, int *raw_value);




#endif // SENSORS_H