#pragma once

#include <stdbool.h>

void wifi_init(void);
void wifi_poll(void);
bool wifi_is_up(void);
