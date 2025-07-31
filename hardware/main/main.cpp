extern "C" void app_main();
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_driver.h"
#include "led_table.h"

void app_main() {
    LedDriver driver(5, 8);
    for(int i = 0; i < FRAME_NUMBER; i++) {
        driver.write(ws2812b_pixels[i][0]);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
    driver.clear();
}
