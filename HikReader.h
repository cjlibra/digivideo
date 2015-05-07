#pragma  once

#include <string>
using namespace std;
#include "RFIDReader_Packets.h"

#define RFID_LIBAPI extern "C" __declspec(dllexport)




RFID_LIBAPI int Connect( char *strIP, char *DevID, unsigned int nPort = 7880, RFID_CALLBACK_FUNCTION DisconnectCallback = NULL, RFID_ERROR_CALLBACK  Errorcallback = NULL);
RFID_LIBAPI int Disconnect(char *DevID);
RFID_LIBAPI int ISO_6C_StartPeriodInventory(char *DevID, RFID_PACKET_CALLBACK_FUNCTION  callbackReport = NULL, BOOL bContinues = FALSE);
RFID_LIBAPI int ISO_6C_GetPeriodInventoryResult ( char *DevID, AllTagInfo &tags);
RFID_LIBAPI int ISO_6C_StopPeriodInventory(char *DevID);
RFID_LIBAPI int ISO_6C_Inventory(char *DevID, AllTagInfo &tags);
RFID_LIBAPI int ISO_6C_Read( char *DevID,short nAntennaID ,BlockInfo *pBlockparam, READ_REPORT_RESULT *pData, int *nCount, bool bEncrypt, bool bCheckSig);
RFID_LIBAPI int ISO_6C_Write(char *DevID, short nAntennaID, BlockInfo *pBlockparam, ResultdData *pData, int *nCount, bool bEncrypt, bool bCheckSig );
RFID_LIBAPI int ISO_6C_BlockWrite(char *DevID, BlockInfo *pBlockparam, ResultdData *pData, int *nCount, short nAntennaID);
RFID_LIBAPI int ISO_6C_BlockErase(char *DevID, short nAntennaID, BlockInfo *pBlockparam,  ResultdData *pData = NULL, int *nCount=0);
RFID_LIBAPI int ISO_6C_Kill(char *DevID, unsigned int unAccessword, unsigned int unKillword, short nAntennaID, char * strEPC, int nEpcLength, ResultdData *pData = NULL, int *nCount=0);
RFID_LIBAPI int ISO_6C_Lock(char *DevID, LockPermissions permission, short nAntennaID, char *strEPC, int nEpcLength, unsigned int accesspwd, ResultdData *pData = NULL, int *nCount=0);
RFID_LIBAPI int GetAntenna(char *DevID, DEVICE_ANAINFO_STATUS_RESULT *pParams );
RFID_LIBAPI int SetAntenna(char * DevID, SET_AllANTENNA_PARAM *pParams );
RFID_LIBAPI int GetReaderInfo(char *DevID, DEVICE_STATUS_RESULT* pRepMsg);
RFID_LIBAPI int SetGpO(char *DevID, SET_GPO_PARAM* pParms);
RFID_LIBAPI int GetGpIO(char *DevID, GPIO io, DEVICE_GPI_STATUS_RESULT* pRepMsg);
RFID_LIBAPI int ISO_6C_AutoReportTag(char *DevID, RFID_PACKET_CALLBACK_FUNCTION  callbackReport, BOOL bState);
RFID_LIBAPI int ISO_6C_QtOperate( char *DevID, int nMemMap, int  nPersistence, int nReadWrite, int nShortRange, UINT passwd, int nAnt, char * strEPC, ResultdData *data, int *Count, QTSTATE &qtstate);

RFID_LIBAPI int ISO_6C_ReadCycle(char *pszDevID, /*int &nCount,*/ short nAntennaID ,BlockInfo *pBlockparam, ALLReadDataInfo &ardi, bool bEncrypt = false, bool bCheckSig = false);
RFID_LIBAPI int ISO_6C_WriteCycle(char *pszDevID, short nAntennaID, BlockInfo *pBlockparam, bool bEncrypt = false, bool bCheckSig = false);
RFID_LIBAPI int ISO_6C_Inventory_Cycle( char *DevID, AllTagInfo &tags);
//////////////////////////////////////////////////////////////////////////
RFID_LIBAPI int StartListen();
RFID_LIBAPI int SetCallBack( RFID_PACKET_CALLBACK_FUNCTION  GetRegisterResulst = NULL, RFID_PACKET_CALLBACK_FUNCTION  GetUnregisterResulst =NULL, RFID_PACKET_CALLBACK_FUNCTION ErrorCallback = NULL );
RFID_LIBAPI int StopListen();