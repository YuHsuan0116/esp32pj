#pragma once

#include <stdint.h>
#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"

typedef struct {
    uint32_t resolution;
} ws2812_encoder_config_t;

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t* bytes_encoder;
    rmt_encoder_t* copy_encoder;
    int state;
    rmt_symbol_word_t reset_code;
} ws2812_encoder_t;

typedef struct {
    int LED_NUM;
    rmt_channel_handle_t channel;
    rmt_encoder_handle_t* encoder;
} ws2812_handle_t;

size_t encode_ws2812(rmt_encoder_t* encoder, rmt_channel_handle_t channel, const void* data, size_t data_size, rmt_encode_state_t* ret_state);
esp_err_t new_ws2812_encoder(rmt_encoder_handle_t* ret_encoder);
esp_err_t new_ws2812(int _gpio_num, int _LED_NUM, rmt_encoder_handle_t* encoder, ws2812_handle_t* ret_ws2812);
esp_err_t display(uint8_t data[][3], ws2812_handle_t ws_2812);