#include "driver/i2c.h"

// configuration
// install driver
// a. communication as master - handle communications
// b. communication as slave - respond to messages from the master
// interrupt handling
// customized configuration
// error handling
// delete driver

// configuration - setting i2c_config_t
// 1. mode of operation
// 2. communication pins
// 3. (master only) clock speed
// 4. (slave only) slave address

// configuration example (master)
int i2c_master_port = 0;
i2c_config_t conf_master = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_MASTER_SDA_IO,  // select GPIO specific to your project
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = I2C_MASTER_SCL_IO,  // select GPIO specific to your project
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = I2C_MASTER_FREQ_HZ,  //select frequency specific to your project
};

// configuration example (slave)
int i2c_slave_port = I2C_SLAVE_NUM;
i2c_config_t conf_slave = {
    .mode = I2C_MODE_SLAVE,
    .sda_io_num = I2C_SLAVE_SDA_IO,  // select GPIO specific to your project
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = I2C_SLAVE_SCL_IO,  // select GPIO specific to your project
    .slave.addr_10bit_en = 0,
    .slave.slave_addr = ESP_SLAVE_ADDR,
};

int main() {}