#include "Arduino.h"
#include <math.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> 
#include <MicroNMEA.h>
#include "GNSS.h"

//------ GNSS -------------
SFE_UBLOX_GNSS myGNSS;
char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

//----- Global vars --------
float myLat   = 0.0f;
float myLon   = 0.0f;
uint8_t myFix = 0;

void GNSS_init(){
    Serial1.begin(9600, SERIAL_8N1, 34, 12);

    if (myGNSS.begin(Serial1) == false) {
        Serial.println(F("Ublox init Failed. Freezing."));
        //while (1);
    }
}

void GNSS_srv(){
    if(myGNSS.checkUblox()){
        if (nmea.isValid() == true) {
            long latitude_mdeg = nmea.getLatitude();
            long longitude_mdeg = nmea.getLongitude();
            myLat = latitude_mdeg  / 1000000.0f;
            myLon = longitude_mdeg / 1000000.0f;
            myFix = 1;
        } else {
            myFix = 0;
        }
    }
}

float GNSS_calcDistance(float targetLat, float targetLon){
    if(myFix == 0)
        return -1.0f;

    float rlat1 = 3.14f * myLat / 180.0f;
    float rlat2 = 3.14f * targetLat / 180.0f;
    float theta = myLon - targetLon;
    float rtheta = 3.14f * theta / 180.0f;

    float dist = sin(rlat1) * sin(rlat2) 
                + cos(rlat1) * cos(rlat2) * cos(rtheta);
    dist = acos(dist);
    dist = dist * 180.0f / 3.14f;
    dist = dist * 60.0f * 1.1515f;
    dist = dist * 1.609344f;
    // myLat
    // myLon
    return 0.0f;
}

float GNSS_calcDir(float deviceAzimuth, float targetLat, float targetLon){
    if(myFix == 0)
        return 0.0f;

    float dy = targetLat - myLat;
    float dx = cosf(M_PI/180.0f*myLat) * (targetLon - myLon);
    float angle = atan2f(dy, dx);

    angle = angle * 180.0f/3.14f;
    angle = angle - 90.0f;
    angle = angle + deviceAzimuth;

    if(angle > 180.0f)
        angle = angle - 360.0f;

    return angle;
}

float GNSS_getOwnLat(){
    return myLat;
}

float GNSS_getOwnLon(){
    return myLon;
}

void SFE_UBLOX_GNSS::processNMEA(char incoming)
{
    //Take the incoming char from the Ublox I2C port and pass it on to the MicroNMEA lib
    //for sentence cracking
    nmea.process(incoming);
}