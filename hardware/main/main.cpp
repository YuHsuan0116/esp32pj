extern "C" void app_main();
#include "LedDriver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    LedDriver driver;
    driver.config(config, 3);

    driver.write(color);

    vTaskDelay(pdMS_TO_TICKS(1000));
    driver.clear();
}
