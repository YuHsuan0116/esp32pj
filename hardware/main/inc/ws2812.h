#pragma once

#include <stdint.h>
#include <string.h>
#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"
#include "esp_check.h"
#include "esp_log.h"

#define WS2812_RESOLUTION 10000000

typedef struct {
    uint32_t resolution;
} encoder_config_t;

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t* bytes_encoder;
    rmt_encoder_t* copy_encoder;
    int state;
    rmt_symbol_word_t reset_code;
} encoder_t;

esp_err_t new_encoder(rmt_encoder_handle_t* ret_encoder);
esp_err_t new_channel(int gpio, rmt_channel_handle_t* ret_channel);
size_t encode(rmt_encoder_t* encoder, rmt_channel_handle_t channel, const void* data, size_t data_size, rmt_encode_state_t* ret_state);
esp_err_t del_encoder(rmt_encoder_t* encoder);
esp_err_t encoder_reset(rmt_encoder_t* encoder);