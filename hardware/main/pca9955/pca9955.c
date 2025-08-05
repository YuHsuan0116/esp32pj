#include "pca9955.h"

static i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_NUM_0,
    .scl_io_num = I2C_MASTER_SDL_IO,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
};

esp_err_t i2c_bus_init(i2c_master_bus_handle_t* i2c_bus) {
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, i2c_bus));
    return ESP_OK;
}