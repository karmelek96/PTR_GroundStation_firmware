#pragma once



typedef struct
{
	uint32_t 	time;
} Pkt_R2G_RtcTime_t;

typedef struct
{
	uint16_t 	status;
	uint8_t 	control;
	uint8_t 	flightState;
} Pkt_R2G_System_t;

typedef struct
{
	uint16_t	volt3V3;
	uint16_t	volt5V0;
	uint16_t	voltRail;
	uint16_t	P_SUPP;     
	uint16_t	P_BUFF;     
	uint16_t	VCC_REC;    
	uint8_t		VCC_CGT;         
	uint8_t		VCC_TVC;         
	uint8_t		flags;        
	uint8_t		ICC_CGT;       
} Pkt_R2G_PwrSupply_t;

typedef struct
{
	uint32_t 	cnt;
	uint32_t 	interval;
} Pkt_R2G_MeasStat_t;

typedef struct
{
	uint16_t 	diagStat;
	int16_t		xGyroOut;
	int16_t 	yGyroOut;
	int16_t 	zGyroOut;
	int16_t 	xAcclOut;
	int16_t 	yAcclOut;
	int16_t 	zAcclOut;
	int16_t 	xMagnOut;
	int16_t 	yMagnOut;
	int16_t 	zMagnOut;
	uint16_t 	baroOut;
	int16_t 	tempOut;
} Pkt_R2G_AdisData_t;

typedef struct
{
	int16_t 	temp_5;
	int16_t 	reserved;
    int32_t 	AUX_BARO_PRESS;
} Pkt_R2G_AuxBaro_t;

typedef struct
{
	int32_t 	latitude;
	int32_t 	longitude;
	int32_t 	altitude;
} Pkt_R2G_Gnss_t;

typedef struct
{
	float 		xAxis;
	float 		yAxis;
	float 		zAxis;
} Pkt_R2G_Acc_t;

typedef struct
{
	float 		xAxis;
	float 		yAxis;
	float		zAxis;
} Pkt_R2G_Vel_t;

typedef struct
{
	float 		xAxis;
	float 		yAxis;
	float		zAxis;
} Pkt_R2G_Pos_t;

typedef struct
{
	float		roll;
	float		pitch;
	float		yaw;
} Pkt_R2G_Orient_t;

typedef struct
{
	uint8_t		data[24];
} Pkt_R2G_UserMem_t;

//-----------------------------------------------------------
typedef enum
{
	CMD_ID_ID 			= 0,
	CMD_ID_SYS_TIME		= 1,
	CMD_ID_STATUS		= 2,
	CMD_ID_RSSI			= 3,
	CMD_ID_RX_PKT		= 4,
	CMD_ID_AUTO_UPD		= 5,
	CMD_ID_DEM_STS		= 6
} Cmd_Id_t;

typedef struct
{
	Pkt_R2G_RtcTime_t		rtcTime;		// 4 Bytes
	Pkt_R2G_System_t		system;			// 4 Bytes
	Pkt_R2G_PwrSupply_t		pwrSupply;		// 16 Bytes
	Pkt_R2G_MeasStat_t		measStat;		// 8 Bytes
	Pkt_R2G_AdisData_t		adisData;		// 24 Bytes
	Pkt_R2G_AuxBaro_t		auxBaro;		// 8 Bytes
	Pkt_R2G_Gnss_t			gnss;			// 12 Bytes
	Pkt_R2G_Acc_t			acc;			// 12 Bytes
	Pkt_R2G_Vel_t			vel;			// 12 Bytes
	Pkt_R2G_Pos_t			pos;			// 12 Bytes
	Pkt_R2G_Orient_t		orient;			// 12 Bytes
//	Pkt_R2G_UserMem_t		userMem;		// 24 Bytes
} Pkt_R2G_Id_AllData_t;

// Rocket --> [Ground RX]
typedef union
{
	struct
	{
		uint8_t		 	length;
		uint8_t		 	address;
		uint8_t 	 	payload[124];
		uint8_t		 	rssi;
		uint8_t		 	crcOkLqi;
	};
	uint8_t rawData[128];
} Pkt_R2G_Rx_t;

void RSL_sendPacket();