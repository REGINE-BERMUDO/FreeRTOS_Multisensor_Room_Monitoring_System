extern "C" {
    #include <esp_log.h>
}
#include <FreeRTOS/FreeRTOS.h>
#include <freeRTOS/task.h>
#include "sensors.h"

const char *MAIN_MONITOR_TAG = "MAIN_MONITOR";

void sensorTask(void *pvParameters) {
    Pin_DHT22_Init(GPIO_NUM_18); // Initialize DHT22 sensor on GPIO 18
    float temperature = 0.0f;
    float humidity = 0.0f;
    
    Pin_LDR_Init(); // Initialize LDR sensor on ADC channel 8
    float percent = 0.0f; // Variable to store the percentage of light intensity
    int raw_value = 0; // Variable to store the raw ADC value from the LDR sensor

    while(true) {
        esp_err_t result = DHT22_send_receive_data(&temperature, &humidity);
        if(result == ESP_OK) {
            ESP_LOGI(MAIN_MONITOR_TAG, "Temperature: %.2f°C, Humidity: %.2f%%", temperature, humidity); //  Log the temperature and humidity values from the DHT22 sensor
        } else {
            ESP_LOGE(MAIN_MONITOR_TAG, "Failed to read from DHT22 sensor. Error code: %d", result); // Log an error message if reading from the DHT22 sensor fails
        }

        esp_err_t ldr_result = LDR_receive_data(&percent, &raw_value); // Read data from LDR sensor
        if(ldr_result == ESP_OK) {
            ESP_LOGI(MAIN_MONITOR_TAG, "Percent: %.2f%% (Raw: %d)", percent, raw_value); // Log the percentage and raw value from the LDR sensor
        } else {
            ESP_LOGE(MAIN_MONITOR_TAG, "Failed to read from LDR sensor. Error code: %d", ldr_result); // Log an error message if reading from the LDR sensor fails  
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}

extern "C" void app_main() {
    ESP_LOGI(MAIN_MONITOR_TAG, "\nBCA152 FreeRTOS Multisensor Monitor\nSYSTEM STARTING...");
    xTaskCreate(sensorTask, "Sensor Task", 4096, NULL, 1, NULL);
}

