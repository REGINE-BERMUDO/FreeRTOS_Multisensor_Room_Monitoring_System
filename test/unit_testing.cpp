#include <unity.h>
#include "alarm.h"
#include "display.h"
#include "system_state.h"    

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

// Display update tests for clockwise and counterclockwise rotations
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

// System state evaluation tests
void system_active_notimeout(void){
    bool is_active = evaluate_State(true, false, 1000);
    TEST_ASSERT_TRUE(is_active);
}

void system_active_timeout(void){
    bool is_active = evaluate_State(true, false, 20000);
    TEST_ASSERT_FALSE(is_active);
}

void system_inactive_nomotion(void){
    bool is_active = evaluate_State(false, false, 1000);
    TEST_ASSERT_FALSE(is_active);
}

void system_inactive_motion(void){
    bool is_active = evaluate_State(false, true, 1000);
    TEST_ASSERT_TRUE(is_active);
}

// Main function to run all tests
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

    // System state evaluation tests
    RUN_TEST(system_active_notimeout);
    RUN_TEST(system_active_timeout);
    RUN_TEST(system_inactive_nomotion);
    RUN_TEST(system_inactive_motion);

    UNITY_END();
}