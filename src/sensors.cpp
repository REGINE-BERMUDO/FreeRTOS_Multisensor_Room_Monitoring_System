#include "sensors.h"

static uint8_t timeout;
static uint8_t data_buffer[5] = {0};
static portMUX_TYPE spinlock_mux = portMUX_INITIALIZER_UNLOCKED;

static void Pin_DHT22_Init() {
    gpio_config_t pin_config = {
        .pin_bit_mask = (1ULL << DHT22PIN),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&pin_config);
}

esp_err_t DHT22_send_receive_data(float *temperature, float *humidity) {
    ////////////////////CLEAR DATA BUFFER START////////////////////
    for(int i = 0; i < 5; i++) {
        data_buffer[i] = 0;
    }
    ////////////////////CLEAR DATA BUFFER END////////////////////

    ////////////////////MCU SEND SIGNAL START////////////////////
    // MCU Initial Pin (HIGH)
    gpio_set_level(DHT22PIN, 1);

    // MCU Pin (LOW for 18ms)
    gpio_set_level(DHT22PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(18));

    // MCU Pin (High for 40µs)
    gpio_set_level(DHT22PIN, 1);
    esp_rom_delay_us(40);
    ////////////////////MCU SEND SIGNAL END////////////////////

    ////////////////////DHT22 PULL SIGNAL DELAY START////////////////////
    timeout = 0;
    while(gpio_get_level(DHT22PIN) != 0) {
        esp_rom_delay_us(1);
        if(++timeout > FiftyMilliseconds) {
            return ESP_ERR_TIMEOUT;
        }
    }
    ////////////////////DHT22 PULL DELAY END////////////////////

    ////////////////////DHT22 RESPONSE SIGNAL START////////////////////
    // DHT22 Pin (Low for 80µs)
    timeout = 0;
    while(gpio_get_level(DHT22PIN) != 1) {
        esp_rom_delay_us(1);
        if(++timeout > EightyMicroseconds) {
            return ESP_ERR_TIMEOUT;
        }
    }

    // DHT22 Pin (High for 80µs)
    timeout = 0;
    while(gpio_get_level(DHT22PIN) != 0) {
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
        while(gpio_get_level(DHT22PIN) == 0) {
            esp_rom_delay_us(1);
            if(++timeout > FiftyMilliseconds) {
                portEXIT_CRITICAL(&spinlock_mux);
                return ESP_ERR_TIMEOUT;
            }
        }

        // DHT22 Pin (High Logic if 70µs or above)
        timeout = 0;
        while(gpio_get_level(DHT22PIN) == 1) {
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





static adc_oneshot_unit_handle_t LDRHANDLE;

static void Pin_LDR_Init() {
    adc_oneshot_unit_init_cfg_t pin_config = {
        .unit_id = ADC_UNIT_2,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE
    };
    adc_oneshot_new_unit(&pin_config, &LDRHANDLE);

    adc_oneshot_chan_cfg_t pin_channel = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12
    };
    adc_oneshot_config_channel(LDRHANDLE, ADC_CHANNEL_8, &pin_channel);
}

esp_err_t LDR_receive_data(float *percent, int *raw_value) {
    *raw_value = 0;
    *percent = 0;

    if(adc_oneshot_read(LDRHANDLE, ADC_CHANNEL_8, raw_value) == ESP_OK) {
        //Range-Fix
        if(*raw_value > Dark) *raw_value = Dark;
        if(*raw_value < Bright) *raw_value = Bright;

        // Conversion & Inversion
        *percent = ((Dark - *raw_value) * 100.0f / (Dark - Bright));
    }

    return ESP_OK;
}





const char *SENSOR_MONITOR_TAG = "SENSOR";

// One-Call Function for DHT22 & LDR Configurations
void Initialize_DHT22_LDR_PINS(void) {
    Pin_DHT22_Init();
    Pin_LDR_Init();
}

// One-Call Function for DHT22 Print
esp_err_t DHT22_Print(float *temperature, float *humidity) {
    esp_err_t dht22_result = DHT22_send_receive_data(temperature, humidity);

    if(dht22_result == ESP_OK) {
        if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
            ESP_LOGI(SENSOR_MONITOR_TAG, "Temperature: %.2f°C, Humidity: %.2f%%", *temperature, *humidity);
            xSemaphoreGive(stateSemaphore);
        }
        return ESP_OK;
    } else {
        if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
            ESP_LOGE(SENSOR_MONITOR_TAG, "Failed to read from DHT22 sensor. Error code: %d", dht22_result);
            xSemaphoreGive(stateSemaphore);
        }
        return ESP_ERR_TIMEOUT;
    }   
 } 

// One-Call Function for LDR Print
esp_err_t LDR_Print(float *percent, int *raw_value) {
    esp_err_t ldr_result = LDR_receive_data(percent, raw_value);
    if(ldr_result == ESP_OK) {
        if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
            ESP_LOGI(SENSOR_MONITOR_TAG, "Percent: %.2f%% (Raw: %d)", *percent, *raw_value);
            xSemaphoreGive(stateSemaphore);
        }
        return ESP_OK;
    } else {
        if(xSemaphoreTake(stateSemaphore, portMAX_DELAY)) {
            ESP_LOGE(SENSOR_MONITOR_TAG, "Failed to read from LDR sensor. Error code: %d", ldr_result);
            xSemaphoreGive(stateSemaphore);
        }
        return ESP_ERR_TIMEOUT;
    }
}

// One-Call Function for Queueing Sensor Data
void Queue_SENSORS_SEND_DATA(esp_err_t DHT22STATUS, esp_err_t LDRSTATUS, const float *temperature, const float *humidity, const float *percent, int *raw_value) {
    SensorData readSensorDataQueue;

    if(DHT22STATUS == ESP_OK) {
        readSensorDataQueue.temperature = *temperature;
        readSensorDataQueue.humidity = *humidity;
    } else {
        readSensorDataQueue.temperature = 0.0f;
        readSensorDataQueue.humidity = 0.0f;
    }

    if(LDRSTATUS == ESP_OK) {
        readSensorDataQueue.lightLevel = *percent;
    } else {
        readSensorDataQueue.lightLevel = 0;
    }
    readSensorDataQueue.motionDetected = (xEventGroupGetBits(stateEventGroup) & EVENT_MOTION) ? true : false; // Check if motion is detected and set the motionDetected field accordingly

    xQueueSend(sensorQueue, &readSensorDataQueue, portMAX_DELAY);
    xQueueSend(alarmQueue, &readSensorDataQueue, portMAX_DELAY);
}