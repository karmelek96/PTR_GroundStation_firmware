#include "FileSys.h"
 

bool FS_init(){

    if(!SPIFFS.begin(true)){
       
        return false;
    }
    //appendFile(SPIFFS, "/log.csv", "\nThis is beginning of new telemetry receiving file, this happensafter every restart of TTGO\n");
    //appendFile(SPIFFS, "/log.csv", "Timestamp,packet_no,state,flags,accX,accY,accZ,gyroX,gyroY,gyroZ,tilt,pressure,velocity,altitude,lat,lat_sign,lon,lon_sign,altitude_gnss,fix,sats,"
    //                    "lat_own,lon_own,fix_own,stas_own,distance,direction\n");

    return true;


  
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    //Serial.printf("Writing file: %s", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.printf("Writing file: %s - failed to open file for writing", path);
        return;
    }
    if(file.print(message)){
        //Serial.println("- file written");
    } else {
        Serial.printf("Writing file: %s - write failed", path);
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    //Serial.printf("Appending to file: %s", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.printf("Appending to file: %s - failed to open file for appending\r\n", path);
        return;
    }
    if(file.print(message)){
        //Serial.println("- message appended");
    } else {
        Serial.printf("Appending to file: %s- append failed", path);
    }
    file.close();
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
