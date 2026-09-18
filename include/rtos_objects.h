#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#define SENSORQUEUELENGTH 5

#include <freeRTOS/FreeRTOS.h>
#include <freeRTOS/task.h>
#include "sensors.h"

extern QueueHandle_t sensorQueue;

// FreeRTOS Tasks Declaration
void sensorTask(void *pvParameters);

// One-Call Function for FreeRTOS Tasks Configuration
void Initialize_FreeRTOS_Tasks(void);

esp_err_t Initialize_FreeRTOS_Queues(void);

#endif