#pragma once

typedef struct {
    uint16_t sender_id;
    uint16_t packet_no;
    float vbat;
    uint8_t sats_fix;
    float latitude;
    float longitude;
    float altitude;
    float max_altitude;
    uint8_t packet_length;
    uint8_t raw[256];
} packet_generic_t;

typedef struct{
    uint16_t sender_id;
    uint32_t last_upload_time_ms;
} history_table_entry_t;

typedef enum{
    HISTORY_ERROR,
    HISTORY_ALLOW_UPLOAD,
    HISTORY_OMIT_UPLOAD
} sql_histoy_state_e;

void SQL_init();
void SQL_addToBuffer(packet_generic_t * data);
bool SQL_implemented();
