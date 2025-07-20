#include <string.h>
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_table.c"
#include "ws2812/ws2812.h"

#define CHANNELS_NUMBER 8
#define WS2812_RESOLUTION 10000000

static const char* TAG = "ws2812_main";

void app_main(void) {
    ws2812_handle_t ws2812[CHANNELS_NUMBER];
    int gpio_num[8] = {5, 19, 21, 22, 23, 25, 26, 27};
    int led_num[8] = {50, 50, 50, 50, 50, 50, 50, 50};
    // int64_t end_time[8];
    for(int i = 0; i < CHANNELS_NUMBER; i++) {
        ESP_ERROR_CHECK(new_ws2812(gpio_num[i], led_num[i], &ws2812[i]));
    }

    for(int frame = 0; frame < FRAME_NUMBER; frame++) {
        // printf("--------------------------------------\n");
        // int64_t start = esp_timer_get_time();
        for(int i = 0; i < CHANNELS_NUMBER; i++) {
            display(ws2812_pixels[frame][i], ws2812[i]);
        }
        for(int i = 0; i < CHANNELS_NUMBER; i++) {
            rmt_tx_wait_all_done(ws2812[i].channel, portMAX_DELAY);
            // end_time[i] = esp_timer_get_time();
        }
        // for(int i = 0; i < CHANNELS_NUMBER; i++) {
        //     printf("channel %d end time: %lldus\n", i, end_time[i] - start);
        // }
        // printf("--------------------------------------\n");
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
