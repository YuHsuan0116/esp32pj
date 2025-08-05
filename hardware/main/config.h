#pragma once

#include <stdint.h>
#include "driver/rmt_tx.h"

#define I2C_MASTER_SDL_IO 22
#define I2C_MASTER_SDA_IO 21

typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} Color;

typedef struct {
    uint8_t type : 1;     // 0: pca9955, 1: ws2812
    uint8_t led_num : 7;  // number of led
    uint8_t gpio_addr;    // gpio for ws2812 and addr for pca9955
    uint8_t pca_ch_index;
} config_t;

static rmt_transmit_config_t tx_config = {
    .loop_count = 0,
};

static config_t config[3] = {
    {
        .type = 0,
        .led_num = 1,
        .gpio_addr = 0x5C,
        .pca_ch_index = 0,
    },
    {
        .type = 1,
        .led_num = 4,
        .gpio_addr = 5,
    },
    {
        .type = 1,
        .led_num = 4,
        .gpio_addr = 4,
    },
};

static Color color[] = {
    {
        .green = 255,
        .red = 255,
        .blue = 255,
    },
    {
        .green = 255,
        .red = 0,
        .blue = 0,
    },
    {
        .green = 0,
        .red = 255,
        .blue = 0,
    },
    {
        .green = 0,
        .red = 0,
        .blue = 255,
    },
    {
        .green = 255,
        .red = 0,
        .blue = 0,
    },
    {
        .green = 0,
        .red = 255,
        .blue = 0,
    },
    {
        .green = 0,
        .red = 0,
        .blue = 255,
    },
    {
        .green = 255,
        .red = 0,
        .blue = 0,
    },
    {
        .green = 0,
        .red = 255,
        .blue = 0,
    },
};
