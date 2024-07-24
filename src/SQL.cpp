#include "Arduino.h"
#include "SQL.h"
#include <freertos/message_buffer.h>
#include <HTTPClient.h>
#define SQL_IMPLEMENTED false

bool SQL_implemented(){
    return SQL_IMPLEMENTED;
}

#if SQL_IMPLEMENTED == false
void SQL_init() {}
void SQL_addToBuffer(packet_generic_t * data) {}
#else
#include "SQL_config.h"



WiFiClient client;
HTTPClient http;

void task_SQL( void * parameter);
sql_histoy_state_e SQL_checkHistory(uint16_t sender_id);
void SQL_updateHistory(uint16_t sender_id);
void SQL_uploadToDB(packet_generic_t packet);

MessageBufferHandle_t xMessageBuffer_sql = NULL;
history_table_entry_t history_table[256];

void SQL_init(){
    xMessageBuffer_sql = xMessageBufferCreate( 4096 );
    if ( xMessageBuffer_sql == NULL )
    {
        Serial.println("Unable to Create Message Buffer");
        while(1){
            vTaskDelay(1000);
        }
    }

    memset(history_table, 0, sizeof(history_table));

    //http.setReuse(true);

    xTaskCreatePinnedToCore(
      task_SQL, /* Function to implement the task */
      "task_SQL", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      NULL,  /* Task handle. */
      0); /* Core where the task should run */
}

void SQL_addToBuffer(packet_generic_t * data){
    if(SQL_checkHistory(data->sender_id) != HISTORY_ALLOW_UPLOAD){
        return;
    }

    SQL_updateHistory(data->sender_id);
    
    size_t xBytesSent = xMessageBufferSend( xMessageBuffer_sql,
                                     ( void * ) data,
                                     sizeof(packet_generic_t),
                                     10 );

    if ( xBytesSent != sizeof(packet_generic_t) )
    {
      Serial.println("xMessageBufferSend error");
    }
}

void task_SQL( void * parameter) {
  Serial.println("task_SQL running");
  packet_generic_t buffer;
  size_t xReceivedBytes = 0;

  for(;;) {
    xReceivedBytes = xMessageBufferReceive( xMessageBuffer_sql,
                                            ( void * ) (&buffer),
                                            sizeof(packet_generic_t), //This sets the maximum length of the message that can be received.
                                            portMAX_DELAY );

    if ( xReceivedBytes > 0 )
    {
        Serial.println("New packet -> SQL");
        SQL_uploadToDB(buffer);
    }

    vTaskDelay(1);
  }
}

sql_histoy_state_e SQL_checkHistory(uint16_t sender_id){
    uint16_t table_size = sizeof(history_table) / sizeof(history_table_entry_t);
    uint16_t i = 0;
    uint8_t found = 0;

    while(i < table_size){
        if(history_table[i].sender_id == sender_id){
            found = 1;
            break;
        }
        i++;
    }

    if(found){
        if((history_table[i].last_upload_time_ms + 2000) < millis()){
            return HISTORY_ALLOW_UPLOAD;
        }
    }
    else{
        return HISTORY_ALLOW_UPLOAD;
    }
 
    return HISTORY_OMIT_UPLOAD;
}

void SQL_updateHistory(uint16_t sender_id){
    uint16_t table_size = sizeof(history_table) / sizeof(history_table_entry_t);
    uint16_t index = 0;
    uint8_t found = 0;

    // check if sender_id entry exists
    uint16_t i = 0;
    while(i < table_size){
        if(history_table[i].sender_id == sender_id){
            found = 1;
            index = i;
            break;
        }
        i++;
    }
    
    if(!found){
        // No existing entry found - add new or replace oldest
        int minIndex = 0; // Assume the first element has the lowest value initially

        for (int i = 1; i < table_size; i++) {
            if (history_table[i].last_upload_time_ms < history_table[minIndex].last_upload_time_ms) {
                minIndex = i; // Update minIndex if a smaller element is found
            }
        }
        index = minIndex;
    }

    history_table[index].sender_id = sender_id;
    history_table[index].last_upload_time_ms = millis();
}

void SQL_uploadToDB(packet_generic_t packet){
    if(WiFi.status() == WL_CONNECTED){	  
	  char raw_packet[128];
      char serverPath[1024];

	  memset(raw_packet, 0, sizeof(raw_packet));
	  memset(serverPath, 0, sizeof(serverPath));

	  for(uint8_t i = 0; i < packet.packet_length; i++){
		sprintf(raw_packet + 2 * i, "%02X", packet.raw[i]);
	  }
    
	  sprintf(serverPath, "%s" 
                            "?api_key=%s"
							"&object_id=%i"   // S_objectID
                            "&object_type=ROCKET"
							"&packet_no=%i"   // S_packetNo
							"&latitude=%.5f" // S_latitude
							"&longitude=%.5f" // S_longitude
							"&altitude=%.1f" // S_altitude
                            "&sats_fix=%i"   // S_gnssFix + sats
							"&max_altitude=%.1f" // S_maxAltitude
                            "&vbat=%.1f"        //S_vbat
							"&raw=%%22%s%%22",
							api_address, api_key, packet.sender_id, packet.packet_no, packet.latitude, packet.longitude, packet.altitude,
						    packet.sats_fix, packet.max_altitude, packet.vbat, raw_packet);
      
	  //Serial.println(serverPath);

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath);
  
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
}
#endif
