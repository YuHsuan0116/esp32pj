#include "ws2812driver.hpp"

static rmt_transmit_config_t tx_config = {
    .loop_count = 0,
};

esp_err_t ws2812Driver::config(const led_config_t config) {
    led_count = config.led_count;
    rmt_activate = true;
    new_channel(config.gpio_or_addr, &channel);
    new_encoder(&encoder);
    return ESP_OK;
}

esp_err_t ws2812Driver::write(const color_t* colors) {
    rmt_transmit(channel, encoder, colors, led_count * 3, &tx_config);
    return ESP_OK;
}

esp_err_t ws2812Driver::detach() {
    if(rmt_activate) {
        rmt_disable(channel);
        rmt_del_channel(channel);
        rmt_activate = false;
    }
    return ESP_OK;
}

esp_err_t ws2812Driver::wait_done() {
    rmt_tx_wait_all_done(channel, -1);
    return ESP_OK;
}