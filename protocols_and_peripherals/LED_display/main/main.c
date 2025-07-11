#include <stdio.h>
#include <string.h>
#include "driver/spi_master.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Config for initial velocity, initial position, end position, FPS, and matrix size
#define VX_INIT 1
#define VY_INIT -1
#define X_INIT 2
#define Y_INIT 6
#define X_END 1
#define Y_END 6
#define FPS 30
#define N 8

#define MOSI 23
#define CLK 18
#define CS 5

// LED matrix state
static uint8_t led_matrix[N][N] = {
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
};

// Motion and control variables
static int vx = VX_INIT;
static int vy = VY_INIT;
static int x = X_INIT;
static int y = Y_INIT;
static int frame_count = 0;
static int TIMER_INTERVAL = (int)(1000000.0 / FPS);  // Microseconds per frame

static SemaphoreHandle_t frame_semaphore;
static spi_device_handle_t spi;
static bool finished = false;

// Timer callback to give semaphore (notifies main loop to update)
static void periodic_timer_callback(void* args) {
    BaseType_t high_task_wakeup = pdFALSE;
    xSemaphoreGiveFromISR(frame_semaphore, &high_task_wakeup);
}

// Send data to MAX7219 (address and data byte)
void max7219_send(uint8_t addr, uint8_t data) {
    uint8_t send_data[2] = {addr, data};
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 16;
    t.tx_buffer = send_data;
    spi_device_transmit(spi, &t);
}

// Initialize MAX7219 display
void max7219_init() {
    spi_bus_config_t buscfg = {
        .mosi_io_num = MOSI,
        .miso_io_num = -1,
        .sclk_io_num = CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1 * 1000 * 1000,
        .spics_io_num = CS,
        .queue_size = 7,
    };
    spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(HSPI_HOST, &devcfg, &spi);

    max7219_send(0x0F, 0x00);  // Disable test mode
    max7219_send(0x0C, 0x01);  // Normal operation
    max7219_send(0x0B, 0x07);  // Scan all 8 digits
    max7219_send(0x0A, 0x0F);  // Max brightness
    max7219_send(0x09, 0x00);  // No decode mode

    // Clear all rows
    for(uint8_t i = 1; i <= 8; i++) {
        max7219_send(i, 0x00);
    }
}

// Send the current led_matrix to display, and print debug pattern
static void update_frame() {
    // Clear display first
    for(uint8_t i = 1; i <= 8; i++) {
        max7219_send(i, 0x00);
    }
    // printf("----------------\n");
    for(uint8_t row = 0; row < 8; row++) {
        uint8_t value = 0;
        for(uint8_t col = 0; col < 8; col++) {
            if(led_matrix[row][col]) {
                value |= (0x80 >> col);
            }
        }
        // // Print binary row pattern for debugging
        // for(int i = 7; i >= 0; i--) {
        //     printf("%d", (value >> i) & 1);
        // }
        // printf("\n");

        // Update MAX7219 row
        max7219_send(row + 1, value);
    }
    // printf("----------------\n");
}

// Calculate the next frame's LED position
static void calculate_next_frame() {
    led_matrix[y][x] = 0;  // Clear current position

    x += vx;
    y += vy;

    // Check horizontal boundary and bounce
    vx = (x == N - 1) ? -1 : (x == 1 ? 1 : vx);
    // Check vertical boundary and bounce
    vy = (y == N - 1) ? -1 : (y == 0 ? 1 : vy);

    led_matrix[y][x] = 1;  // Set new position

    // Check if reached final target
    if(x == X_END && y == Y_END) {
        finished = true;
    }
}

void app_main(void) {
    frame_semaphore = xSemaphoreCreateBinary();
    max7219_init();

    // blink start point
    led_matrix[Y_INIT][X_INIT] = 1;
    update_frame();
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_matrix[Y_INIT][X_INIT] = 0;
    update_frame();
    vTaskDelay(pdMS_TO_TICKS(1000));

    // blink end point
    led_matrix[Y_END][X_END] = 1;
    update_frame();
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_matrix[Y_END][X_END] = 0;
    update_frame();
    vTaskDelay(pdMS_TO_TICKS(1000));

    led_matrix[Y_INIT][X_INIT] = 1;

    // Create periodic timer
    const esp_timer_create_args_t periodic_timer_args = {.callback = &periodic_timer_callback, .name = "frame_timer"};
    esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, TIMER_INTERVAL);

    // start
    while(1) {
        if(xSemaphoreTake(frame_semaphore, portMAX_DELAY)) {
            update_frame();
            frame_count++;
            if(finished) {
                break;
            }
            calculate_next_frame();
            printf("Frame update at %lld ms\n", esp_timer_get_time() / 1000);
        }
    }

    printf("frame count: %d\n", frame_count);
    vTaskDelay(pdMS_TO_TICKS(2000));  // Hold for a while before clearing

    // Clear display when done
    for(uint8_t i = 1; i <= 8; i++) {
        max7219_send(i, 0x00);
    }
    printf("--------------end--------------\n");
}
