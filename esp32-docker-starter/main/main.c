#include <inttypes.h>

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "board.h"
#include "status.h"
#include "telemetry.h"
#include "wifi.h"

static const char *TAG = "main";
static TickType_t next_telemetry_tick;

void app_main(void)
{
    board_init();
    status_init();
    telemetry_init();
    next_telemetry_tick = xTaskGetTickCount() + pdMS_TO_TICKS(30000);

    ESP_LOGI(TAG, "project start");
    ESP_LOGI(TAG, "status LED on GPIO %d", (int) board_status_led_gpio());

    wifi_init();

    while (1) {
        TickType_t now = xTaskGetTickCount();

        status_poll();
        wifi_poll();

        if (now >= next_telemetry_tick) {
            struct telemetry_snapshot snapshot;
            UBaseType_t stack_hwm_words = uxTaskGetStackHighWaterMark(NULL);

            snapshot = (struct telemetry_snapshot) {
                .uptime_ms = (uint32_t) (now * portTICK_PERIOD_MS),
                .free_heap = esp_get_free_heap_size(),
                .min_free_heap = esp_get_minimum_free_heap_size(),
                .largest_free_block = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT),
                .internal_free_heap = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),
                .main_stack_hwm_words = (uint32_t) stack_hwm_words,
                .main_stack_hwm_bytes = (uint32_t) (stack_hwm_words * sizeof(StackType_t)),
                .wifi_up = wifi_is_up(),
                .last_post_duration_ms = telemetry_last_post_duration_ms(),
            };

            ESP_LOGI(TAG,
                     "telemetry: uptime_ms=%" PRIu32
                     " free_heap=%" PRIu32
                     " min_free_heap=%" PRIu32
                     " largest_free_block=%zu"
                     " internal_free_heap=%zu"
                     " main_stack_hwm_words=%" PRIu32
                     " main_stack_hwm_bytes=%" PRIu32
                     " wifi_up=%s",
                     snapshot.uptime_ms,
                     snapshot.free_heap,
                     snapshot.min_free_heap,
                     snapshot.largest_free_block,
                     snapshot.internal_free_heap,
                     snapshot.main_stack_hwm_words,
                     snapshot.main_stack_hwm_bytes,
                     snapshot.wifi_up ? "yes" : "no");
            telemetry_poll(&snapshot);
            next_telemetry_tick = now + pdMS_TO_TICKS(30000);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
