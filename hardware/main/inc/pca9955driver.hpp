extern "C" {
#include "pca9955.h"
}
#include "config.hpp"

class pca9955Driver {
  public:
    esp_err_t config(const led_config_t config);
    esp_err_t write(const color_t* colors);
    esp_err_t detach();
    esp_err_t wait_done();

  private:
    bool i2c_activate = 0;
    uint8_t addr;
    uint8_t pca_channel;
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
};