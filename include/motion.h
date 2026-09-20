#ifndef MOTION_H
#define MOTION_H

#define PIRPIN GPIO_NUM_19

#include <driver/gpio.h>
#include "rtos_objects.h"
#include <esp_log.h>

void Initialize_PIR_PIN(void);
void PIR_State(TickType_t *lastTickState);

#endif // MOTION_H