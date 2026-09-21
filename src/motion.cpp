#include "motion.h"

static bool currentState = false;
static bool prevState = false;

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

void PIR_State(void) {
    currentState = gpio_get_level(PIRPIN);

    if(currentState) {
        xEventGroupSetBits(stateEventGroup, EVENT_MOTION);

        if(prevState == false) {
            if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
                ESP_LOGI(MOTION_MONITOR_TAG, "Motion Sensor Triggered!");
                xSemaphoreGive(stateSemaphore);
            }
        }
    } else {
        xEventGroupClearBits(stateEventGroup, EVENT_MOTION);
    }

    prevState = currentState;
}