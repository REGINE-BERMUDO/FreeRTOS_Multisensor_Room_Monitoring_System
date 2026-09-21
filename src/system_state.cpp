#include "system_state.h"

static EventBits_t bits_State;
static const char *SYSTEM_STATE_MONITOR_TAG = "SYSTEM STATE";

void system_State(bool *currentState, bool *currentMotion, uint32_t *elapsedTime, TickType_t *lastWakeTime) {
    bits_State = xEventGroupGetBits(stateEventGroup);
    if((bits_State & EVENT_ACTIVE) != 0) { 
        *currentState = true; // If EVENT_ACTIVE is true
    } else {
        *currentState = false; // If EVENT_ACTIVE is false
    }
    if((bits_State & EVENT_MOTION) != 0) {
        *lastWakeTime = xTaskGetTickCount();
        *currentMotion = true; // If EVENT_MOTION is true
    } else {
        *currentMotion = false; // If EVENT_MOTION is false
    }

    *elapsedTime = pdTICKS_TO_MS(xTaskGetTickCount() - *lastWakeTime); // Calculate elapsed time in milliseconds since the last motion event
}

bool evaluate_State(bool currentState, bool currentMotion, uint32_t elapsedTime) {
    if(currentState) {
        if(elapsedTime > FIFTEEN_MS_TIMEOUT) {
            return false;
        } else {
            return true;
        }
    } else {
        if(currentMotion) {
            return true;
        } else {
            return false;
        }
    }
}

void print_State(bool currentState, bool temporaryStateHolder) {
    if(currentState && !temporaryStateHolder) {
        xEventGroupClearBits(stateEventGroup, EVENT_ACTIVE);
        
        if (xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
            ESP_LOGI(SYSTEM_STATE_MONITOR_TAG, "Inactivity timeout (15s) reached! State changed: ACTIVE -> INACTIVE");
            xSemaphoreGive(stateSemaphore);
        }
    }
    
    if(!currentState && temporaryStateHolder) {
        xEventGroupSetBits(stateEventGroup, EVENT_ACTIVE);

        if (xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
            ESP_LOGI(SYSTEM_STATE_MONITOR_TAG, "Motion Detected! State changed: INACTIVE -> ACTIVE");
            xSemaphoreGive(stateSemaphore);
        }
    }
}