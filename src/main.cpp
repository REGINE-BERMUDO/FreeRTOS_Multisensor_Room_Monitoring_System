#include "rtos_objects.h"
#include "sensors.h"

const char *MAIN_MONITOR_TAG = "MAIN_MONITOR";

void sensorTask(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();

    Initialize_DHT22_LDR_PINS(); // Initialize DHT22 and LDR sensor pins
    float temperature = 0.0f;
    float humidity = 0.0f;
    
    float percent = 0.0f; // Variable to store the percentage of light intensity
    int raw_value = 0; // Variable to store the raw ADC value from the LDR sensor


    while(true) {
        esp_err_t DHT22_STATUS = DHT22_Print(&temperature, &humidity);
        esp_err_t LDR_STATUS = LDR_Print(&percent, &raw_value);
        Queue_SENSORS_SEND_DATA(DHT22_STATUS, LDR_STATUS, &temperature, &humidity, &percent, &raw_value);

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}

extern "C" void app_main() {
    ESP_LOGI(MAIN_MONITOR_TAG, "\nBCA152 FreeRTOS Multisensor Monitor\nSYSTEM STARTING...");

    Initialize_FreeRTOS_Queues(); // Initialize FreeRTOS queues    
    Initialize_FreeRTOS_Tasks(); // Initialize FreeRTOS tasks
}

