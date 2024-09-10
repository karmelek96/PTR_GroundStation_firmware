#include <Wire.h>
#include <Arduino.h>
#include "PWR.h"

#define XPOWERS_CHIP_AXP2101
#include "XPowersLib.h"
XPowersPMU PMU;


void PWR_init(){
   bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, 21, 22);

    if (result == false) {
        Serial.println("PMU is not online..."); while (1) delay(50);
    }
    Serial.printf("getID:0x%x\n", PMU.getChipID());


    PMU.setALDO2Voltage(3300);
    PMU.enableALDO2();
}