#pragma once

#include "driver/i2c_master.h"
#include "esp_check.h"
#include "esp_log.h"
#include "pca9955_config.h"

esp_err_t i2c_bus_init(i2c_master_bus_handle_t* i2c_bus);
