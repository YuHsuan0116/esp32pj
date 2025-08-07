#include "channelhandle.hpp"

class LedDriver {
  public:
    LedDriver();
    ~LedDriver();
    esp_err_t config(const led_config_t* configs, const int _ch_num);
    esp_err_t write(const color_t** frame);
    esp_err_t part_test(const int ch_idx, const color_t* colors);
    esp_err_t reset();
    esp_err_t clear_frame();
    esp_err_t wait_all_done();

  private:
    int ch_num = -1;
    i2c_master_bus_handle_t bus_handle = NULL;
    ChannelHandle channel_handle[50] = {};
};