#include "rtos_objects.h"
#include "sensors.h"
#include "display.h"
#include "input.h"

const char *MAIN_MONITOR_TAG = "MAIN_MONITOR";

void SensorTask(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount(); // Get the current tick count for task delay management

    Initialize_DHT22_LDR_PINS(); // Initialize DHT22 and LDR sensor pins
    float temperature = 0.0f;
    float humidity = 0.0f;
    
    float percent = 0.0f; // Variable to store the percentage of light intensity
    int raw_value = 0; // Variable to store the raw ADC value from the LDR sensor

    while(true) {
        esp_err_t DHT22_STATUS = DHT22_Print(&temperature, &humidity); // Get temperature and humidity from DHT22 sensor
        esp_err_t LDR_STATUS = LDR_Print(&percent, &raw_value); // Get light intensity percentage and raw ADC value from LDR sensor
        Queue_SENSORS_SEND_DATA(DHT22_STATUS, LDR_STATUS, &temperature, &humidity, &percent, &raw_value); // Send sensor data to the queue

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}

void DisplayTask(void *pvParameters) {
    Initialize_I2CFOR_SSD1306();
    SensorData displaydata_Received;
    char data_print[15];

    while(true) {
        if(xQueueReceive(sensorQueue, &displaydata_Received, portMAX_DELAY) == pdPASS) {
            snprintf(data_print, sizeof(data_print), "%.1f C", displaydata_Received.temperature);
            Display_Clear();
            Display_DrawText(0, 0, "ROOM MONITOR");
            Display_DrawText(0, 20, "Temperature");
            Display_DrawText(0, 32, data_print);
            Display_Show();
        }
    }
}

void InputTask(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();

    Pin_CLK_DT_Init();
    uint8_t prev_monitor_mode = 1;

    while(true) {
        uint8_t monitor_mode = (uint8_t)encoder_receive_data();
        if(monitor_mode != prev_monitor_mode) {
            ESP_LOGI(MAIN_MONITOR_TAG, "Encoder State: %d", monitor_mode);
            prev_monitor_mode = monitor_mode;
        }
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
    }
}

extern "C" void app_main() {
    ESP_LOGI(MAIN_MONITOR_TAG, "\nBCA152 FreeRTOS Multisensor Monitor\nSYSTEM STARTING...");

    Initialize_FreeRTOS_Queues(); // Initialize FreeRTOS queues    
    Initialize_FreeRTOS_Tasks(); // Initialize FreeRTOS tasks
}

