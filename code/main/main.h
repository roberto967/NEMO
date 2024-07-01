#pragma once

#include <inttypes.h>
#include <iostream>

#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_netif.h>
#include <esp_wifi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

extern "C" {
#include "hx711.h"
#include "iot_servo.h"
#include "ultrasonic.h"
#include "wifi_manager.h"
#include <stdio.h>
#include <string.h>
}