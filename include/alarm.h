#ifndef ALARM_H
#define ALARM_H

#define Buzzer_PIN 27

#include <driver/ledc.h>

enum class AlarmState {
    NORMAL,
    LOW_TEMPERATURE,
    HIGH_TEMPERATURE
};

void Initialize_BUZZER_PIN(void);
AlarmState evaluateTemperature(float temperature);
void Buzzer_State(AlarmState currentResult);

#endif