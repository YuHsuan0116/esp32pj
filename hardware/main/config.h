#pragma once

#include <stdint.h>
#include <string.h>
#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_table.c"
#include "ws2812b/ws2812b.h"

#define WS2812B_RESOLUTION 10000000
