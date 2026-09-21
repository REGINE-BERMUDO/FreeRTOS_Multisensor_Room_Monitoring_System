#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#define FIFTEEN_MS_TIMEOUT 15000

#include "rtos_objects.h"

void system_State(bool *currentState, bool *currentMotion, uint32_t *elapsedTime, TickType_t *lastWakeTime);
bool evaluate_State(bool currentState, bool currentMotion, uint32_t elapsedTime);
void print_State(bool currentState, bool temporaryStateHolder);

#endif // SYSTEM_STATE_H