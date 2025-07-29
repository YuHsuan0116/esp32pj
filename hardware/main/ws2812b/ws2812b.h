#pragma once

#include <stdint.h>
#include <string.h>
#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"
#include "esp_check.h"
#include "esp_log.h"

typedef struct {
    int LED_NUM;
    rmt_channel_handle_t channel;
    rmt_encoder_handle_t encoder;
} ws2812b_handle_t;

esp_err_t new_ws2812b(int _gpio_num, int _LED_NUM, ws2812b_handle_t* ret_ws2812b);
esp_err_t ws2812b_write(uint8_t data[][3], ws2812b_handle_t ws2812b);