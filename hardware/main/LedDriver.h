#include "ChannelHandle.h"

Color clear_code[101] = {{
    .green = 0,
    .red = 0,
    .blue = 0,
}};

class LedDriver {
  public:
    LedDriver() {
        i2c_bus_init(&i2c_bus);
    }
    void config(config_t* config_array, int _ch_num) {
        ch_num = _ch_num;
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].clean();
        }
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].config(config_array[i]);
            ch_start_point[i + 1] = ch_start_point[i] + channel_handle[i].get_led_num();
        }
    }
    const void write(Color* color) {
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].write(&color[ch_start_point[i]]);
        }
    }
    void part_test(int ch_idx, Color* color) {
        channel_handle[ch_idx].write(color);
    }
    void clear() {
        for(int i = 0; i < ch_num; i++) {
            channel_handle[i].write(clear_code);
        }
    }

  private:
    int ch_num = 0;
    int ch_start_point[48] = {};
    i2c_master_bus_handle_t i2c_bus = NULL;
    Channel_Handle channel_handle[48] = {};
};
