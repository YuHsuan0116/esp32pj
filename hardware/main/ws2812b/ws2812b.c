#include "ws2812b.h"
#define WS2812B_RESOLUTION 10000000
static const char* TAG = "ws2812.c";

typedef struct {
    uint32_t resolution;
} ws2812b_encoder_config_t;

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t* bytes_encoder;
    rmt_encoder_t* copy_encoder;
    int state;
    rmt_symbol_word_t reset_code;
} ws2812b_encoder_t;

size_t encode_ws2812b(rmt_encoder_t* encoder, rmt_channel_handle_t channel, const void* data, size_t data_size, rmt_encode_state_t* ret_state) {
    ws2812b_encoder_t* ws2812b_encoder = __containerof(encoder, ws2812b_encoder_t, base);
    rmt_encoder_handle_t bytes_encoder = ws2812b_encoder->bytes_encoder;
    rmt_encoder_handle_t copy_encoder = ws2812b_encoder->copy_encoder;
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    rmt_encode_state_t state = RMT_ENCODING_RESET;
    size_t encoded_symbols = 0;
    switch(ws2812b_encoder->state) {
        case 0:  // send RGB data
            encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, data, data_size, &session_state);
            if(session_state & RMT_ENCODING_COMPLETE) {
                ws2812b_encoder->state = 1;
            }
            if(session_state & RMT_ENCODING_MEM_FULL) {
                state |= RMT_ENCODING_MEM_FULL;
                goto out;
            }
        case 1:  // send reset code
            encoded_symbols +=
                copy_encoder->encode(copy_encoder, channel, &ws2812b_encoder->reset_code, sizeof(ws2812b_encoder->reset_code), &session_state);
            if(session_state & RMT_ENCODING_COMPLETE) {
                ws2812b_encoder->state = RMT_ENCODING_RESET;
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

esp_err_t del_ws2812b_encoder(rmt_encoder_t* encoder) {
    ws2812b_encoder_t* ws2812b_encoder = __containerof(encoder, ws2812b_encoder_t, base);
    rmt_del_encoder(ws2812b_encoder->bytes_encoder);
    rmt_del_encoder(ws2812b_encoder->copy_encoder);
    free(ws2812b_encoder);
    return ESP_OK;
}

esp_err_t ws2812b_encoder_reset(rmt_encoder_t* encoder) {
    ws2812b_encoder_t* ws2812b_encoder = __containerof(encoder, ws2812b_encoder_t, base);
    rmt_encoder_reset(ws2812b_encoder->bytes_encoder);
    rmt_encoder_reset(ws2812b_encoder->copy_encoder);
    ws2812b_encoder->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

esp_err_t new_ws2812b_encoder(rmt_encoder_handle_t* ret_encoder) {
    ws2812b_encoder_config_t config = {
        .resolution = WS2812B_RESOLUTION,
    };
    ws2812b_encoder_t* ws2812b_encoder = NULL;
    ws2812b_encoder = rmt_alloc_encoder_mem(sizeof(ws2812b_encoder_t));
    ws2812b_encoder->base.encode = encode_ws2812b;
    ws2812b_encoder->base.del = del_ws2812b_encoder;
    ws2812b_encoder->base.reset = ws2812b_encoder_reset;
    rmt_bytes_encoder_config_t bytes_encoder_config = {
        .bit0 =
            {
                .level0 = 1,
                .duration0 = 0.4 * config.resolution / 1000000,
                .level1 = 0,
                .duration1 = 0.85 * config.resolution / 1000000,
            },
        .bit1 =
            {
                .level0 = 1,
                .duration0 = 0.8 * config.resolution / 1000000,
                .level1 = 0,
                .duration1 = 0.45 * config.resolution / 1000000,
            },
        .flags.msb_first = 1,
    };
    rmt_new_bytes_encoder(&bytes_encoder_config, &ws2812b_encoder->bytes_encoder);
    rmt_copy_encoder_config_t copy_encoder_config = {};
    rmt_new_copy_encoder(&copy_encoder_config, &ws2812b_encoder->copy_encoder);

    uint32_t reset_ticks = config.resolution / 1000000 * 50 / 2;
    ws2812b_encoder->reset_code = (rmt_symbol_word_t){
        .level0 = 0,
        .duration0 = reset_ticks,
        .level1 = 0,
        .duration1 = reset_ticks,
    };
    *ret_encoder = &ws2812b_encoder->base;
    return ESP_OK;
}

esp_err_t new_ws2812b(int _gpio_num, int _LED_NUM, ws2812b_handle_t* ret_ws2812b) {
    ret_ws2812b->channel = NULL;
    new_ws2812b_encoder(&(ret_ws2812b->encoder));
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = _gpio_num,
        .mem_block_symbols = 64,
        .resolution_hz = WS2812B_RESOLUTION,
        .trans_queue_depth = 1,
    };
    ESP_RETURN_ON_ERROR(rmt_new_tx_channel(&tx_chan_config, &ret_ws2812b->channel), TAG, "new_tx_channel failed");
    ESP_RETURN_ON_ERROR(rmt_enable(ret_ws2812b->channel), TAG, "enable failed");
    ret_ws2812b->LED_NUM = _LED_NUM;
    return ESP_OK;
}

static rmt_transmit_config_t tx_config = {
    .loop_count = 0,
};
esp_err_t ws2812b_write(uint8_t data[][3], ws2812b_handle_t ws2812b) {
    rmt_transmit(ws2812b.channel, ws2812b.encoder, data, ws2812b.LED_NUM * 3, &tx_config);
    // rmt_tx_wait_all_done(ws2812.channel, portMAX_DELAY);
    return ESP_OK;
}