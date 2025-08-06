#include "ChannelHandle.hpp"

const static color_t clear_code[101] = {{
    .green = 0,
    .red = 0,
    .blue = 0,
}};

class LedDriver {
  public:
    LedDriver() {
        i2c_bus_init(&i2c_bus);
    }
    ~LedDriver() {
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].detach();
        }
        i2c_del_master_bus(i2c_bus);
    }

    void config(const led_config_t* config_array, const int _ch_num) {
        ch_num = _ch_num;
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].detach();
        }
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].config(config_array[i]);
        }
        reset();
    }
    void write(const color_t** color) {
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].write(color[i]);
        }
    }
    void part_test(const int ch_idx, const color_t* color) {
        channel_handle[ch_idx].write(color);
    }
    void reset() {
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].write(clear_code);
        }
    }

  private:
    int ch_num = 0;
    i2c_master_bus_handle_t i2c_bus = NULL;
    Channel_Handle channel_handle[48] = {};
};
