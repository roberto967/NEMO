#pragma once

#include <inttypes.h>
#include <iostream>

#include "driver/gpio.h"
#include "esp_log.h"

extern "C" {
#include "hx711.h"
}

float calibrate(hx711_t *dev, float known_mass, uint8_t times = 15);
int32_t tare(hx711_t *dev, uint8_t times);