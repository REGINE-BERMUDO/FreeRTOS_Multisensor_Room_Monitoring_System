#ifndef INPUT_H
#define INPUT_H

#define CLK GPIO_NUM_32
#define DT GPIO_NUM_33

#include <esp_attr.h>
#include <driver/gpio.h>

// Enum to represent the different display modes
enum class DisplayMode {
    TEMPERATURE,
    HUMIDITY,
    LIGHT,
    MOTION
};

void Pin_CLK_DT_Init(void); // Function to initialize the CLK and DT pins for the rotary encoder
DisplayMode encoder_receive_data(void); // Function to receive data from the encoder and return the current display mode

#endif 