# WS2812 LED Strip Driver

This module provides a driver for controlling WS2812 RGB LED strips using the ESP32 RMT peripheral and a software-based RGB encoder.


## File Structure

```
ws2812/
├── ws2812.h             # Public API
├── ws2812.c             # Implementation
└── README.md            # This documentation
```

## API Usage

### Include Header

```c
#include "ws2812.h"
```

### Initialize WS2812 Strip

```c
ws2812_handle_t strip;
new_ws2812(GPIO_NUM_5, 8, &encoder, &strip);  // 8 LEDs on GPIO5
```

### Send Color Data

```c
uint8_t data[8][3] = {
    {0x10, 0x00, 0x00},  // Red, Green, Blue for LED 0
    {0x00, 0x10, 0x00},  // LED 1
    ...
};
display(data, strip);
```

---

## Function Reference

### `esp_err_t new_ws2812(int gpio_num, int led_num, rmt_encoder_handle_t* encoder, ws2812_handle_t* ret_ws2812)`

Initializes a WS2812 LED strip on the specified GPIO pin.

### `esp_err_t display(uint8_t data[][3], ws2812_handle_t ws2812)`

Displays GRB data on the specified LED strip.

---

## Notes

- Data must be sent in **RGB** order.
- Each LED update takes 30 µs; a full strip of 100 LEDs takes 3 ms + 50 µs reset.

---

## License

MIT License
