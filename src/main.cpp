#include "FS.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "string.h"
#include "Arduino.h"

#include "LORA_typedefs.h"
#include "TeleMetry.h"
#include "lora.h"
#include "OLED.h"
#include "GNSS.h"
#include "sensors.h"
#include "SF_RSL.h"
#include "FileSys.h"
#include "preferences.h"
#include "SQL.h"

static long RSL_previousMillis = 0;

const char *ssid = "TTGO";
//const char *password = "KPPTR";

AsyncWebServer server(80);

void setup() {

  Serial.begin(115200);
  Serial.println(F("App start!"));

  if(OLED_init(preferences_get_OLEDdriver())){
    Serial.println(F("OLED init done!"));
    OLED_clear();
    OLED_drawString(0, 5, "OLED OK");
  }
  if(FS_init()){
    Serial.println(F("FS init done!"));
    OLED_drawString(0, 13, "FS OK");
  }  

  preferences_init();

  if(GNSS_init()){
    Serial.println(F("GNSS init done!"));
  }  

  if(LORA_init()){
    Serial.println(F("LORA init done!"));
    OLED_drawString(0, 21, "LORA OK");
  } 

  LORA_changeFrequency(preferences_get_frequency());
  TM_changeID(preferences_get_id());
  
  //SQL init
  SQL_init();

  //Server stuff
  if(SQL_implemented()){
    WiFi.mode(WIFI_AP_STA);

    Serial.println("\n[*] Creating ESP32 AP");
    WiFi.softAP(ssid);
    OLED_drawString(0, 29, "WiFi AP created!");

    Serial.print("Connecting to Hotspot");
    WiFi.begin("HUAWEI-E5372-D407", "widlywgnoju");             // Connect to the network

    OLED_drawString(0, 37, "WiFi connecting...");
    Serial.println(" ...");

    uint8_t timeout = 255;
    while (timeout && (WiFi.status() != WL_CONNECTED))
    {
      delay(100);
      timeout--;
      Serial.print(".");
    }

    if(timeout){
      OLED_drawString(0, 45, "WiFi connected!");
      Serial.println();
      Serial.println("Connected!");
      Serial.print("IP address for WiFi: ");
      Serial.println(WiFi.localIP());
      Serial.print("IP address for AP: ");
      Serial.println(WiFi.softAPIP());
    }
    else {
      OLED_drawString(0, 45, "WiFi con. failed!");
      Serial.println();
      Serial.println("WiFi connection failed!");
    }

    Serial.println('\n');
    Serial.println("Connection established!");  
  } else {
    WiFi.softAP(ssid);
    OLED_drawString(0, 29, "WiFi AP created!");
  }
  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html", false);
  });
  
  server.serveStatic("/download", SPIFFS, "/log.csv");
 
  server.on("/parse", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/log.csv", "text/plain", false);
  });
  server.on("/help", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "KPPTR telemetry receiver:");
  });

  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    deleteFile(SPIFFS, "/log.csv");
    request->send(200, "text/plain", "Succesfully deleted file!");
  });

  server.on("/setFreq", HTTP_POST, [](AsyncWebServerRequest *request){
    String temp;
    int freq;

    temp = request->getParam("freq", true)->value();
    freq = temp.toInt();
    Serial.printf("Received method: %s \n", temp);
    if(freq >= 430000 && freq <= 440000){
      if(LORA_changeFrequency(freq)){
        request->send(200, "text/plain", "Succesfully changed frequency to " + (String)(freq) );
      }
      else{
        request->send(200, "text/plain", "Failed to change frequency!");
      }
    }
    else{
      request->send(200, "text/plain", "Frequency not in range!");
    }
  });

  server.on("/setID", HTTP_POST, [](AsyncWebServerRequest *request){
    String temp;
    int id;
    temp = request->getParam("code", true)->value();
    id = temp.toInt();
    Serial.printf("Received method: %s \n", temp);
    TM_changeID(id);
    request->send(200, "text/plain", "Succesfully changed ID to " + (String)(id) );
  });

  server.on("/setOLED", HTTP_POST, [](AsyncWebServerRequest *request){
    String temp;

    if(request->getParam("driver", true)->value() != NULL){
      temp = request->getParam("driver", true)->value();
      if(temp == NULL){
        request->send(200, "text/plain", "Args Error");
        return;
      }

      String driver = temp;
      Serial.printf("Received method: %s \n", driver);
      OLED_changeDriver(driver);
      request->send(200, "text/plain", "Succesfully changed OLED driver to " + driver);
    }
  });

  server.begin();
  
  LORA_startRX();
}

void loop() {
  LORA_RXhandler();

  LORA_PacketCounter();

  GNSS_srv();

  OLED_refresh();

}