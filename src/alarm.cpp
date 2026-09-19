#include "alarm.h"

void Initialize_BUZZER_PIN(void) {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    ledc_channel_config_t channel_config = {
        .gpio_num = Buzzer_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 127,
        .hpoint = 0
    };

    ledc_channel_config(&channel_config);
    #pragma GCC diagnostic pop
}

AlarmState evaluateTemperature(float temperature) {
    if(temperature >= 30.0f) {
        return AlarmState::HIGH_TEMPERATURE;
    }
    
    if(temperature <= 18.0f) {
        return AlarmState::LOW_TEMPERATURE;
    }

    return AlarmState::NORMAL;
}

void Buzzer_State(AlarmState currentResult){
    switch(currentResult) {
        case AlarmState::NORMAL:
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            break;
        case AlarmState::LOW_TEMPERATURE:
        case AlarmState::HIGH_TEMPERATURE:
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 127);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            break;
    }
}