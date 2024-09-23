#include "BOARD.h"
#include "FileSys.h"

#if defined(HAS_SDCARD)
#include <SD.h>
SPIClass SDCardSPI(HSPI);
#endif

bool FS_init(){
    #ifdef HAS_SDCARD
    if (SD.begin(SDCARD_CS, SDCardSPI)) {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        Serial.print("Sd Card init succeeded, The current available capacity is ");
        Serial.print(cardSize / 1024.0);
        Serial.println(" GB");
        return true;
    } else {
        Serial.println("Warning: Failed to init Sd Card");
    }

    #endif
    
    if(!SPIFFS.begin(true)){
        return false;
    }

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
