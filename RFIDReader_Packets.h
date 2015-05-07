#ifndef RFIDREADER_PACKETS_H_INCLUDED
#define RFIDREADER_PACKETS_H_INCLUDED


#include "stdafx.h"
#include "rfid_structs_define.h"


typedef enum
{
	PRIVATE  = 0,
	PUBLIC   = 1
}QTSTATE ;

typedef enum 
{
	I = 0,
	O = 1
}GPIO;

struct BlockInfo  
{
	int nEpcLength; 
	int nTagType;
	int nBankType;
	int nStartByte;
	int nLength;
	unsigned char pszPWD[8];
	unsigned char pszEPC[1024];
	unsigned char pszResult[1024]; 

	BlockInfo ()
	{
		nEpcLength = 0;
		nTagType = 0;
		nBankType = 0;
		nLength = 0;
		nStartByte = 0;
	}
};
struct ResultdData
{
	char pszMacErr[8];
	char pszScatErr[8];
	char pszEPC[1024]; 
	ResultdData()
	{
	}
};

//struct  ReadResult
//{
//	char epc[255];
//	char data[1024];
//
//};

typedef struct tagLockPermissions
{
	int killPasswordPermissions;
	/* Permissions for the tag's access password                              */
	int accessPasswordPermissions;
	/* Permissions for the tag's EPC memory bank                              */
	int epcMemoryBankPermissions;
	/* Permissions for the tag's TID memory bank                              */
	int tidMemoryBankPermissions;
	/* Permissions for the tag's user memory bank                             */
	int userMemoryBankPermissions;
}LockPermissions, * LPLockPermissions;


typedef struct { 
	char DevType[8];
	char SocketID[32];
	char IP[24];
}CONNECT_SERVER_INFO;

typedef struct { 
	char eleLevel1[4];
	char eleLevel2[4];
	char eleLevel3[4];
	char eleLevel4[4];
}DEVICE_GPI_STATUS_RESULT;


typedef struct { 
	char AnaStatus[8];
	char TransmitPower[8];
	char DwellTime[8];
	char NumInvCyc[8];
	char AnaStatus2[8];
	char TransmitPower2[8];
	char DwellTime2[8];
	char NumInvCyc2[8];
	char AnaStatus3[8];
	char TransmitPower3[8];
	char DwellTime3[8];
	char NumInvCyc3[8];
	char AnaStatus4[8];
	char TransmitPower4[8];
	char DwellTime4[8];
	char NumInvCyc4[8];
}DEVICE_ANAINFO_STATUS_RESULT;

typedef struct { 
	char MacRegion[8];
	char AnaNum[8];
	char RadioOpen[8];
	char PowerStatus[8];
	char GPINum[8];
	char GPONum[8];
}DEVICE_STATUS_RESULT;

typedef struct { 
	char DeviceID[32];
	char TimeStamp[24];
	char AntennaID[8];
	char EPC[1024];
	char RSSI[32];
	char TID[32];
	char Frequency[32];
}INVENTORY_REPORT_RESULT;

typedef struct { 
	char DeviceID[32];
	char AntennaID[8];
	//2012.3.15
	char strEpc[1024];
	char ReadData[1024];
	char ScatterErr[8];
	char MacErr[8];
	char Ciphertext[1024];
	char Signature[1024];
	int EncryptType;
}READ_REPORT_RESULT;

typedef struct { 

	char			AntennaID[8];
	char			DeviceID[32];
	char			strEpc[1024];
	unsigned char	ReadData[1024];
}FAST_READ_RESULT;
typedef struct  
{
	FAST_READ_RESULT readdata[255];
	short  tagcount;
} ALLReadDataInfo;

typedef enum
{
	None,
	Signature,
	Encrypt, 
	Both
} EncryptType;

typedef struct { 
	char eleLevel1[4];
	char eleLevel2[4];
	char eleLevel3[4];
	char eleLevel4[4];
}DEVICE_GPO_STATUS_RESULT;

typedef struct{
	int		Level1;
	int		Level2;
	int		Level3;
	int		Level4;
}SET_GPO_PARAM;

typedef struct{
	int		status1;
	int		status2;
	int		status3;
	int		status4;
}SET_ANTENNA_PARAM;

typedef struct 
{
	char DeviceID[32];
	char ErrorCode[6];
	char MacCode[4];
}ErrorReportResult;

//////////////////////////////////////////////////////////////////////////
//增加天线的具体设置功能后，添加进来的参数
typedef struct tagANTENNA_PARAM
{
	bool		bEnable;
	int			nPower;
	int			nDwellTime;
	int			nInvCycle;
} SET_ANTENNA_PARAMS;

typedef struct tagAllANTENNA_PARAM
{
	SET_ANTENNA_PARAMS			Ant1;
	SET_ANTENNA_PARAMS			Ant2;
	SET_ANTENNA_PARAMS			Ant3;
	SET_ANTENNA_PARAMS			Ant4;
}SET_AllANTENNA_PARAM;
//////////////////////////////////////////////////////////////////////////

//typedef struct { 
//	E_Result result;
//}GET_RESULT_INFO;
typedef enum E_Result
{
	SUCCESS,
	RUSULTERROR,
	UNIMPLEMENTED,
	UNHANDLEDEXCEPTION,
	ALREADYCONNECTED,
	READERNOTONNETWORK,
	CONNECTIONSTATUSTIMEOUT,
	CONNECTIONSTATUSMALFORMED,
	READERCONNECTEDTOANOTHER,
	READERREFUSEDCONNECTION,
	FAILEDPREPAREFEATURESET,
	NOTCONNECTED,
	MISSINGFEATURESET,
	BADFEATURESET,
	MISSINGSTATUS,
	INVALIDSTATUSREFRESHARGUMENT,
	INVALIDSETTINGS,
	READERNOTSET,
	READERNOTIDLE,
	SENDMESSAGEFAILED,
	RECEIVEDERRORMESSAGE,
	TIMEOUT,
	LOSTCONNECTION,
	ENDOFINPUT,
	RESPONSESTATUSMALFORMED,
	RESPONSESTATUSUNSUCCESSFUL,
	THREADINGERROR,
	THREADCONTEXTERROR
};

typedef struct {                                            
	RFID_PACKET_CALLBACK_FUNCTION           pRegisterCallback;
	RFID_PACKET_CALLBACK_FUNCTION           pUnregisterCallback;
	RFID_PACKET_CALLBACK_FUNCTION           pGetAntennaCallback;
	RFID_PACKET_CALLBACK_FUNCTION           pGetStatusCallback;
	RFID_PACKET_CALLBACK_FUNCTION           pGetGpiCallback;
	RFID_PACKET_CALLBACK_FUNCTION           pGetGpoCallback;
	RFID_PACKET_CALLBACK_FUNCTION           pSetGpoCallback;
	RFID_PACKET_CALLBACK_FUNCTION           pCancelOperationCallback;
	RFID_PACKET_CALLBACK_FUNCTION			pGetResultCallback;
	RFID_PACKET_CALLBACK_FUNCTION			pGetErrorCodeCallBack;
} RFID_SERVER_CALLBACK_PARMS;


typedef struct
{
	unsigned char cid[1024];
	short ncidLen;   //id的长度
	short nCardType; //1为6B协议，2为6C协议
	short nAntenna;
	char TimeStamp[24]; 
	char  szRSSI[32];
	char DeviceID[32];
	char TID[32];
	char Frequency[32];
}SingleTagInfo;

typedef struct
{
	SingleTagInfo OneTagInfo[255];//最大识读255个标签
	short nTagsCount;
} AllTagInfo;

typedef INT32 (RFID_CALLBACK * RFID_CALLBACK_FUNCTION )();

typedef struct
{
	unsigned int len;
	RFID_CALLBACK_FUNCTION disconnect;
}RFID_CLIENT_DISCONNECT;

typedef INT32 (RFID_CALLBACK * RFID_ERROR_CALLBACK)( void*  context );

typedef struct  
{
	unsigned int len;
	RFID_ERROR_CALLBACK  errorreport;
}RFID_ERROR_CALLBACK_FUNCTION;
#endif /* #ifndef RFID_PACKETS_H_INCLUDED */