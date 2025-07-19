#pragma once

#include <stdint.h>
#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"

typedef struct {
    int LED_NUM;
    rmt_channel_handle_t channel;
    rmt_encoder_handle_t* encoder;
} ws2812_handle_t;

esp_err_t new_ws2812_encoder(rmt_encoder_handle_t* ret_encoder);
esp_err_t new_ws2812(int _gpio_num, int _LED_NUM, rmt_encoder_handle_t* encoder, ws2812_handle_t* ret_ws2812);
esp_err_t display(uint8_t data[][3], ws2812_handle_t ws_2812);