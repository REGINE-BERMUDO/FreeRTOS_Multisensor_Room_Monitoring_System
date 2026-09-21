#include <unity.h>
#include "alarm.h"
#include "display.h"    

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



void display_test_clockwise_temp(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::TEMPERATURE, data_print, sizeof(data_print));
}

void display_test_clockwise_humidity(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::HUMIDITY, data_print, sizeof(data_print));

}

void display_test_clockwise_light(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::LIGHT, data_print, sizeof(data_print));
}

void display_test_clockwise_motion(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::MOTION, data_print, sizeof(data_print));
}

void display_test_clockwise_temp1(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::TEMPERATURE, data_print, sizeof(data_print));
}

void display_test_counterclockwise_motion(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::MOTION, data_print, sizeof(data_print));
}

void display_test_counterclockwise_light(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::LIGHT, data_print, sizeof(data_print));
}

void display_test_counterclockwise_humidity(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::HUMIDITY, data_print, sizeof(data_print));
}

void display_test_counterclockwise_temp(void) {
    SensorData display_dataReceived = {25.0f, 50.0f, 300, true}; // Example sensor data
    char data_print[15];
    Display_Process_Update(&display_dataReceived, (uint8_t)DisplayMode::TEMPERATURE, data_print, sizeof(data_print));
}

extern "C" void app_main(void){
    UNITY_BEGIN();

    // Temperature threshold tests
    RUN_TEST(temp_below_lower_threshold);
    RUN_TEST(temp_at_lower_threshold);
    RUN_TEST(temp_within_threshold);
    RUN_TEST(temp_at_upper_threshold);
    RUN_TEST(temp_above_upper_threshold);

    // Clockwise tests
    RUN_TEST(display_test_clockwise_temp);
    RUN_TEST(display_test_clockwise_humidity);
    RUN_TEST(display_test_clockwise_light);
    RUN_TEST(display_test_clockwise_motion);
    RUN_TEST(display_test_clockwise_temp1);

    // Counterclockwise tests
    RUN_TEST(display_test_counterclockwise_motion);
    RUN_TEST(display_test_counterclockwise_light);
    RUN_TEST(display_test_counterclockwise_humidity);
    RUN_TEST(display_test_counterclockwise_temp);
    
    UNITY_END();
}