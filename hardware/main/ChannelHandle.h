#pragma once

extern "C" {
#include "config.h"
#include "pca9955/pca9955.h"
#include "pca9955/pca9955_config.h"
#include "ws2812b/ws2812b.h"
}

static int pca9955_counter = 0;

class Channel_Handle {
  public:
    void config(config_t config) {
        type = config.type;
        led_num = config.led_num;
        if(config.type == 1) {
            rmt_activate = true;
            new_ws2812b_channel(config.gpio_addr, &channel);
            new_ws2812b_encoder(&encoder);
        } else {
            pca_ch_index = config.pca_ch_index;
            addr = config.gpio_addr;
            i2c_master_get_bus_handle(I2C_NUM_0, &i2c_bus);
            i2c_device_config_t dev_cfg = {
                .dev_addr_length = I2C_ADDR_BIT_LEN_7,
                .device_address = addr,
                .scl_speed_hz = 100000,
            };
            i2c_master_bus_add_device(i2c_bus, &dev_cfg, &dev_handle);
            uint8_t cmd[] = {IREF_addr[3 * pca_ch_index], 0x80};
            i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
            cmd[0] = IREF_addr[3 * pca_ch_index + 1];
            i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
            cmd[0] = IREF_addr[3 * pca_ch_index + 2];
            i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);

            // uint8_t buffer[6] = {IREF_addr[3 * pca_ch_index], 0xFF, IREF_addr[3 * pca_ch_index + 1], 0xFF, IREF_addr[3 * pca_ch_index + 2], 0xFF};
            // i2c_master_transmit(dev_handle, buffer, sizeof(buffer), -1);
        }
    }
    void write(Color* color) {
        if(type == 1) {
            rmt_transmit(channel, encoder, color, led_num * 3, &tx_config);
        } else {
            // uint8_t buffer[6] = {PWM_addr[3 * pca_ch_index],
            //                      color[0].red,
            //                      PWM_addr[3 * pca_ch_index + 1],
            //                      color[0].green,
            //                      PWM_addr[3 * pca_ch_index + 2],
            //                      color[0].blue};
            // i2c_master_transmit(dev_handle, buffer, sizeof(buffer), -1);
            uint8_t cmd[] = {PWM_addr[3 * pca_ch_index], color[0].red};
            i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
            cmd[0] = PWM_addr[3 * pca_ch_index + 1];
            cmd[1] = color[0].green;
            i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
            cmd[0] = PWM_addr[3 * pca_ch_index + 2];
            cmd[1] = color[0].blue;
            i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
        }
    }
    void clean() {
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

    const int get_led_num() {
        return led_num;
    }

  private:
    bool rmt_activate = 0;
    bool i2c_activate = 0;
    uint8_t type : 1;
    uint8_t addr : 7;
    uint8_t pca_ch_index;
    uint8_t led_num;
    i2c_master_bus_handle_t i2c_bus;
    i2c_master_dev_handle_t dev_handle;
    rmt_channel_handle_t channel;
    rmt_encoder_handle_t encoder;
};