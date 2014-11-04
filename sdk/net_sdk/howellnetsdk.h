#ifndef _HOWELL_NET_SDK_INCLUDE_H
#define _HOWELL_NET_SDK_INCLUDE_H
#include "protocol_type.h"
#define HOWELL_NET_API	extern "C"__declspec(dllexport)
typedef long USER_HANDLE;
typedef long REAL_HANDLE;
typedef long REC_HANDLE;
typedef long DOWNLOAD_HANDLE;
typedef long VOICE_HANDLE;
typedef long SERIAL_HANDLE;

#define HW_INVALID_HANDLE (-1)

/*alarm data type*/
#define HW_ALARM_HARD                   1 /*alarmData.value = 0; harddisk error
											alarmData.value = 1; dsp error*/
#define HW_ALARM_MOTION                 2
#define HW_ALARM_VIDEO_LOST             3
#define HW_ALARM_STARTREC		        4
#define HW_ALARM_STOPREC		        5
#define HW_ALARM_MASK	                6
#define HW_ALARM_IN 			        7
#define HW_ALARM_HEARTBEAT 	            8
#define HW_ALARM_IN_SERVER              9  //VISTA 120/���� ����������������
#define HW_ALARM_MOTIONEX				10 //��չ�ƶ���ⱨ��

//���һط��ļ�����ֵ
#define HWNET_FILE_SUCCESS		1000	//��ȡ�ļ��ɹ�
#define HWNET_NOFILE			1001	//û���ļ�
#define HWNET_ISFINDING			1002	//���ڲ���
#define HWNET_NOMOREFILES		1003	//û�и����ļ�
#define HWNET_FILE_EXCEPTION	1004	//�쳣


HOWELL_NET_API BOOL __stdcall	HW_NET_Init(long localPort);
HOWELL_NET_API BOOL __stdcall	HW_NET_Release();
HOWELL_NET_API USER_HANDLE __stdcall	HW_NET_Login(const char *sIP,int dwPort,const char *sUser,const char*sPassword);
HOWELL_NET_API BOOL __stdcall	HW_NET_Logout(USER_HANDLE lUserID);
HOWELL_NET_API long __stdcall   HW_NET_GetWindowCount(USER_HANDLE lUserID);
HOWELL_NET_API BOOL __stdcall   HW_NET_GetNetVersion(long *pMainVersion,long *pSubVersion);
/**************    obsoleted  ********************/
HOWELL_NET_API BOOL __stdcall   HW_NET_StartCenterListen(long listenPort);
/*************************************************/
HOWELL_NET_API long __stdcall   HW_NET_GetLastError();
HOWELL_NET_API BOOL __stdcall   HW_NET_Reboot(USER_HANDLE lUserID);
HOWELL_NET_API BOOL __stdcall   HW_NET_ShutDown(USER_HANDLE lUserID);
HOWELL_NET_API BOOL __stdcall   HW_NET_Restore(USER_HANDLE lUserID);
HOWELL_NET_API void __stdcall	HW_NET_SetTimeout(long timeout_millisec);

/*real video*/
HOWELL_NET_API REAL_HANDLE  __stdcall	HW_NET_OpenVideo(USER_HANDLE lUserID,int slot,HWND hWnd,int nPlaySlot);
HOWELL_NET_API REAL_HANDLE  __stdcall	HW_NET_OpenVideoEx(USER_HANDLE lUserID,int slot,void(CALLBACK*func)(long lVideoID,char*buf,int len,int videoType,long nUser),long nUser);
HOWELL_NET_API BOOL	__stdcall	HW_NET_CloseVideo(REAL_HANDLE lRealID);
HOWELL_NET_API BOOL __stdcall   HW_NET_OpenSound(REAL_HANDLE lRealID);
HOWELL_NET_API BOOL __stdcall   HW_NET_CloseSound(REAL_HANDLE lRealID);
HOWELL_NET_API BOOL __stdcall	HW_NET_GetVideoColor(REAL_HANDLE lRealID,struct tVideoColor *pVideoCOlor);
HOWELL_NET_API BOOL __stdcall	HW_NET_SetVideoColor(REAL_HANDLE lRealID,struct tVideoColor *pVideoColor);
HOWELL_NET_API BOOL __stdcall	HW_NET_GetVideoStatus(REAL_HANDLE lRealID,struct tVideoStatus *pVideoStatus);
HOWELL_NET_API BOOL __stdcall	HW_NET_ControlPtz(REAL_HANDLE lRealID,struct tPtzControl *pPtzControl);
HOWELL_NET_API BOOL __stdcall	HW_NET_Snap(REAL_HANDLE lRealID,const char * szFile);
HOWELL_NET_API BOOL __stdcall   HW_NET_SnapJpg(REAL_HANDLE lRealID,const char*szFile);
HOWELL_NET_API BOOL __stdcall	HW_NET_StartRecord(REAL_HANDLE lRealID,const char *szFile);
HOWELL_NET_API BOOL __stdcall	HW_NET_StopRecord(REAL_HANDLE lRealID);

/*rec video*/
HOWELL_NET_API REC_HANDLE __stdcall	HW_NET_PLAY_OpenVideo(USER_HANDLE lUserID,struct tRecFile *pRecFile,HWND hWnd,int nPlaySlot);
HOWELL_NET_API REC_HANDLE __stdcall	HW_NET_PLAY_OpenVideoEx(USER_HANDLE lUserID,struct tRecFile *pRecFile,void(CALLBACK*func)(long lPlayID,char*buf,int len,int videoType,long nUser),long nUser);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_CloseVideo(REC_HANDLE lPlayID);
/************************************************************************/
///old interfaces
HOWELL_NET_API long __stdcall	HW_NET_PLAY_GetPlayFiles(USER_HANDLE lUserID,struct tRecFile *pRecFile,const char*szSaveFile);
HOWELL_NET_API BOOL __stdcall   HW_NET_PLAY_GetPlayFilesEx(USER_HANDLE lUserID,struct tRecFile *pSearchTime,struct tRecFile *pToSave,long maxNumFile,long *pReturnNum);
///new interfaces
HOWELL_NET_API long __stdcall	HW_NET_PLAY_FindFile(USER_HANDLE lUserID,struct tRecFile * pSearchFile);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_FindFileClose(long lFileHandle);
HOWELL_NET_API int  __stdcall	HW_NET_PLAY_FindNextFile(long lFileHandle,struct tRecFile * pRecFile);
/************************************************************************/
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_GetColor(REC_HANDLE lPlayID,int *pBrightness,int *pConstrast,int *pSaturation, int*pHue);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_SetColor(REC_HANDLE lPlayID,int nBrightness,int nContrast,int nSaturation,int nHue);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_GetVideoSize(REC_HANDLE lPlayID,int *pWidth,int *pHeight);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_OpenSound(REC_HANDLE lPlayID);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_CloseSound(REC_HANDLE lPlayID);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_GetVolume(long lPlayID,int *volume);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_SetVolume(REC_HANDLE lPlayID,int volume);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_Pause(REC_HANDLE lPlayID,int bPause);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_SetSpeed(REC_HANDLE lPlayID,int nSpeed);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_GetSpeed(REC_HANDLE lPlayID,int *nSpeed);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_Snap(REC_HANDLE lPlayID,const char *szSaveFile);
HOWELL_NET_API BOOL __stdcall   HW_NET_PLAY_SnapJpg(REC_HANDLE lPlayID,const char*szSaveFile);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_StepForward(REC_HANDLE lPlayID);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_StepBack(REC_HANDLE lPlayID);
HOWELL_NET_API BOOL __stdcall	HW_NET_PLAY_Refresh(REC_HANDLE lPlayID);
HOWELL_NET_API BOOL __stdcall   HW_NET_PLAY_GetPlayedSec(REC_HANDLE lPlayID,int *pSec);
HOWELL_NET_API BOOL __stdcall   HW_NET_PLAY_SetFileEndMsg(HWND hMsgWnd,UINT msg);
HOWELL_NET_API BOOL __stdcall   HW_NET_PLAY_ThrowBFrame(REC_HANDLE lPlayID,int num);

/*remote set*/
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetChannelName(USER_HANDLE lUserID,struct tOsdChannel *pOsdName);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetChannelName(USER_HANDLE lUserID,struct tOsdChannel *pOsdName);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetDvrVersion(USER_HANDLE lUserID,long *pVersion);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetDvrNetVersion(USER_HANDLE lUserID,long *pNetVersion);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetNetState(USER_HANDLE lUserID,long *pIntervalMillSec);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetVideoColor(USER_HANDLE lUserID,struct tVideoColor *pVideoColor);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetVideoColor(USER_HANDLE lUserID,struct tVideoColor *pVideoColor);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetVideoQuality(USER_HANDLE lUserID,struct tVideoQuality *pVideoQuality);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetVideoQuality(USER_HANDLE lUserID,struct tVideoQuality *pVideoQuality);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetOsdDate(USER_HANDLE lUserID,struct tOsdDate *pOsdDate);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetOsdDate(USER_HANDLE lUserID,struct tOsdDate *pOsdDate);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetVideoStandard(USER_HANDLE lUserID,struct tVideoStandard *pVideoStandard);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_ControlPTZ(USER_HANDLE lUserID,struct tPtzControl *pPtzControl);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetMotion(USER_HANDLE lUserID,struct tMotion *pMotionSet);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetMotion(USER_HANDLE lUserID,struct tMotion *pMotionSet);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetMask(USER_HANDLE lUserID,struct tMask *pMask);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetMask(USER_HANDLE lUserID,struct tMask *pMask);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_StartRecord(USER_HANDLE lUserID,int slot);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_StopRecord(USER_HANDLE lUserID,int slot);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetAlarmCallback(void (CALLBACK* func)(const char *ip,int type,char *buf,int len,long nUser),long nUser);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetAlarmCallbackEx(void (CALLBACK* func)(USER_HANDLE lUserID,int type,char *buf,int len,long nUser),long nUser);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_RegisterAlarm(USER_HANDLE lUserID,BOOL bRegister);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetStreamType(USER_HANDLE lUserID,struct tStreamType *pStreamType);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetStreamType(USER_HANDLE lUserID,struct tStreamType *pStreamType);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetSystime(USER_HANDLE lUserID, SYSTEMTIME* sysTime);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetSystime(USER_HANDLE lUserID, SYSTEMTIME* sysTime);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetNetworkSetting(USER_HANDLE lUserID, tNetworkSetInfo* networkSetInfo);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetNetworkSetting(USER_HANDLE lUserID, tNetworkSetInfo* networkSetInfo);
/************************************************************/
///old interface
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetLog(USER_HANDLE lUserID, tLogInfo* pSearchTime, tLogInfo* pSaveLog, long MaxLogNum, long* ReturnNum);
///new interfaces
HOWELL_NET_API long __stdcall	HW_NET_SET_FindLog(USER_HANDLE lUserID,tLogInfo* pSearchTime);
HOWELL_NET_API int  __stdcall	HW_NET_SET_FindNextLog(long lLogHandle,tLogInfo* pResLog);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_FindLogClose(long lLogHandle);
/************************************************************/

/************************************************************/
///old
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetSmartSearchFile(USER_HANDLE lUserID, tSmartSearch *pSearchTime, tRecFile *pSaveSSFile, long maxSSFileNum, long* rtnNum);
///new
HOWELL_NET_API long __stdcall	HW_NET_SET_FindSmartSearchFile(USER_HANDLE lUserID,tSmartSearch* pSearch);
/************************************************************/
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetCenterInfo(USER_HANDLE lUserID,tCenter* pCenterInfo);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetCenterInfo(USER_HANDLE lUserID,tCenter* pCenterInfo);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_ForceIFrame(USER_HANDLE lUserID,int slot);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetAlarmState(USER_HANDLE lUserID,tAlarmState* pAlarmState);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetDevConfig(USER_HANDLE lUserID,tDevConfig* pDevConfig);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_UpgradeRequest(USER_HANDLE lUserID,int *ready);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SendUpgradeFile(USER_HANDLE lUserID,const char* fileName);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_StopSendUpgFile(USER_HANDLE lUserID);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetUpgradeState(USER_HANDLE lUserID,int *upgrade_state);

/*download*/
HOWELL_NET_API DOWNLOAD_HANDLE __stdcall   HW_NET_SET_DownloadFile(USER_HANDLE lUserID,struct tRecFile *pRecFile,const char *szSaveFile);
HOWELL_NET_API DOWNLOAD_HANDLE __stdcall   HW_NET_SET_DownloadFileEx(USER_HANDLE lUserID,struct tRecFile *pRecFile,void(CALLBACK*func)(long lDownID,char*buf,int len,int videoType,long nUser),long nUser);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_StopDownload(DOWNLOAD_HANDLE lDownloadID);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetdownloadPos(DOWNLOAD_HANDLE lDownloadID,int *pPos);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetdownloadFileSize(DOWNLOAD_HANDLE lDownloadID,int *pSize);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetdownloadFileSizeEx(DOWNLOAD_HANDLE lDownloadID,unsigned int *pSizeL,unsigned int *pSizeH);

/*call back*/
/***********************obsoleted****************************/
HOWELL_NET_API BOOL __stdcall	HW_NET_RegisterDrawFunc(void(CALLBACK*func)(long nPort,HDC hdc,long nUser),long nUser);
HOWELL_NET_API BOOL __stdcall   HW_NET_RegisterDrawFuncEx(int nPort,void(CALLBACK*func)(long nPort,HDC hdc,long nUser),long nUser);
/************************************************************/
HOWELL_NET_API BOOL __stdcall	HW_NET_RegisterRealDrawFunc(long realhandle,void(CALLBACK*func)(long nPort,HDC hdc,long nUser),long nUser);
HOWELL_NET_API BOOL __stdcall	HW_NET_RegisterPlayDrawFunc(long playhandle,void(CALLBACK*func)(long nPort,HDC hdc,long nUser),long nUser);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*for tempest dvr*/
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetWorkSheet(USER_HANDLE lUserID,struct tTempestWorkSheet* pWorkSheet);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetWorkSheet(USER_HANDLE lUserID,struct tTempestWorkSheet* pWorkSheet);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetUsers(USER_HANDLE lUserID,tTempestUsers *pUsers);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetUsers(USER_HANDLE lUserID,tTempestUsers *pUsers);

/*for devinci dvr*/
// HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetDevinciWorkSheet(USER_HANDLE lUserID,tDeVinciWorkSheet* pWorkSheet);
// HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetDevinciWorkSheet(USER_HANDLE lUserID,tDeVinciWorkSheet* pWorkSheet); 
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetDavinciUsers(USER_HANDLE lUserID, tDavinciUser* pUsers);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetDavinciUsers(USER_HANDLE lUserID, tDavinciUser* pUsers);

/*for howell dvr*/
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetHowellWorkSheet(USER_HANDLE lUserID,struct tWorkSheet* pWorkSheet);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetHowellWorkSheet(USER_HANDLE lUserID,struct tWorkSheet* pWorkSheet);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetUsingWorkSheetNo(USER_HANDLE lUserID,int * pWorkSheetNo);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetUsingWorkSheetNo(USER_HANDLE lUserID,int WorkSheetNo);

//================= IPCAM ========================//
//ip���������
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_IPCam_GetFeature(USER_HANDLE lUserID,struct tIPCam_feature *pFeature);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_IPCam_SetFeature(USER_HANDLE lUserID,struct tIPCam_feature *pFeature);
//ǿ������
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_IPCAM_GetBlackMaskBLC(USER_HANDLE lUserID,struct tBlackMaskBLC *pBMB);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_IPCAM_SetBlackMaskBLC(USER_HANDLE lUserID,struct tBlackMaskBLC *pBMB);
//����Ǩ��
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_IPCAM_GetLostDataTransfer(USER_HANDLE lUserID,struct tLostDataTransfer* pLostDataTransfer);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_IPCAM_SetLostDataTransfer(USER_HANDLE lUserID,struct tLostDataTransfer* pLostDataTransfer);

//================= NVR ==========================//
//��ȡ����ipcamͨ����ͨ��������Ϣ
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetChannelSet(USER_HANDLE lUserID,struct tChannelSet *pChanSet);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_SetChannelSet(USER_HANDLE lUserID,struct tChannelSet *pChanSet);
//��ȡͨ������ipcam��״̬��0-δ���ӻ�δ������1-�������ӣ�����*pChanStatus��lenΪdvrͨ������byte����
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetChannelStatus(USER_HANDLE lUserID,struct tChannelStatus* pChanStatus);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetChannelType(USER_HANDLE lUserID,struct tChannelType *pChanType);
HOWELL_NET_API BOOL __stdcall	HW_NET_SET_GetRecFileInfo(USER_HANDLE lUserID,struct tRecFileInfo *pFileInfo);

//=================head data======================//
HOWELL_NET_API BOOL __stdcall   HW_NET_GetVideoHead(REAL_HANDLE lRealID,char* head,int len,int* head_len);
HOWELL_NET_API BOOL __stdcall   HW_NET_PLAY_GetVideoHead(REC_HANDLE lPlayID,char* head,int len,int* head_len);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetDownloadHead(DOWNLOAD_HANDLE lDownloadID,char* head,int len,int* head_len);

//===============================================//
//�ֶ������������ݵĲ��񣬱�����øýӿڲŻ�������
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_StartDownloadCapture(long lDownloadID);	


//================�������(�̵���)״̬===========//
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetAlarmOutState(USER_HANDLE lUserID, tAlarmOutState* pAlarmOutState);
//lAlarmOutPort:0xff-���м̵���  ����-ָ���̵���
//AlarmOutState: 1-��Ч 0- ��Ч
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetAlarmOutState(USER_HANDLE lUserID, LONG lAlarmOutPort,LONG AlarmOutState);

//================������������===================//
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetAlarmInConfig(USER_HANDLE lUserID,tAlarmInCfg* pAlarmInConfig);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetAlarmInConfig(USER_HANDLE lUserID,tAlarmInCfg* pAlarmInConfig);

//================�ֶ�������=====================//
//lAlarmInPort: ���������
//DefendType: 1-����  0-����
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetAlarmDefend(USER_HANDLE lUserID,LONG lAlarmInPort,LONG* DefendType);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetAlarmDefend(USER_HANDLE lUserID,LONG lAlarmInPort,LONG DefendType);

//================RS232����======================//
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetRS232Config(USER_HANDLE lUserID,tRs232Cfg* pRS232Config);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetRS232Config(USER_HANDLE lUserID,tRs232Cfg* pRS232Config);

//================͸��ͨ��=======================//
HOWELL_NET_API SERIAL_HANDLE __stdcall   HW_NET_SET_StartRS232Send(USER_HANDLE lUserID,int Rs232No);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SendRS232Data(SERIAL_HANDLE lSerialID,tRs232Send* pRS232Send);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_StopRS232Send(SERIAL_HANDLE lSerialID);

//================PTZ RS232����=================//
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetPtzRSConfig(USER_HANDLE lUserID,tPtzRs232Cfg * pPtzRSConfig);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetPtzRSConfig(USER_HANDLE lUserID,tPtzRs232Cfg * pPtzRSConfig);

//================��Ƶ��ʧ��������==============//
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_GetVideoLostConfig(USER_HANDLE lUserID,tVideoLostCfg* pVideoLostConfig);
HOWELL_NET_API BOOL __stdcall   HW_NET_SET_SetVideoLostConfig(USER_HANDLE lUserID,tVideoLostCfg* pVideoLostConfig);				

//================�����Խ�======================//
//VoiceComType: 0-�ͻ��˵����� 1-˫��
HOWELL_NET_API VOICE_HANDLE __stdcall HW_NET_SET_StartVoiceCom(USER_HANDLE lUserID,int VoiceComType);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_StopVoiceCom(VOICE_HANDLE lVoiceComHandle);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_RegisterVoiceComCallBack(VOICE_HANDLE lVoiceComHandle,void(CALLBACK* fun)(long lVoiceComHandle,char *buf,int len,int flag,long dwUser),long dwUser);

//================������========================//
HOWELL_NET_API REAL_HANDLE  __stdcall	HW_NET_OpenSubVideo(USER_HANDLE lUserID,int slot,HWND hWnd,int nPlaySlot);
HOWELL_NET_API REAL_HANDLE  __stdcall	HW_NET_OpenSubVideoEx(USER_HANDLE lUserID,int slot,void(CALLBACK*func)(long lVideoID,char*buf,int len,int videoType,long nUser),long nUser);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_GetSubChannelSet(USER_HANDLE lUserID,tSubChannelSet* pSubChannelSet);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_SetSubChannelSet(USER_HANDLE lUserID,tSubChannelSet* pSubChannelSet);


//================�ƶ������չ=================//
HOWELL_NET_API BOOL __stdcall HW_NET_SET_GetMotionRowCols(USER_HANDLE lUserID,tMotionRowCols* pMotionRowCols);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_GetMotionEx(USER_HANDLE lUserID,tMotionEx* pMotionEx);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_SetMotionEx(USER_HANDLE lUserID,tMotionEx* pMotionEx);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_GetSmartSearchFileEx(USER_HANDLE lUserID, tSmartSearchEx *pSearchTime, tRecFile *pSaveSSFile, long maxSSFileNum, long* rtnNum);
HOWELL_NET_API long __stdcall HW_NET_SET_FindSmartSearchFileEx(USER_HANDLE lUserID,tSmartSearchEx *pSearch);

//================�ڵ�����=====================//
HOWELL_NET_API BOOL __stdcall HW_NET_SET_GetAlarmMask(USER_HANDLE lUserID,tAlarmMask* pAlarmMask);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_SetAlarmMask(USER_HANDLE lUserID,tAlarmMask* pAlarmMask);

//=================Ӳ����Ϣ�͸�ʽ��============//
HOWELL_NET_API BOOL __stdcall HW_NET_SET_GetHarddiskState(USER_HANDLE lUserID,tHarddiskInfo* pHarddiskInfo);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_FormatHarddisk(USER_HANDLE lUserID,int disk_idx);
HOWELL_NET_API BOOL __stdcall HW_NET_SET_GetFormatProgress(USER_HANDLE lUserID,int* progress);//0-100

#if 0
//=====================Ӳ��������=============//
HOWELL_NET_API BOOL __stdcall HW_NET_DECODER_GetWinFormat(USER_HANDLE lUserID,int* win_count,int* win_format);
HOWELL_NET_API BOOL __stdcall HW_NET_DECODER_SetWinFormat(USER_HANDLE lUserID,int win_count,int win_format);
HOWELL_NET_API BOOL __stdcall HW_NET_DECODER_OpenWinVideo(USER_HANDLE lUserID,net_win_video_t* pvideo);
HOWELL_NET_API BOOL __stdcall HW_NET_DECODER_CloseWinVideo(USER_HANDLE lUserID,int win_idx);
HOWELL_NET_API BOOL __stdcall HW_NET_DECODER_GetWinVideo(USER_HANDLE lUserID,net_win_video_t* pvideo);
#endif

#endif
