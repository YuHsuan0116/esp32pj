#include "pca9955.h"
#include "ws2812.h"

class LedDriver {
  public:
    LedDriver(int _gpio, int _led_num) {
        p = new WS2812(_gpio, _led_num);
    }
    LedDriver(i2c_master_bus_handle_t _i2c_bus, uint8_t _addr, int _channel) {
        p = new PCA9955(_i2c_bus, _addr, _channel);
    }
    ~LedDriver() {
        delete p;
    }

    esp_err_t write(const uint8_t data[][3]) {
        return p->write(data);
    }
    esp_err_t clear() {
        return p->clear();
    }

  private:
    Base* p;
};