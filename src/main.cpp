extern "C" {
    #include <esp_log.h>
}
#include <FreeRTOS/FreeRTOS.h>
#include <freeRTOS/task.h>



 const char *MAIN_MONITOR_TAG = "MAIN_MONITOR";

void taskA(void *pvParameters) {
    while(true) {
        ESP_LOGI(MAIN_MONITOR_TAG, "Task A is running.");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

void taskB(void *pvParameters) {
    while(true) {
        ESP_LOGI(MAIN_MONITOR_TAG, "Task B is running.");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

extern "C" void app_main() {
    ESP_LOGI(MAIN_MONITOR_TAG, "\nBCA152 FreeRTOS Multisensor Monitor\nSYSTEM STARTING...");
    xTaskCreate(taskA, "Dummy Task A", 2048, NULL, 1, NULL);
    xTaskCreate(taskB, "Dummy Task B", 2048, NULL, 1, NULL);
}