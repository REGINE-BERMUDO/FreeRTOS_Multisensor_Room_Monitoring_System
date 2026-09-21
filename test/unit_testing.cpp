#include <unity.h>
#include "alarm.h"

void setUp(void);
void tearDown(void);

// Below lower threshold (e.g., 15.0°C)
void temp_below_lower_threshold(void) {
    TEST_ASSERT_EQUAL(AlarmState::LOW_TEMPERATURE, evaluateTemperature(15.0f));
}

//exactly at lower threshold (e.g., 18.0°C)
void temp_at_lower_threshold(void) {
    TEST_ASSERT_EQUAL(AlarmState::LOW_TEMPERATURE, evaluateTemperature(18.0f));
}

// Within threshold (e.g., 20.0°C)
void temp_within_threshold(void) {
    TEST_ASSERT_EQUAL(AlarmState::NORMAL, evaluateTemperature(20.0f));
}

//Exactly at upper threshold (e.g., 30.0°C)
void temp_at_upper_threshold(void) {
    TEST_ASSERT_EQUAL(AlarmState::HIGH_TEMPERATURE, evaluateTemperature(30.0f));
}

// Above upper threshold (e.g., 35.0°C)
void temp_above_upper_threshold(void) {
    TEST_ASSERT_EQUAL(AlarmState::HIGH_TEMPERATURE, evaluateTemperature(35.0f));
}


extern "C" void app_main(void){
    UNITY_BEGIN();
    RUN_TEST(temp_below_lower_threshold);
    RUN_TEST(temp_at_lower_threshold);
    RUN_TEST(temp_within_threshold);
    RUN_TEST(temp_at_upper_threshold);
    RUN_TEST(temp_above_upper_threshold);
    UNITY_END();
}
