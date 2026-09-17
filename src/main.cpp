extern "C" {
    #include <esp_log.h>
}
 const char *MAIN_MONITOR_TAG = "MAIN_MONITOR";

extern "C" void app_main() {
    ESP_LOGI(MAIN_MONITOR_TAG, "\nBCA152 FreeRTOS Multisensor Monitor\nSYSTEM STARTING...");
}