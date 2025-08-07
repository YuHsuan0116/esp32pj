extern "C" {
#include "ws2812.h"
}
#include "config.hpp"

class ws2812Driver {
  public:
    esp_err_t config(const led_config_t config);
    esp_err_t write(const color_t* colors);
    esp_err_t detach();
    esp_err_t wait_done();

  private:
    bool rmt_activate = 0;
    uint8_t led_count;
    rmt_channel_handle_t channel;
    rmt_encoder_handle_t encoder;
};