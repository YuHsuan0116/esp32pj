#pragma once

extern "C" {
#include "ws2812b/ws2812b.h"
}
#include "base.h"

static rmt_transmit_config_t tx_config = {
    .loop_count = 0,
};

class WS2812: public Base {
  public:
    WS2812(int _gpio, int _led_num) {
        gpio = _gpio;
        led_num = _led_num;
        new_ws2812b_encoder(&encoder);
        new_ws2812b_channel(gpio, &channel);
    }
    ~WS2812() {
        rmt_disable(channel);
        rmt_del_channel(channel);
        rmt_del_encoder(encoder);
    };

    esp_err_t write(const uint8_t data[][3]) {
        return rmt_transmit(channel, encoder, data, led_num * 3, &tx_config);
    }
    esp_err_t clear() {
        return write(clear_frame);
    };

  private:
    int gpio;
    int led_num;
    rmt_channel_handle_t channel;
    rmt_encoder_handle_t encoder;
};