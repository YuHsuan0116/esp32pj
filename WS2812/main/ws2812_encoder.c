#include "ws2812_encoder.h"
#include "esp_check.h"

static const char* TAG = "ws2812_encoder";

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t* bytes_encoder;
    rmt_encoder_t* copy_encoder;
    int state;
    rmt_symbol_word_t reset_code;
} rmt_ws2812_encoder_t;

static size_t rmt_encode_ws2812(rmt_encoder_t* encoder,
                                rmt_channel_handle_t channel,
                                const void* primary_data,
                                size_t data_size,
                                rmt_encode_state_t* ret_state) {
    rmt_ws2812_encoder_t* ws2812_encoder = __containerof(encoder, rmt_ws2812_encoder_t, base);
    rmt_encoder_handle_t bytes_encoder = ws2812_encoder->bytes_encoder;
    rmt_encoder_handle_t copy_encoder = ws2812_encoder->copy_encoder;
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    rmt_encode_state_t state = RMT_ENCODING_RESET;
    size_t encoded_symbols = 0;
    switch(ws2812_encoder->state) {
        case 0:  // send RGB data
            encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, primary_data, data_size, &session_state);
            if(session_state & RMT_ENCODING_COMPLETE) {
                ws2812_encoder->state = 1;
            }
            if(session_state & RMT_ENCODING_MEM_FULL) {
                state |= RMT_ENCODING_MEM_FULL;
                goto out;
            }
        case 1:  // send reset code
            encoded_symbols +=
                copy_encoder->encode(copy_encoder, channel, &ws2812_encoder->reset_code, sizeof(ws2812_encoder->reset_code), &session_state);
            if(session_state & RMT_ENCODING_COMPLETE) {
                ws2812_encoder->state = RMT_ENCODING_RESET;
                state |= RMT_ENCODING_COMPLETE;
            }
            if(session_state & RMT_ENCODING_MEM_FULL) {
                state |= RMT_ENCODING_MEM_FULL;
                goto out;
            }
    }
out:
    *ret_state = state;
    return encoded_symbols;
}

static esp_err_t rmt_del_ws2812_encoder(rmt_encoder_t* encoder) {
    rmt_ws2812_encoder_t* ws2812_encoder = __containerof(encoder, rmt_ws2812_encoder_t, base);
    rmt_del_encoder(ws2812_encoder->bytes_encoder);
    rmt_del_encoder(ws2812_encoder->copy_encoder);
    free(ws2812_encoder);
    return ESP_OK;
}

static esp_err_t rmt_ws2812_encoder_reset(rmt_encoder_t* encoder) {
    rmt_ws2812_encoder_t* ws2812_encoder = __containerof(encoder, rmt_ws2812_encoder_t, base);
    rmt_encoder_reset(ws2812_encoder->bytes_encoder);
    rmt_encoder_reset(ws2812_encoder->copy_encoder);
    ws2812_encoder->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

esp_err_t rmt_new_ws2812_encoder(const ws2812_encoder_config_t* config, rmt_encoder_handle_t* ret_encoder) {
    esp_err_t ret = ESP_OK;
    rmt_ws2812_encoder_t* ws2812_encoder = NULL;
    ESP_GOTO_ON_FALSE(config && ret_encoder, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
    ws2812_encoder = rmt_alloc_encoder_mem(sizeof(rmt_ws2812_encoder_t));
    ESP_GOTO_ON_FALSE(ws2812_encoder, ESP_ERR_NO_MEM, err, TAG, "no mem for ws2812 encoder");
    ws2812_encoder->base.encode = rmt_encode_ws2812;
    ws2812_encoder->base.del = rmt_del_ws2812_encoder;
    ws2812_encoder->base.reset = rmt_ws2812_encoder_reset;
    rmt_bytes_encoder_config_t bytes_encoder_config = {
        .bit0 =
            {
                .level0 = 1,
                .duration0 = 0.35 * config->resolution / 1000000,
                .level1 = 0,
                .duration1 = 0.8 * config->resolution / 1000000,
            },
        .bit1 =
            {
                .level0 = 1,
                .duration0 = 0.7 * config->resolution / 1000000,
                .level1 = 0,
                .duration1 = 0.6 * config->resolution / 1000000,
            },
        .flags.msb_first = 1,
    };
    ESP_GOTO_ON_ERROR(rmt_new_bytes_encoder(&bytes_encoder_config, &ws2812_encoder->bytes_encoder), err, TAG, "create bytes encoder failed");
    rmt_copy_encoder_config_t copy_encoder_config = {};
    ESP_GOTO_ON_ERROR(rmt_new_copy_encoder(&copy_encoder_config, &ws2812_encoder->copy_encoder), err, TAG, "create copy encoder failed");

    uint32_t reset_ticks = config->resolution / 1000000 * 50 / 2;
    ws2812_encoder->reset_code = (rmt_symbol_word_t){
        .level0 = 0,
        .duration0 = reset_ticks,
        .level1 = 0,
        .duration1 = reset_ticks,
    };
    *ret_encoder = &ws2812_encoder->base;
    return ESP_OK;
err:
    if(ws2812_encoder) {
        if(ws2812_encoder->bytes_encoder) {
            rmt_del_encoder(ws2812_encoder->bytes_encoder);
        }
        if(ws2812_encoder->copy_encoder) {
            rmt_del_encoder(ws2812_encoder->copy_encoder);
        }
        free(ws2812_encoder);
    }
    return ret;
}