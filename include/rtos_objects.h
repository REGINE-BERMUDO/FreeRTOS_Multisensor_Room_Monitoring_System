#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#define SENSORQUEUELENGTH 5


#include <freeRTOS/FreeRTOS.h>
#include <freeRTOS/task.h>
#include "sensors.h"

// FreeRTOS Queue Handlers
extern QueueHandle_t sensorQueue; 

// FreeRTOS Task Function Prototypes
void SensorTask(void *pvParameters);
void DisplayTask(void *pvParameters);
void InputTask(void *pvParameters); 

// FreeRTOS Queue & Tasks Initialization
void Initialize_FreeRTOS_Tasks(void);
esp_err_t Initialize_FreeRTOS_Queues(void);

#endif