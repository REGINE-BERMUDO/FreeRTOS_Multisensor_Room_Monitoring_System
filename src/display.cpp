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