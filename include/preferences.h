#pragma once

typedef enum{
    SSD1306 = 0,
    SH1106 = 1
} OLED_driver_e;

typedef struct{
    int frequency;
    int id;
    OLED_driver_e oled_driver;
}config_data_t;


int preferences_init();
int preferences_save();

int preferences_get_frequency();
int preferences_get_id();
String preferences_get_OLEDdriver();

void preferences_update_frequency(int frequency);
void preferences_update_id(int id);

void preferences_update_OLEDdriver(String driver);