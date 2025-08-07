#include "pca9955driver.hpp"

static const char* TAG = "pca9955driver.cpp";

// 1xxxxxxx: auto increment
static const uint8_t PWM_addr[5] = {0x88, 0x8B, 0x8E, 0x91, 0x94};

esp_err_t pca9955Driver::config(const led_config_t config) {
    pca_channel = config.pca_channel;
    addr = config.gpio_or_addr;
    i2c_activate = true;
    i2c_master_get_bus_handle(I2C_NUM_0, &bus_handle);
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = 100000,
    };
    i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);
    uint8_t cmd[] = {IREFALL, (uint8_t)MAXIMUM_BRIGHTNESS};
    i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
    return ESP_OK;
}

esp_err_t pca9955Driver::write(const color_t* colors) {
    uint8_t cmd[] = {PWM_addr[pca_channel], colors[0].red, colors[0].green, colors[0].blue};
    i2c_master_transmit(dev_handle, cmd, sizeof(cmd), -1);
    i2c_master_bus_wait_all_done(bus_handle, -1);
    return ESP_OK;
}

esp_err_t pca9955Driver::detach() {
    if(i2c_activate) {
        i2c_master_bus_rm_device(dev_handle);
        i2c_activate = false;
    }
    return ESP_OK;
}

esp_err_t pca9955Driver::wait_done() {
    return ESP_OK;
}