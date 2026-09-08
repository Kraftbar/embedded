#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

struct telemetry_snapshot {
    uint32_t uptime_ms;
    uint32_t free_heap;
    uint32_t min_free_heap;
    size_t largest_free_block;
    size_t internal_free_heap;
    uint32_t main_stack_hwm_words;
    uint32_t main_stack_hwm_bytes;
    bool wifi_up;
    int64_t last_post_duration_ms;
};

void telemetry_init(void);
void telemetry_poll(const struct telemetry_snapshot *snapshot);
int64_t telemetry_last_post_duration_ms(void);
