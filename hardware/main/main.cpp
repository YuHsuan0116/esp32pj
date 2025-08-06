extern "C" void app_main();
#include "LedDriver.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    LedDriver driver;
    driver.config(config, 4);

    driver.part_test(0, test_red);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(0, test_green);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(0, test_blue);
    vTaskDelay(pdMS_TO_TICKS(500));

    driver.part_test(1, test_red);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(1, test_green);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(1, test_blue);
    vTaskDelay(pdMS_TO_TICKS(500));

    driver.part_test(2, test_red);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(2, test_green);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(2, test_blue);
    vTaskDelay(pdMS_TO_TICKS(500));

    driver.part_test(3, test_red);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(3, test_green);
    vTaskDelay(pdMS_TO_TICKS(500));
    driver.part_test(3, test_blue);
    vTaskDelay(pdMS_TO_TICKS(500));

    vTaskDelay(pdMS_TO_TICKS(5000));
    for(int i = 0; i < 10; i++) {
        driver.write(color_ptr[i]);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    driver.reset();
    vTaskDelay(pdMS_TO_TICKS(100));
}
