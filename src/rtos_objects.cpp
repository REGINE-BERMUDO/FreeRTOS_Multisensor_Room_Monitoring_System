#include "rtos_objects.h"

void Initialize_FreeRTOS_Tasks(void) {
    xTaskCreate(sensorTask, "Sensor Task", 4096, NULL, 1, NULL);
}