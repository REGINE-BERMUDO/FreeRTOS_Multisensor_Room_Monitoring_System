#include <sensors.h>

static gpio_num_t dht22_pin;
static uint8_t timeout;
static uint8_t data_buffer[5] = {0};
static portMUX_TYPE spinlock_mux = portMUX_INITIALIZER_UNLOCKED;

void Pin_DHT22_Init(gpio_num_t pin) {
    gpio_config_t pin_config = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&pin_config);
    dht22_pin = pin;
}

esp_err_t DHT22_send_receive_data(float *temperature, float *humidity) {
    ////////////////////CLEAR DATA BUFFER START////////////////////
    for(int i = 0; i < 5; i++) {
        data_buffer[i] = 0;
    }
    ////////////////////CLEAR DATA BUFFER END////////////////////

    ////////////////////MCU SEND SIGNAL START////////////////////
    // MCU Initial Pin (HIGH)
    gpio_set_level(dht22_pin, 1);

    // MCU Pin (LOW for 18ms)
    gpio_set_level(dht22_pin, 0);
    vTaskDelay(pdMS_TO_TICKS(18));

    // MCU Pin (High for 40µs)
    gpio_set_level(dht22_pin, 1);
    esp_rom_delay_us(40);
    ////////////////////MCU SEND SIGNAL END////////////////////

    ////////////////////DHT22 PULL SIGNAL DELAY START////////////////////
    timeout = 0;
    while(gpio_get_level(dht22_pin) != 0) {
        esp_rom_delay_us(1);
        if(++timeout > FiftyMilliseconds) {
            return ESP_ERR_TIMEOUT;
        }
    }
    ////////////////////DHT22 PULL DELAY END////////////////////

    ////////////////////DHT22 RESPONSE SIGNAL START////////////////////
    // DHT22 Pin (Low for 80µs)
    timeout = 0;
    while(gpio_get_level(dht22_pin) != 1) {
        esp_rom_delay_us(1);
        if(++timeout > EightyMicroseconds) {
            return ESP_ERR_TIMEOUT;
        }
    }

    // DHT22 Pin (High for 80µs)
    timeout = 0;
    while(gpio_get_level(dht22_pin) != 0) {
        esp_rom_delay_us(1);
        if(++timeout > EightyMicroseconds) {
            return ESP_ERR_TIMEOUT;
        }
    }
    ////////////////////DHT22 RESPONSE SIGNAL DELAY END////////////////////

    ////////////////////DHT22 SEND 40-BIT DATA START////////////////////
    portENTER_CRITICAL(&spinlock_mux); // Enter Critical Section
    for(int i = 0; i < 40; i++) {
        // DHT22 Pin (Low for 50µs)
        timeout = 0;
        while(gpio_get_level(dht22_pin) == 0) {
            esp_rom_delay_us(1);
            if(++timeout > FiftyMilliseconds) {
                portEXIT_CRITICAL(&spinlock_mux);
                return ESP_ERR_TIMEOUT;
            }
        }

        // DHT22 Pin (High Logic if 70µs or above)
        timeout = 0;
        while(gpio_get_level(dht22_pin) == 1) {
            esp_rom_delay_us(1);
            if(++timeout > SeventyMicroseconds) {
                portEXIT_CRITICAL(&spinlock_mux);
                return ESP_ERR_TIMEOUT;
            }
        }

        // DHT22 Pin (Low Logic if 28µs or Lower)
        if(timeout > TwentyEightMicroseconds) {
            data_buffer[i / 8] |= (1 << (7 - (i % 8)));
        }
    }
    portEXIT_CRITICAL(&spinlock_mux); // Enter Critical Section
    ////////////////////DHT22 SEND 40-BIT DATA END////////////////////

    ////////////////////MCU RECEIVED CHECK-SUM START////////////////////
    // data_buffer[0 - 3] truncated to 8-bit data, and compared with the 8-bit data checksum
    if((uint8_t)(data_buffer[0] + data_buffer[1] + data_buffer[2] + data_buffer[3]) != data_buffer[4]) {
        return ESP_ERR_TIMEOUT;
    }
    ////////////////////MCU RECEIVED CHECK-SUM END////////////////////

    ////////////////////COMBINE INTEGER & DECIMAL DATA TO A SINGLE 16-BIT INTEGER VARIABLE START////////////////////
    int16_t raw_humidity = (data_buffer[0] << 8) | data_buffer[1];
    int16_t raw_temperature = (data_buffer[2] << 8) | data_buffer[3];
    ////////////////////COMBINE INTEGER & DECIMAL DATA TO A SINGLE 16-BIT INTEGER VARIABLE END////////////////////

    ////////////////////NEGATIVE TEMPERATURE HANDLING START////////////////////
    if(raw_temperature & 0x8000) { // 1000-0000-0000-0000 16th-bit polarity-sign check 
        raw_temperature &= 0x7FFF;
        *temperature = ((float)raw_temperature / 10.0) * -1.0;
    } else {
        *temperature = ((float)raw_temperature / 10.0);
    }
    ////////////////////NEGATIVE TEMPERATURE HANDLING END////////////////////

    ////////////////////HUMIDITY START////////////////////
    *humidity = (float)raw_humidity / 10.0;
    ////////////////////HUMIDITY END////////////////////

    return ESP_OK;
}