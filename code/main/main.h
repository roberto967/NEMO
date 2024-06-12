#pragma once

#include <inttypes.h>
#include <iostream>

#include "driver/gpio.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

extern "C" {
#include "hx711.h"
#include "iot_servo.h"
#include "ultrasonic.h"
}