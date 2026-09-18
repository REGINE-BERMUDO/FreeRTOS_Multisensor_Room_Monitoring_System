#include "rtos_objects.h"

// FreeRTOS Queue Handlers
QueueHandle_t sensorQueue;

void Initialize_FreeRTOS_Tasks(void) {
    xTaskCreate(sensorTask, "Sensor Task", 4096, NULL, 1, NULL);
}

esp_err_t Initialize_FreeRTOS_Queues(void) {
    sensorQueue = xQueueCreate(SENSORQUEUELENGTH, sizeof(SensorData));
    if(sensorQueue == nullptr) {
        return ESP_ERR_TIMEOUT;
    }

    return ESP_OK;
}