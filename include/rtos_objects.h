#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#define SENSORQUEUELENGTH 5
#define ALARMQUEUELENGTH 5
#define INPUTQUEUELENGTH 5

#define EVENT_ACTIVE BIT0 
#define EVENT_MOTION BIT1
#define EVENT_ALARM BIT2

#include <freeRTOS/FreeRTOS.h>
#include <freeRTOS/task.h>
#include "sensors.h"

// FreeRTOS Queue Handlers
extern QueueHandle_t sensorQueue;
extern QueueHandle_t alarmQueue; 
extern QueueHandle_t inputQueue;
extern QueueSetHandle_t displayQueue;
extern EventGroupHandle_t stateEventGroup; // Event group to manage system states

// FreeRTOS Task Function Prototypes
void SensorTask(void *pvParameters);
void DisplayTask(void *pvParameters);
void InputTask(void *pvParameters); 
void AlarmTask(void *pvParameters);
void MotionTask(void *pvParameters);


// FreeRTOS Queue & Tasks Initialization
void Initialize_FreeRTOS_Tasks(void);
esp_err_t Initialize_FreeRTOS_Queues(void);
esp_err_t Initialize_FreeRTOS_Semaphore_Event(void);

#endif