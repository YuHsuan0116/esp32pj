#include <string.h>
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_table.c"
#include "ws2812_encoder.h"

#define RMT_WS2812_RESOLUTION_HZ 10000000
#define RMT_WS2812_GPIO_NUM 5

static const char* TAG = "ws2812_main";

void app_main(void) {
    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_channel_handle_t tx_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = RMT_WS2812_GPIO_NUM,
        .mem_block_symbols = 64,
        .resolution_hz = RMT_WS2812_RESOLUTION_HZ,
        .trans_queue_depth = 4,
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &tx_chan));

    rmt_encoder_handle_t ws2812_encoder = NULL;
    ws2812_encoder_config_t encoder_config = {
        .resolution = RMT_WS2812_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_ws2812_encoder(&encoder_config, &ws2812_encoder));

    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(tx_chan));

    // ESP_LOGI(TAG, "Start LED rainbow chase");
    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
    };
    for(int frame = 0; frame < FRAME_NUMBER; frame++) {
        ESP_ERROR_CHECK(rmt_transmit(tx_chan, ws2812_encoder, ws2812_pixels[frame], sizeof(ws2812_pixels[frame]), &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(tx_chan, portMAX_DELAY));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
