#ifndef INPUT_H
#define INPUT_H

#define CLK GPIO_NUM_32
#define DT GPIO_NUM_33

#include <esp_attr.h>
#include <driver/gpio.h>

enum class DisplayMode {
    TEMPERATURE,
    HUMIDITY,
    LIGHT,
    MOTION
};

void Pin_CLK_DT_Init(void);
DisplayMode encoder_receive_data(void);

#endif 