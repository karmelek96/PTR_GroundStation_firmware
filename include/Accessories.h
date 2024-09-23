#pragma once

#include <Arduino.h>

#define ADC_BATTERY_LEVEL_SAMPLES 5

void Accessories_init();
float Accessories_getVBat();