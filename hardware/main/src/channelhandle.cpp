#include "channelhandle.hpp"

esp_err_t ChannelHandle::config(const led_config_t config) {
    type = config.type;
    if(type == WS2812) {
        return ws2812.config(config);
    }
    if(type == PCA9955) {
        return pca9955.config(config);
    }
    return ESP_FAIL;
}

esp_err_t ChannelHandle::write(const color_t* colors) {
    if(type == WS2812) {
        return ws2812.write(colors);
    }
    if(type == PCA9955) {
        return pca9955.write(colors);
    }
    return ESP_FAIL;
}

esp_err_t ChannelHandle::detach() {
    return ESP_OK;
    if(type == WS2812) {
        return ws2812.detach();
    }
    if(type == PCA9955) {
        return pca9955.detach();
    }
}

esp_err_t ChannelHandle::wait_done() {
    if(type == WS2812) {
        return ws2812.wait_done();
    }
    if(type == PCA9955) {
        return pca9955.wait_done();
    }
    return ESP_FAIL;
}