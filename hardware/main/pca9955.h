#include "base.h"
#include "driver/i2c_master.h"

class PCA9955: public Base {
  public:
    PCA9955(i2c_master_bus_handle_t _i2c_bus, uint8_t _addr, int _channel) {
        i2c_bus = _i2c_bus;
        addr = _addr;
        channel = _channel;
    }
    ~PCA9955() {}

    esp_err_t write(const uint8_t data[][3]) {
        return ESP_OK;
    }
    esp_err_t clear() {
        return ESP_OK;
    }

  private:
    i2c_master_bus_handle_t i2c_bus;
    uint8_t addr;
    int channel;
};