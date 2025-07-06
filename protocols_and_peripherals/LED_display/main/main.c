#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define VX_INIT 1
#define VY_INIT 1
#define X_INIT 2
#define Y_INIT 1
#define X_END 7
#define Y_END 5
#define FPS 1
#define N 8

#define MOSI 23
#define CLK 18
#define CS 5

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

static int vx = VX_INIT;
static int vy = VY_INIT;
static int x = X_INIT;
static int y = Y_INIT;
static int TIMER_INTERVAL = 1000000 / FPS;

static SemaphoreHandle_t frame_semaphore;
static spi_device_handle_t spi;
static bool finished = false;

static void periodic_timer_callback(void* args) {
    BaseType_t high_task_wakeup = pdFALSE;
    xSemaphoreGiveFromISR(frame_semaphore, &high_task_wakeup);
}

void max7219_send(uint8_t addr, uint8_t data) {
    uint8_t send_data[2] = {addr, data};
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 16;
    t.tx_buffer = send_data;
    spi_device_transmit(spi, &t);
}
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

    max7219_send(0x0F, 0x00);
    max7219_send(0x0C, 0x01);
    max7219_send(0x0B, 0x07);
    max7219_send(0x0A, 0x0F);
    max7219_send(0x09, 0x00);

    for(uint8_t i = 1; i <= 8; i++) {
        max7219_send(i, 0x00);
    }
}

// TODO
static void update_frame() {
    printf("----------------\n");
    for(uint8_t row = 0; row < 8; row++) {
        uint8_t value = 0;
        for(uint8_t col = 0; col < 8; col++) {
            if(led_matrix[row][col]) {
                value |= (0x80 >> col);
            }
        }
        for(int i = 7; i >= 0; i--) {
            printf("%d", (value >> i) & 1);
        }
        printf("\n");

        max7219_send(row + 1, value);
    }
    printf("----------------\n");
}
static void calculate_next_frame() {
    led_matrix[x][y] = 0;
    x += vx;
    y += vy;
    vx = (x >= N - 1) ? -1 : (x <= 0 ? 1 : vx);
    vy = (y >= N - 1) ? -1 : (y <= 1 ? 1 : vy);
    led_matrix[x][y] = 1;
    if(x == X_END && y == Y_END) {
        finished = true;
    }
}

void app_main(void) {
    frame_semaphore = xSemaphoreCreateBinary();
    max7219_init();

    const esp_timer_create_args_t periodic_timer_args = {.callback = &periodic_timer_callback, .name = "frame_timer"};

    esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, TIMER_INTERVAL);

    while(1) {
        if(xSemaphoreTake(frame_semaphore, portMAX_DELAY)) {
            update_frame();
            if(finished) {
                break;
            }
            calculate_next_frame();
            printf("Frame update at %lld ms\n", esp_timer_get_time() / 1000);
        }
    }
}