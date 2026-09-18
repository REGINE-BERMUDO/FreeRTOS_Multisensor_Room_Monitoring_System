#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#include <FreeRTOS/FreeRTOS.h>
#include <freeRTOS/task.h>

void sensorTask(void *pvParameters); // Task function for reading sensor data

void Initialize_FreeRTOS_Tasks(void); // Function to initialize FreeRTOS tasks

#endif // RTOS_OBJECTS_H