#pragma once

void OLED_init();
void OLED_refresh();

void OLED_drawRocketLaunch();
void OLED_drawFinder();
void OLED_drawCompass(int16_t x, int16_t y, float angle, float pitch, float yaw);