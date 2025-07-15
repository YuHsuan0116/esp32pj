#pragma once

#include <stdint.h>
#include "driver/rmt_encoder.h"

typedef struct {
    uint32_t resolution;
} ws2812_encoder_config_t;

esp_err_t rmt_new_ws2812_encoder(const ws2812_encoder_config_t* config, rmt_encoder_handle_t* ret_encoder);