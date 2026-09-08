#include "telemetry.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

#include "esp_crt_bundle.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "freertos/queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi.h"

#define TELEMETRY_ENDPOINT_URL "https://gautenybo.no/esp32.php"
#define TELEMETRY_HTTP_TIMEOUT_MS 10000
#define TELEMETRY_TASK_STACK_SIZE 8192
#define TELEMETRY_TASK_PRIORITY 4
#define TELEMETRY_QUEUE_LENGTH 1

static const char *TAG = "telemetry";

static QueueHandle_t telemetry_queue;
static int64_t s_last_post_duration_ms = -1;

static void format_mac_string(char *out, size_t out_size)
{
    uint8_t mac[6];

    if (esp_read_mac(mac, ESP_MAC_WIFI_STA) != ESP_OK) {
        snprintf(out, out_size, "00:00:00:00:00:00");
        return;
    }

    snprintf(out,
             out_size,
             "%02x:%02x:%02x:%02x:%02x:%02x",
             mac[0],
             mac[1],
             mac[2],
             mac[3],
             mac[4],
             mac[5]);
}

static void format_ip_string(char *out, size_t out_size)
{
    esp_netif_t *netif;
    esp_netif_ip_info_t ip_info;

    netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif != NULL && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
        snprintf(out, out_size, IPSTR, IP2STR(&ip_info.ip));
        return;
    }

    snprintf(out, out_size, "0.0.0.0");
}

static void format_wifi_json_fields(char *out, size_t out_size)
{
    wifi_ap_record_t ap_info;

    if (wifi_is_up() && esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        snprintf(out,
                 out_size,
                 "\"ssid\":\"%s\",\"bssid\":\"%02x:%02x:%02x:%02x:%02x:%02x\",\"rssi\":%d,\"channel\":%u",
                 (char *) ap_info.ssid,
                 ap_info.bssid[0],
                 ap_info.bssid[1],
                 ap_info.bssid[2],
                 ap_info.bssid[3],
                 ap_info.bssid[4],
                 ap_info.bssid[5],
                 ap_info.rssi,
                 ap_info.primary);
        return;
    }

    snprintf(out, out_size, "\"ssid\":\"\",\"bssid\":\"\",\"rssi\":0,\"channel\":0");
}

static void telemetry_post(const struct telemetry_snapshot *snapshot)
{
    esp_http_client_config_t http_cfg;
    esp_http_client_handle_t client;
    char payload[512];
    char mac[18];
    char ip[16];
    char wifi_fields[96];
    int payload_len;
    esp_err_t err;
    int status_code;
    int64_t start_us;
    int64_t elapsed_ms;

    if (snapshot == NULL) {
        return;
    }

    format_mac_string(mac, sizeof(mac));
    format_ip_string(ip, sizeof(ip));
    format_wifi_json_fields(wifi_fields, sizeof(wifi_fields));

    payload_len = snprintf(payload,
                           sizeof(payload),
                           "{"
                           "\"device\":\"esp32_docker_starter\","
                           "\"mac\":\"%s\","
                           "\"ip\":\"%s\","
                           "%s,"
                           "\"uptime_ms\":%" PRIu32 ","
                           "\"free_heap\":%" PRIu32 ","
                           "\"min_free_heap\":%" PRIu32 ","
                           "\"largest_free_block\":%zu,"
                           "\"internal_free_heap\":%zu,"
                           "\"main_stack_hwm_words\":%" PRIu32 ","
                           "\"main_stack_hwm_bytes\":%" PRIu32 ","
                           "\"wifi_up\":%s,"
                           "\"last_post_duration_ms\":%" PRIi64
                           "}",
                           mac,
                           ip,
                           wifi_fields,
                           snapshot->uptime_ms,
                           snapshot->free_heap,
                           snapshot->min_free_heap,
                           snapshot->largest_free_block,
                           snapshot->internal_free_heap,
                           snapshot->main_stack_hwm_words,
                           snapshot->main_stack_hwm_bytes,
                           snapshot->wifi_up ? "true" : "false",
                           snapshot->last_post_duration_ms);

    if (payload_len <= 0 || payload_len >= (int) sizeof(payload)) {
        ESP_LOGE(TAG, "payload formatting failed");
        return;
    }

    http_cfg = (esp_http_client_config_t) {
        .url = TELEMETRY_ENDPOINT_URL,
        .method = HTTP_METHOD_POST,
        .timeout_ms = TELEMETRY_HTTP_TIMEOUT_MS,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .keep_alive_enable = false,
    };

    client = esp_http_client_init(&http_cfg);
    if (client == NULL) {
        ESP_LOGE(TAG, "failed to create HTTP client");
        return;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, payload, payload_len);

    start_us = esp_timer_get_time();
    err = esp_http_client_perform(client);
    elapsed_ms = (esp_timer_get_time() - start_us) / 1000;
    s_last_post_duration_ms = elapsed_ms;
    if (err == ESP_OK) {
        status_code = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "posted telemetry: status=%d duration_ms=%" PRIi64, status_code, elapsed_ms);
    } else {
        ESP_LOGW(TAG, "telemetry post failed: %s duration_ms=%" PRIi64, esp_err_to_name(err), elapsed_ms);
    }

    esp_http_client_cleanup(client);
}

static void telemetry_task(void *arg)
{
    struct telemetry_snapshot snapshot;

    (void) arg;

    while (1) {
        if (xQueueReceive(telemetry_queue, &snapshot, portMAX_DELAY) == pdTRUE) {
            telemetry_post(&snapshot);
        }
    }
}

void telemetry_init(void)
{
    telemetry_queue = xQueueCreate(TELEMETRY_QUEUE_LENGTH, sizeof(struct telemetry_snapshot));
    assert(telemetry_queue != NULL);

    xTaskCreate(telemetry_task,
                "telemetry_task",
                TELEMETRY_TASK_STACK_SIZE,
                NULL,
                TELEMETRY_TASK_PRIORITY,
                NULL);
}

int64_t telemetry_last_post_duration_ms(void)
{
    return s_last_post_duration_ms;
}

void telemetry_poll(const struct telemetry_snapshot *snapshot)
{
    if (snapshot == NULL || telemetry_queue == NULL) {
        return;
    }

    xQueueOverwrite(telemetry_queue, snapshot);
}
