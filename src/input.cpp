#include "input.h"

static DisplayMode currentMode = DisplayMode::TEMPERATURE;
volatile static bool state_changed = false;
volatile static int direction = 0;

static void right_clockwise(void) {
    switch(currentMode) {
        case DisplayMode::TEMPERATURE:
            currentMode = DisplayMode::HUMIDITY;
            break;
        case DisplayMode::HUMIDITY:
            currentMode = DisplayMode::LIGHT;
            break;
        case DisplayMode::LIGHT:
            currentMode = DisplayMode::MOTION;
            break;
        case DisplayMode::MOTION:
            currentMode = DisplayMode::TEMPERATURE;
            break;
    }
}

static void left_counterclockwise(void) {
        switch(currentMode) {
        case DisplayMode::TEMPERATURE:
            currentMode = DisplayMode::MOTION;
            break;
        case DisplayMode::MOTION:
            currentMode = DisplayMode::LIGHT;
            break;
        case DisplayMode::LIGHT:
            currentMode = DisplayMode::HUMIDITY;
            break;
        case DisplayMode::HUMIDITY:
            currentMode = DisplayMode::TEMPERATURE;
            break;
    }
}

static void IRAM_ATTR ISR_Handler(void *arg) {
    int check_DT = gpio_get_level(DT);

    if(check_DT == 0) { // Inverted (Might be Interrupt Delay Causing it to be Inverted)
        direction = 1; // Clockwise
    } else {
        direction = -1; // Counter Clockwise
    }

    state_changed = true;
}

void Pin_CLK_DT_Init(void) {
    gpio_config_t pin_CLK_config = {
        .pin_bit_mask = (1ULL << CLK),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };

    gpio_config_t pin_DT_config = {
        .pin_bit_mask = (1ULL << DT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&pin_CLK_config);
    gpio_config(&pin_DT_config);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(CLK, ISR_Handler, NULL);
}

// Function to receive data from the encoder and return the current display mode
DisplayMode encoder_receive_data(void) {
    if(state_changed) {
        if(direction == 1) {
            right_clockwise();
        } else if(direction == -1) {
            left_counterclockwise();
        }
        state_changed = false;
        direction = 0;
    }
    
    return currentMode;
}