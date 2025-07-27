#include "config.h"

#define CHANNELS_NUMBER 8

static const char* TAG = "ws2812_main";

void app_main(void) {
    ws2812b_handle_t ws2812b[CHANNELS_NUMBER];
    int gpio_num[8] = {4, 18, 16, 27, 17, 25, 5, 26};
    int led_num[8] = {8, 8, 8, 8, 8, 8, 8, 8};
    // int64_t end_time[8];
    for(int i = 0; i < CHANNELS_NUMBER; i++) {
        ESP_ERROR_CHECK(new_ws2812b(gpio_num[i], led_num[i], &ws2812b[i]));
    }

    for(int frame = 0; frame < FRAME_NUMBER; frame++) {
        printf("--------------------------------------\n");
        // int64_t start = esp_timer_get_time();
        for(int i = 0; i < CHANNELS_NUMBER; i++) {
            ws2812b_write(ws2812b_pixels[frame][i], ws2812b[i]);
        }
        for(int i = 0; i < CHANNELS_NUMBER; i++) {
            rmt_tx_wait_all_done(ws2812b[i].channel, portMAX_DELAY);
            // end_time[i] = esp_timer_get_time();
        }
        // for(int i = 0; i < CHANNELS_NUMBER; i++) {
        //     printf("channel %d end time: %lldus\n", i, end_time[i] - start);
        // }
        // printf("--------------------------------------\n");
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
