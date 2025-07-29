extern "C" {
#include <stdint.h>
#include <string.h>
#include "esp_check.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ws2812b/ws2812b.h"
}

#include "led_table.cpp"
#define CHANNELS_NUMBER 8

class LED {
  public:
    esp_err_t init(int, int);
    esp_err_t write(uint8_t[][3]);
    esp_err_t wait_done();

  private:
    ws2812b_handle_t ws2812;
};

extern "C" void app_main() {
    LED ws2812;
    ws2812.init(4, 8);
    for(int frame = 0; frame < FRAME_NUMBER; frame++) {
        ws2812.write(ws2812b_pixels[frame][0]);
        ws2812.wait_done();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
};

esp_err_t LED::init(int _gpio_num, int _LED_NUM) {
    return new_ws2812b(_gpio_num, _LED_NUM, &this->ws2812);
}
esp_err_t LED::write(uint8_t data[][3]) {
    return ws2812b_write(data, this->ws2812);
}
esp_err_t LED::wait_done() {
    return rmt_tx_wait_all_done(ws2812.channel, portMAX_DELAY);
}