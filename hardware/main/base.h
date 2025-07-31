#pragma once
#include <stdint.h>
#include "esp_check.h"
#include "esp_err.h"

static const uint8_t clear_frame[101][3] = {{0, 0, 0}};

class Base {
  public:
    Base() {}
    virtual ~Base() {};
    virtual esp_err_t write(const uint8_t data[][3]) = 0;
    virtual esp_err_t clear() = 0;
};