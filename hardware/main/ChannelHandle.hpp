#pragma once

extern "C" {
#include "pca9955/pca9955.h"
#include "pca9955/pca9955_config.h"
#include "ws2812b/ws2812b.h"
}
#include "config.hpp"

static rmt_transmit_config_t tx_config = {
    .loop_count = 0,
};

class Channel_Handle {
  public:
    void config(const led_config_t config) {
        type = config.type;
        led_count = config.led_count;
        if(config.type == 1) {
            rmt_activate = true;
            new_ws2812b_channel(config.gpio_or_addr, &channel);
            new_ws2812b_encoder(&encoder);
        } else {
            pca_channel = config.pca_channel;
            addr = config.gpio_or_addr;
            i2c_master_get_bus_handle(I2C_NUM_0, &i2c_bus);
            i2c_device_config_t dev_cfg = {
                .dev_addr_length = I2C_ADDR_BIT_LEN_7,
                .device_address = addr,
                .scl_speed_hz = 100000,
            };
            i2c_master_bus_add_device(i2c_bus, &dev_cfg, &dev_handle);
            uint8_t cmd[] = {IREFALL, (uint8_t)LED_MAXIMUM_BRIGHTNESS};
            i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
        }
    }
    void write(const color_t* color) {
        if(type == 1) {
            rmt_transmit(channel, encoder, color, led_count * 3, &tx_config);
        } else {
            uint8_t buffer[4] = {(uint8_t)(PWM_addr[3 * pca_channel] | 0x80), color[0].red, color[0].green, color[0].blue};
            i2c_master_transmit(dev_handle, buffer, sizeof(buffer), -1);
            i2c_master_bus_wait_all_done(i2c_bus, -1);
        }
    }
    void detach() {
        if(rmt_activate) {
            rmt_disable(channel);
            rmt_del_channel(channel);
            rmt_del_encoder(encoder);
            rmt_activate = false;
        }
        if(i2c_activate) {
            i2c_master_bus_rm_device(dev_handle);
            i2c_activate = false;
        }
    }

  private:
    bool rmt_activate = 0;
    bool i2c_activate = 0;
    uint8_t type : 1;
    uint8_t addr : 7;
    uint8_t pca_channel;
    uint8_t led_count;
    i2c_master_bus_handle_t i2c_bus;
    i2c_master_dev_handle_t dev_handle;
    rmt_channel_handle_t channel;
    rmt_encoder_handle_t encoder;
};
