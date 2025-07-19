#include <string.h>
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_table.c"
#include "ws2812.h"

#define CHANNELS_NUMBER 2
#define WS2812_RESOLUTION 10000000

static const char* TAG = "ws2812_main";

void app_main(void) {
    rmt_encoder_handle_t ws2812_encoder;
    new_ws2812_encoder(&ws2812_encoder);

    ws2812_handle_t ws2812[CHANNELS_NUMBER];
    int gpio_num[CHANNELS_NUMBER] = {5, 19};
    int led_num[CHANNELS_NUMBER] = {8, 8};
    for(int i = 0; i < CHANNELS_NUMBER; i++) {
        ESP_ERROR_CHECK(new_ws2812(gpio_num[i], led_num[i], &ws2812_encoder, &ws2812[i]));
    }

    for(int frame = 0; frame < FRAME_NUMBER; frame++) {
        display(ws2812_pixels[frame], ws2812[0]);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
