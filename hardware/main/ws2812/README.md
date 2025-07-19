# WS2812 LED Strip Driver

This module provides a driver for controlling WS2812 RGB LED strips using the ESP32 RMT peripheral and a software-based GRB encoder.

## Features

- Supports arbitrary number of WS2812 LEDs
- RMT-based waveform generation with precise timing
- Encodes data in GRB format, MSB-first
- Supports reset signal timing for proper latching
- Designed for use with ESP-IDF (v4.3+)

## File Structure

```
ws2812/
├── ws2812.h             # Public API
├── ws2812.c             # Implementation
└── README.md            # This documentation
```

## Data Format

Each LED accepts **24 bits** of color data in the order:

```
[G7:G0] [R7:R0] [B7:B0]
```

Data is sent **MSB-first** over a single GPIO pin using RMT. A **reset code** (low for >50µs) is required at the end.

---

## API Usage

### Include Header

```c
#include "ws2812.h"
```

### Create Encoder

```c
rmt_encoder_handle_t encoder;
new_ws2812_encoder(&encoder);
```

### Initialize WS2812 Strip

```c
ws2812_handle_t strip;
new_ws2812(GPIO_NUM_5, 8, &encoder, &strip);  // 8 LEDs on GPIO5
```

### Send Color Data

```c
uint8_t data[8][3] = {
    {0x10, 0x00, 0x00},  // Green, Red, Blue for LED 0
    {0x00, 0x10, 0x00},  // LED 1
    ...
};
display(data, strip);
```

---

## Function Reference

### `esp_err_t new_ws2812_encoder(rmt_encoder_handle_t* ret_encoder)`

Creates an encoder instance suitable for WS2812 timing.

### `esp_err_t new_ws2812(int gpio_num, int led_num, rmt_encoder_handle_t* encoder, ws2812_handle_t* ret_ws2812)`

Initializes a WS2812 LED strip on the specified GPIO pin.

### `esp_err_t display(uint8_t data[][3], ws2812_handle_t ws2812)`

Displays GRB data on the specified LED strip.

---

## Notes

- Data must be sent in **RGB** order.
- Each LED update takes ~1.25 µs; a full strip of 300 LEDs takes ~375 µs + 50 µs reset.
- Be sure to allocate enough memory blocks in RMT to hold full frame data.

---

## License

MIT License
