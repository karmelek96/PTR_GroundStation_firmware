#pragma once


typedef struct{
    int frequency;
    int id;
}config_data_t;


int preferences_init();
int preferences_save();

int preferences_get_frequency();
int preferences_get_id();

void preferences_update_frequency(int frequency);
void preferences_update_id(int id);
