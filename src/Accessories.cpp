#include "BOARD.h"
#include "Accessories.h"
void Accessories_init() {
#if defined(BAT_ADC_PIN)
    pinMode(BAT_ADC_PIN, INPUT);
#endif
}

float Accessories_getVBat() {
    #if defined(BAT_ADC_PIN)
    uint32_t in = 0;
    for (int i = 0; i < ADC_BATTERY_LEVEL_SAMPLES; i++)
    {
        in += (uint32_t)analogRead(BAT_ADC_PIN);
    }
    in = (int)in / ADC_BATTERY_LEVEL_SAMPLES;
    float bat_mv = ((float)in / 4096) * 3600 * 2;
    float bat_v = bat_mv / 1000;
    return bat_v;
#else
    return 0.0f;
#endif
}