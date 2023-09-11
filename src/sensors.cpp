#include <MPU9250_asukiaaa.h>
#include <Wire.h>
#include <ADXL345.h>
#include "sensors.h"


ADXL345 accel(ADXL345_ALT);
MPU9250_asukiaaa mpu;

static float pitch = 0.0f;
static float roll = 0.0f;
static float azimuth = 0.0f;

void Sensors_init(){
    accel.writeRate(ADXL345_RATE_50HZ);
    accel.writeRange(ADXL345_RANGE_2G);
    accel.start();

    mpu.beginMag();
    // You can set your own offset for mag values
    mpu.magXOffset = 19;
    mpu.magYOffset = -31;
    mpu.magZOffset = -116;

    uint8_t sensorId;
  if (mpu.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }
}

void Sensors_task(){
    if (accel.update()) {
    static float accX = (-accel.getY()) - 0.07f;
    static float accY = (-accel.getX()) - 0.02f;
    static float accZ = -accel.getZ();

    accX = 0.95f*accX + 0.05f*((-accel.getY()) - 0.07f);
    accY = 0.95f*accY + 0.05f*((-accel.getX()) - 0.02f);
    accZ = 0.95f*accZ + 0.05f*((-accel.getZ()));

    pitch = atan2(-accX, sqrt(accZ*accZ + accY*accY));
    roll  = atan2( accY, sqrt(accZ*accZ + accX*accX));
  }

  if (mpu.magUpdate() == 0) {
    //azimuth = -atan2((double) mpu.magZ(), -(double) mpu.magX()) * (180.0f / 3.14159265f);
    float magX = mpu.magX();
    float magY = mpu.magZ();
    float magZ = -mpu.magY();

    // Serial.print(mpu.magX());
    // Serial.print(";");
    // Serial.print(mpu.magY());
    // Serial.print(";");
    // Serial.println(mpu.magZ());

    static float X_h = magX*cos(pitch) + magY*sin(roll)*sin(pitch) + magZ*cos(roll)*sin(pitch);
    static float Y_h = magY*cos(roll) - magZ*sin(roll);
    
    float X_h_new = magX*cos(pitch) + magY*sin(roll)*sin(pitch) + magZ*cos(roll)*sin(pitch);
    float Y_h_new = magY*cos(roll) - magZ*sin(roll);

    X_h = 0.99f*X_h + 0.01f*X_h_new;
    Y_h = 0.99f*Y_h + 0.01f*Y_h_new;
    azimuth = atan2(Y_h, X_h);
    //azimuth -= 0.5f*M_PI;  //obrót o 180 stopni
    if(azimuth < 0) { /* Convert Azimuth in the range (0, 2pi) */
      azimuth = 2*M_PI + azimuth;
    }
  }
}

float Sensors_getPitch(){
    return pitch;
}

float Sensors_getRoll(){
    return roll;
}

float Sensors_getAzm(){
    return azimuth;
}