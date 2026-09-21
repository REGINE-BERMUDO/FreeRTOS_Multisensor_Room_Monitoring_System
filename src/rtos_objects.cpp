#include "rtos_objects.h"

// FreeRTOS Queue Handlers
QueueHandle_t sensorQueue;
QueueHandle_t alarmQueue;
QueueHandle_t inputQueue;
QueueSetHandle_t displayQueue;
SemaphoreHandle_t stateSemaphore; // Semaphore to manage system states

EventGroupHandle_t stateEventGroup; // Event group to manage system states


void Initialize_FreeRTOS_Tasks(void){
    xTaskCreate(SensorTask, "Sensor Task", 4096, NULL, 2, NULL);
    xTaskCreate(DisplayTask, "Display Task", 4096, NULL, 1, NULL);
    xTaskCreate(InputTask, "Input Task", 2048, NULL, 3, NULL);
    xTaskCreate(AlarmTask, "Alarm Task", 2048, NULL, 2, NULL); 
    xTaskCreate(MotionTask, "Motion Task", 2048, NULL, 3, NULL);
    xTaskCreate(StateTask, "State Task", 2048, NULL, 3, NULL);
}

esp_err_t Initialize_FreeRTOS_Queues(void) {
    sensorQueue = xQueueCreate(SENSORQUEUELENGTH, sizeof(SensorData)); 
    alarmQueue = xQueueCreate(ALARMQUEUELENGTH, sizeof(SensorData));
    inputQueue = xQueueCreate(INPUTQUEUELENGTH, sizeof(uint8_t));
    stateSemaphore = xSemaphoreCreateMutex();

    if(sensorQueue == nullptr) {
        return ESP_ERR_TIMEOUT;
    }

    if(alarmQueue == nullptr) {
        return ESP_ERR_TIMEOUT;
    }
    if(inputQueue == nullptr) {
        return ESP_ERR_TIMEOUT;
    }

    displayQueue = xQueueCreateSet(SENSORQUEUELENGTH + INPUTQUEUELENGTH);
    if(displayQueue != NULL) {
        xQueueAddToSet(sensorQueue, displayQueue);
        xQueueAddToSet(inputQueue, displayQueue);
    } else {
        return ESP_ERR_TIMEOUT;
    }
    if(stateSemaphore == nullptr) {
        return ESP_ERR_TIMEOUT;
    }
    
    return ESP_OK;
}

esp_err_t Initialize_FreeRTOS_Semaphore_Event(void){
    stateEventGroup = xEventGroupCreate();
    if(stateEventGroup == nullptr) {
        return ESP_ERR_TIMEOUT;
    }
    xEventGroupSetBits(stateEventGroup, EVENT_ACTIVE); 
    return ESP_OK;
}
   