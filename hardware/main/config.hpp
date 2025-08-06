#pragma once

#include <stdint.h>

#define COLOR_GRB(g, r, b) {.green = (g / 4), .red = (r / 4), .blue = (b / 4)}
#define COLOR_RED COLOR_GRB(0, 255, 0)
#define COLOR_GREEN COLOR_GRB(255, 0, 0)
#define COLOR_BLUE COLOR_GRB(0, 0, 255)
#define LED_MAXIMUM_BRIGHTNESS 63

struct color_t {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
};

struct led_config_t {
    uint8_t type : 1;
    uint8_t led_count : 7;
    uint8_t gpio_or_addr;
    uint8_t pca_channel;
};

const static led_config_t config[4] = {
    {
        .type = 0,
        .led_count = 1,
        .gpio_or_addr = 0x5C,
        .pca_channel = 0,
    },
    {
        .type = 1,
        .led_count = 8,
        .gpio_or_addr = 4,
        .pca_channel = 0,
    },
    {
        .type = 1,
        .led_count = 8,
        .gpio_or_addr = 5,
        .pca_channel = 0,
    },
    {
        .type = 0,
        .led_count = 1,
        .gpio_or_addr = 0x5C,
        .pca_channel = 1,
    },
};

color_t test_green[8] = {COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN};
color_t test_red[8] = {COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED};
color_t test_blue[8] = {COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE};

color_t color0[10][1] = {
    {
        COLOR_GRB(255, 255, 255),
    },
    {
        COLOR_GRB(255, 0, 0),
    },
    {
        COLOR_GRB(255, 127, 0),
    },
    {
        COLOR_GRB(255, 255, 0),
    },
    {
        COLOR_GRB(0, 255, 0),
    },
    {
        COLOR_GRB(0, 255, 255),
    },
    {
        COLOR_GRB(0, 0, 255),
    },
    {
        COLOR_GRB(139, 0, 255),
    },
    {
        COLOR_GRB(255, 0, 127),
    },
    {
        COLOR_GRB(255, 0, 0),
    },
};
color_t color1[10][8] = {
    {
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
    },
    {
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
    },
    {
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
    },
    {
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
    },
    {
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
    },
    {
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
    },
    {
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
    },
    {
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
    },
    {
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
    },
    {
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
    },
};

color_t color2[10][8] = {
    {
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
    },
    {
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
    },
    {
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
    },
    {
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
    },
    {
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
    },
    {
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
    },
    {
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
    },
    {
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
    },
    {
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
        COLOR_GRB(255, 0, 0),
    },
    {
        COLOR_GRB(255, 0, 0),
        COLOR_GRB(255, 127, 0),
        COLOR_GRB(255, 255, 0),
        COLOR_GRB(0, 255, 0),
        COLOR_GRB(0, 255, 255),
        COLOR_GRB(0, 0, 255),
        COLOR_GRB(139, 0, 255),
        COLOR_GRB(255, 0, 127),
    },
};

const color_t* color_ptr[10][4] = {
    {color0[0], color1[0], color2[0], color0[1]},
    {color0[1], color1[1], color2[1], color0[2]},
    {color0[2], color1[2], color2[2], color0[3]},
    {color0[3], color1[3], color2[3], color0[4]},
    {color0[4], color1[4], color2[4], color0[5]},
    {color0[5], color1[5], color2[5], color0[6]},
    {color0[6], color1[6], color2[6], color0[7]},
    {color0[7], color1[7], color2[7], color0[8]},
    {color0[8], color1[8], color2[8], color0[9]},
    {color0[9], color1[9], color2[9], color0[0]},
};
