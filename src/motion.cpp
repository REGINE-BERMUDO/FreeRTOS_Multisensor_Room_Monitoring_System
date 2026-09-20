#include "motion.h"

static bool currentState = false;
static bool prevState = false;
static uint8_t eventState = 0;

const char *MOTION_MONITOR_TAG = "MOTION";

void Initialize_PIR_PIN(void) {
    gpio_config_t pin_config = {
        .pin_bit_mask = (1ULL << PIRPIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE        
    };

    gpio_config(&pin_config);
}

static void PIR_inactiveState(void) {
    if(currentState == true && prevState == false) {
        if(!eventState & EVENT_ACTIVE) { // If Inactive
            xEventGroupSetBits(stateEventGroup, EVENT_ACTIVE);
            if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
                ESP_LOGI(MOTION_MONITOR_TAG, "Motion Detected! State: Inactive to Active State");
                xSemaphoreGive(stateSemaphore);
            }
        } else { // If Already Active
            if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
                ESP_LOGI(MOTION_MONITOR_TAG, "Motion Detected!");
                xSemaphoreGive(stateSemaphore);
            }
        }
    }
}

static void PIR_activeConstant(TickType_t *lastTickState) {
    // If both currentState & prevState are active
    if(currentState == true) {
        xEventGroupSetBits(stateEventGroup, EVENT_MOTION);
        *lastTickState = xTaskGetTickCount();
    } else { // If currentState is inactive & prevstate is active
        xEventGroupClearBits(stateEventGroup, EVENT_MOTION);
    }
}

static void PIR_activeState(TickType_t *lastTickState) {
    if(eventState & EVENT_ACTIVE) {
        if((xTaskGetTickCount() - *lastTickState) > pdMS_TO_TICKS(15000)) {
            xEventGroupClearBits(stateEventGroup, EVENT_ACTIVE);
            if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
                ESP_LOGI(MOTION_MONITOR_TAG, "Timeout Reached! State: Active to Inactive State");
                xSemaphoreGive(stateSemaphore);
            }
        }
    }
}

void PIR_State(TickType_t *lastTickState) {
    currentState = gpio_get_level(PIRPIN);
    eventState = xEventGroupGetBits(stateEventGroup);

    PIR_inactiveState();
    PIR_activeConstant(lastTickState);
    PIR_activeState(lastTickState);
    prevState = currentState;
}


