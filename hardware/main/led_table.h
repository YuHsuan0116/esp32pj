#pragma once
#include <stdint.h>

#define FRAME_NUMBER 500
#define CHANNELS_NUMBER 8
#define LED_NUMBER 8

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t ws2812b_pixels[FRAME_NUMBER][CHANNELS_NUMBER][LED_NUMBER][3];

#ifdef __cplusplus
}
#endif
