#include "display.h"

static i2c_master_bus_handle_t bus_handle = NULL;
static ssd1306_handle_t display_handle;

void Initialize_I2CFOR_SSD1306(void) {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    i2c_master_bus_config_t bus_cfg = {
    .i2c_port = I2C_NUM_0,
    .sda_io_num = SDAPIN,
    .scl_io_num = SCLPIN,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7
    };
    bus_cfg.flags.enable_internal_pullup = true,

    i2c_new_master_bus(&bus_cfg, &bus_handle);

    ssd1306_config_t cfg = {
        .bus = SSD1306_I2C,
        .width = 128,
        .height = 64
    };
    cfg.iface.i2c = {
        .port = I2C_NUM_0,
        .addr = 0x3C
    };

    ssd1306_new_i2c(&cfg, &display_handle);
    #pragma GCC diagnostic pop
}

void Display_Clear(void) {
    ssd1306_clear(display_handle);
}

void Display_DrawText(int x, int y, const char *text){
    ssd1306_draw_text(display_handle, x, y, text, true);
}

void Display_Show(void) {
    ssd1306_display(display_handle);
}

void Display_Process_Update(SensorData *display_dataReceived, uint8_t current_State, char *data_print, size_t data_print_size) {
    switch((DisplayMode)current_State) {
        case DisplayMode::TEMPERATURE:
            snprintf(data_print, data_print_size, "%.1f C", display_dataReceived->temperature);
            break;
        case DisplayMode::HUMIDITY:
            snprintf(data_print, data_print_size, "%.1f RH", display_dataReceived->humidity);     
            break;
        case DisplayMode::LIGHT:
            snprintf(data_print, data_print_size, "%d%%", display_dataReceived->lightLevel);
            break;
        case DisplayMode::MOTION:
            snprintf(data_print, data_print_size, "%s", display_dataReceived->motionDetected ? "DETECTED" : "CLEAR");
            break;
    }
}

void Display_Update(uint8_t current_State, bool isActive, char *data_print) {
    if(isActive) {
        Display_Clear();
        Display_DrawText(0, 0, "ROOM MONITOR");

        switch((DisplayMode)current_State) {
            case DisplayMode::TEMPERATURE:
                Display_DrawText(0, 20, "TEMPERATURE");
                break;
            case DisplayMode::HUMIDITY:
                Display_DrawText(0, 20, "HUMIDITY");     
                break;
            case DisplayMode::LIGHT:
                Display_DrawText(0, 20, "LIGHT");
                break;
            case DisplayMode::MOTION:
                Display_DrawText(0, 20, "MOTION");
                break;
        }
        Display_DrawText(0, 32, data_print);
        Display_Show();
    } else {
        Display_Clear();
        Display_Show();
    }
}