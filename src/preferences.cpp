#include "preferences.h"
#include "FS.h"
#include "SPIFFS.h"
#include <string>
#include <ArduinoJson.h>


using namespace std;

const char *path = "/config.json";

config_data_t config_data_d;

JsonDocument config;

void preferences_update();

int preferences_init(){
    File file;

    if(!SPIFFS.exists(path)){
        file = SPIFFS.open(path, FILE_WRITE);
        if(!file){
            Serial.println("- failed to open file for writing");
            return -1;
        }

        config_data_d.frequency = 433.25;
        config_data_d.id = 21;

        config["configuration"]["frequency"] = 434.25;
        config["configuration"]["id"] = 21;

        serializeJson(config, file);
        file.close(); 

    }
    
    file = SPIFFS.open(path, FILE_READ);
    if(!file){
        Serial.println("- failed to open file for reading");
        return -1;
    }

    deserializeJson(config, file);

    config_data_d.frequency = config["configuration"]["frequency"];
    config_data_d.id = config["configuration"]["id"];



    return 0;
}

int preferences_get_frequency(){  
    return config_data_d.frequency;
}

int preferences_get_id(){
    return config_data_d.id;
}

void preferences_update_frequency(int frequency){
    config_data_d.frequency = frequency;

    preferences_update();
}

void preferences_update_id(int id){
    config_data_d.id = id;
    preferences_update();
}

void preferences_update(){
    File file;
    file = SPIFFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    config["configuration"]["frequency"] = config_data_d.frequency;
    config["configuration"]["id"] = config_data_d.id;

    serializeJson(config, file);
    file.close(); 

}