#ifndef _protocol_type_include_h
#define _protocol_type_include_h

#define MAX_DATA_LEN			4096
#define MAX_USER				256
#define MAX_MANAGER_USER		5
#define MAX_SLOT				256
#define	MAX_NAME_LEN			32
#define	MAX_PASS_LEN			16
#define IP_LENGTH				32
#define	MAX_DAVINCI_USER		16
#define MAX_FILES				2000
#define HW_MAX_ALARM_IN			16
#define HW_MAX_ALARM_OUT		16
#define HW_MAX_DISKNUM			8
#define HW_MAX_DSPNUM			32
#define HW_MAX_SERIALID			32

#define VERSION_TEMPEST(x) ((x) >= 20000 && (x) <=29999)
#define VERSION_DAVI(x) ((x) >= 10000 && (x) <=19999)
#define VERSION_HOWELL_LINUX(x)((x) >= 30000 && (x) <= 39999)
#define VERSION_HIS(x) ((x) >= 40000 && (x) <= 49999)
#define VERSION_NVR(x) (((x) >= 50000 && (x) <= 59999) || ((x) >= 100000 && (x) <= 109999))
#define VERSION_IPCAM(x) ((x) >= 60000 && (x) <= 69999)
#define VERSION_IPCAM_A5(x) ((x) >= 65000 && (x) <= 69999)
#define VERSION_MIXED_NVR(x)((x) >= 70000 && (x) <= 79999)//�����nvr
#define VERSION_HIS_RAILWAY(x) ((x) >= 80000 && (x) <= 89999)

#define OLD_NET_VERSION_DVR(x) ((x) < 90000)
#define VERSION_DECODER(x) ((x) >= 120000 && (x) <= 130000)


#define HWHI_HANDLE_BEG			4000		//his
// #define HWNVR_HANDLE_BEG		5000		//nvr
// #define HWIPC_HANDLE_BEG		6000		//ip cam

/*Video Data Type*/
#define HW_FRAME_VIDEO_HEAD	            1
#define HW_FRAME_VIDEO_I                3
#define HW_FRAME_VIDEO_P                4
#define HW_FRAME_VIDEO_BP               5
#define HW_FRAME_AUDIO                  2
#define HW_FRAME_VIDEO_SUB_HEAD         7
#define HW_FRAME_VIDEO_SUB_I            8
#define HW_FRAME_VIDEO_SUB_P            9
#define HW_FRAME_VIDEO_SUB_BP          10

#define HW_FRAME_VIDEO_BBP             11
#define HW_FRAME_VIDEO_SUB_BBP         12
#define HW_FRAME_MOTION_FRAME          13
#define HW_FRAME_VIDEO_B               14
#define HW_FRAME_VIDEO_SUB_B           15

#define HW_FRAME_VIDEO_MJPEG		   16
#define HW_FRAME_VIDEO_SUB_MJPEG       17

#define HW_FRAME_FOCUS				   20
#define HW_FRAME_VIDEO_ANALYZE		   21
#define HW_FRAME_EXTRA_DATA			   22
#define HW_FRAME_SUB_EXTRA_DATA		   23

//���һط��ļ�����ֵ
#define HWNET_FILE_SUCCESS		1000
#define HWNET_NOFILE			1001
#define HWNET_ISFINDING			1002
#define HWNET_NOMOREFILES		1003
#define HWNET_FILE_EXCEPTION	1004

struct tServerInfo{
	int slotCount;			//ͨ������
	int sverVersion;		//�������汾
	int netVersion;			//����汾
	int reserver[32];		
};

/*osd struct*/
struct tOsdDate
{
	int slot;				//��0��ʼ
	int isEnable;			//�Ƿ���ʾ
	int left;				//0-703
	int top;				//0-575
	int type;				//		��Ч
	int isShowWeek;			//�Ƿ���ʾ����
};

/*video color struct*/
struct tVideoColor
{
	int slot;
	int brightness/*0-255*/;
	int contrast/*0-127*/;
	int saturation/*0-127*/;
	int hue/*0-255*/;
};

/*osd channel struct*/
struct tOsdChannel
{
	int slot;
	int isEnable;		//�Ƿ���ʾ
	int left;			//0-703
	int top;			//0-575
	char name[32];		//ͨ������
};

/*encode quality struct*/
struct tVideoQuality
{
	int slot;
	int encodeType; /*0-CIF, 1-D1 2-720p,3-1080p,4-1280*960,5-1024*768,0xff-qcif*/
	int qualityLev;/* 0-���,1-�κã�2-�Ϻã�3һ��*/
	int maxBps;		//0-����,1-��С 2-�� 3-�ϴ� 4-���CIF(2000,400,600,800,1000),D1(4000,800,1000,1400,1600),720p(8000,2000,2500,3000,4000)
	
	int vbr;		//0-cbr 1-vbr
	int framerate;  //0-ȫ֡�� 1-24 ʵ��֡��
	int reserved;
	//int iQuality;
	//int pQuality;
	//int bQuality;
};

/*user log password*/
struct tLogin
{	
	int type;	//��ʱδʹ��
	char logName[32];
	char logPassword[32];	
	int clientUserID;
	int reserved[32];
};

/*work sheet  2 in 1  davinci & linux*/
#define MAX_SEGMENT 3
#define MAX_DAY     7
#ifndef _WINDOWS 
typedef struct _HWSYSTEMTIME {
		short wYear;
   short wMonth;
   short wDayOfWeek;
   short wDay;
   short wHour;
   short wMinute;
   short wSecond;
   short wMilliseconds;
}HWSYSTEMTIME;
#endif
struct tSegment{
	int isSet;
	SYSTEMTIME beg;
	SYSTEMTIME end;
	int recType;//0-Real ,1-Motion
};
struct tDaySheet{
	int isSet;		//��ʱδʹ��
	struct tSegment segment[MAX_SEGMENT];
};
struct tSlotSheet{	
	struct tDaySheet sameSheet;//each day is same,��ʱδʹ��
	struct tDaySheet eachDaySheet[MAX_DAY];
};
struct tWorkSheet{
	int sheetNO;		//1:�ֶ����������ù�����
	int slotNo;
	struct tSlotSheet slotSheet;
};

/*motion*/
struct tMotion{
	int slot;
	int lev ;		/*0���-5��ͣ�6�ر�*/
	int	recDelay;	//0-6:10s,20s,30s,1m,2m,5m,10m
	int data[18];	//�ӵ�λ����λΪ0-21
};

/*mask*/
#define MAX_MASK_RECT 4
struct tMask{
	int slot;
	int count;		//�ڵ�������������4��
	int lev;		//��ʱδʹ��
	RECT rt[MAX_MASK_RECT];
};

/*rec file*/
struct tRecFile{
	int slot;
	SYSTEMTIME beg;
	SYSTEMTIME end;
	int type;/*0=all 1=normal file 2=mot file 3=alarm 4=mot&&alarm 5=ipcam lost 6=analyze metadata*/
};

/*rec data*/
struct tRecData{
	int slot;
	int type;/*0=head  1=video data*/
	int dataLen;
	char buf[2048];
};

/*video standard*/
struct tVideoStandard{
	int slot;
	int videoStandard;/*0 - pal,1-ntfs*/
};

/*video status*/
struct tVideoStatus{
	int slot;
	int recStatus;/*0- not rec, 1- rec*/
	int signalStatus;/*0 - no signal, 1- signal*/
	int motionStatus;/*0- no motion alarm, 1- motion alarm*/
};

/*alarm */
struct tAlarmData{
	int value;			//��������Ƶͨ�������ΪӲ�̱�����ΪӲ�����
	int status;	    
	int reserved[32];
};

/*center information*/
struct tCenter{
	char ip[32];
	int port;
	int listenType;/*0 - udp ,1 - tcp*/
	int reserved[32];
};

/*stream type*/
struct tStreamType{
	int slot;
	int type;/*1- video, 2-audio,3-both*/
};

struct tAlarmMotionData
{
	unsigned int slot; //��Ƶ���ںţ���0��ʼ
	long tmSec;        //ʱ�䣬��1970��1��1�ž���������
	unsigned int tmMSec;//���룬0-1000
	unsigned int data[18];//����˶�֡��Ϣ
	unsigned int reserved[4];//����
};

typedef enum
{
//---------------record_type-------------------------
  CONTINUOUS        = 1 << 0,
  MOTION            = 1 << 1,
  TRIGGER_INPUT_0   = 1 << 2,
  TRIGGER_INPUT_1   = 1 << 3,
  TRIGGER_INPUT_2   = 1 << 4,
  TRIGGER_INPUT_3   = 1 << 5,
  TRIGGER_INPUT_4   = 1 << 6,
  TRIGGER_INPUT_5   = 1 << 7,
  TRIGGER_INPUT_6   = 1 << 8,
  TRIGGER_INPUT_7   = 1 << 9,
  TRIGGER           = 1 << 10,
//---------------record_mode-------------------------
  DAILY             = 1 << 11,
  WEEKLY            = 1 << 12,
  ONCE              = 1 << 13
}RecrtdTypeFlags;


/*work sheet */
#define MAX_TEMPEST_SCHEDULE_COUNT 1000
typedef struct
{
  SYSTEMTIME beg;
  SYSTEMTIME end;
  unsigned int frame_rate;
  unsigned int record_type;   //CONTINUOUS MOTION TRIGGER,see RecrtdTypeFlags
  unsigned int record_mode;   //WEEKLY DAILY ONCE,see RecrtdTypeFlags
}record_schedule_item_t;

struct tTempestWorkSheet
{
	int slot;	
	record_schedule_item_t *sch_list; 
	int sch_count;	
};

/*user*/

/* privileges */
enum{
  USER_PRIVILEGS_GUEST,
  USER_PRIVILEGS_ADMIN,
  USER_PRIVILEGS_CUSTOM,
};
/* access flag */
enum{
  USER_ACCESS_PLAYBACK =   (1 << 0),
  USER_ACCESS_PTZ      =   (1 << 1),
  USER_ACCESS_LOGS     =   (1 << 2),
  USER_ACCESS_SCHEDULE =   (1 << 3),
  USER_ACCESS_QUALITY  =   (1 << 4),
  USER_ACCESS_MOTION   =   (1 << 5),
  USER_ACCESS_NETWORK  =   (1 << 6),
  USER_ACCESS_DELETE   =   (1 << 7),
  USER_ACCESS_TIME     =   (1 << 8),
  USER_ACCESS_ADMIN    =   (1 << 9),
  USER_ACCESS_UPGRADE  =   (1 << 10),
  USER_ACCESS_ALARM    =   (1 << 11),
  USER_ACCESS_POS      =   (1 << 20),
};

#define USER_GUEST_PGUEST_ACCESS_FLAG USER_ACCESS_PLAYBACK
#define USER_GUEST_PADMIN_ACCESS_FLAG (0xffffffff-USER_ACCESS_UPGRADE-USER_ACCESS_ADMIN-USER_ACCESS_TIME-USER_ACCESS_DELETE-USER_ACCESS_NETWORK)

typedef struct  {
  char email[50];
  char user[50];
  int num;
  int numLeft;
  int min_time;
  unsigned char motion[16];
  unsigned char input[8];
  unsigned char motion_snap;
  unsigned char input_snap;
  unsigned char net;
  unsigned char dis;
  unsigned char emailTimeRange[4];
}email_user_t;


typedef struct  {
  char name[50];
  char pass[50];
  char email[100];

  /* for encrypt */
  int encPass[50];
  int passLen;
  int passTm;

  /*
  unsigned int admin;
  unsigned int flags;
  */
  int privilege;
  unsigned int flags;
  unsigned int loggedIn;

  unsigned int access_cameras;
  unsigned int access_sound;
  unsigned int access_flags;

  //email_user_t  emailUser;
}user_t;

struct tTempestUsers{
	int userCount;
	user_t *user_item;
};


/*ptz*/
struct tPtzControl{
	int slot;
	int controlType;/*0-direct 1- len 2-AUTO zoom in,3-preset*/
	int cmd;/*direct: 7-left up
		             8-up
					 9-right up
					 4-left
					 5-stop
					 6-right
					 1-left down
					 2-down
					 3-right down

			len:     1-iris open
			         2-iris close
					 3-len tele
					 4-len wide
					 5-focus far
					 6-focus near
					 7-stop
			AUTO zoom in:
					 1-auto zoom in
			preset:
					 1-set
					 2-clear
					 3-goto
			*/
	int presetno;			//Ԥ�õ�Ż����ƶ����ٶȣ�0-64��
	RECT rect;			//����Ŵ������704��576
};

struct tRegisterAlarm{
	int bRegister;
	int listenPort;
	/*long lUserID;*/
	long reserved[4];
};

typedef enum{
	ALARM_DO_NOTHING = 0,
	ALARM_TO_MONITOR = (1<<0),
	ALARM_WITH_AUDIO = (1<<1),
	ALARM_TO_CENTER =  (1<<2),
	ALARM_TO_OUT =  (1<<3),
	ALARM_TO_RECORD = (1<<4),
	ALARM_TO_SNAP = (1<<5),
}AlarmHandleType;

typedef struct 
{ 
	int type;   /*����ʽ,����ʽ��"��"���,����AlarmHandleType*/
	byte byRelAlarmOut[HW_MAX_ALARM_OUT];  /*  �������������ͨ��,�������������,Ϊ1 ��ʾ��������� */ 
	byte byRelRecord[MAX_SLOT];// ����¼��	, ����������¼��ͨ��,Ϊ1��ʾ������ͨ�� //�ƶ������Բ�֧��
	byte byRelSnap[MAX_SLOT];// ����ץͼ			
}tHandle; 
typedef struct
{
	byte byStartHour; //0-23
	byte byStartMin; //0-59
	byte byStopHour; //0-23
	byte byStopMin; //0-59
}HW_NET_SCHEDTIME;

struct tAlarmInCfg{
	int alarmin_no;//��������� 
	int type;//���������� 0-���� 1-����
	tHandle handle;//������������
	HW_NET_SCHEDTIME schedule[MAX_DAY][MAX_SEGMENT];//���������ʱ��Σ�����Щʱ�����Ŵ�����
	int reserved[32];
};

struct tAlarminDefend{
	int alarmin_no;//��������� 
	int type;//0-��ʱ  1-�ֶ����� 2-�ֶ�����
	int reserved[32];
};

struct tAlarmOutState{
	byte state[HW_MAX_ALARM_OUT]; //�������״̬ 0:��Ч 1����Ч
	int reserved[32];
};

struct tAlarmOutCfg{
	int alarmout_no;//0xff-ȫ������������������
	byte state;//1- ��Ч��0- ��Ч
	int reserved[32];
};

//�ڵ�����
struct tAlarmMask{
	int slot;
	int enabled;//�Ƿ�����
	tHandle handle;
	int reserved[32];
};

//�����豸����
struct tRs232Cfg{
	int rs232_no;//��slotС�ڱ���ͨ������ʱ��,Ϊ���ش��ںţ�����Ϊ������ͨ����Ӧ�Ĵ��ں�
	int rate; /*  ������(bps)��0��50��1��75��2��110��3��150��4��300��5��600��6��1200��7��2400��8��4800��9��9600��10��19200�� 11��38400��12��57600��13��76800��14��115.2k;*/ 
	byte data_bit; //0-5λ 1-6λ 2-7λ 3-8λ
	byte stop_bit; //0-1λ 1-2λ
	byte parity; //0-��У�� 1-��У�� 2-żУ��
	byte flow_control;//0-�� 1-������ 2-Ӳ����
	int work_mode; //����ģʽ 0-��̨ 1-�������� 2-͸��ͨ��
	int annunciator_type;/*0-Howell 1-VISTA 120 ... */	
	int reserved[32];
};

typedef enum
{
	PELCO_D		= 0,
	PELCO_P		,
	ALEC		,
	YANAN		
}ProtocolType;

//ptz��̨����,ֻ���ڴ��ڵ�work_mode=0(��̨)ʱ��Ч
struct tPtzRs232Cfg{
	int slot;
	int rs232_no;//��slotС�ڱ���ͨ������ʱ��,Ϊ���ش��ںţ�����Ϊ������ͨ����Ӧ�Ĵ��ں�
	int rate; /*  ������(bps)��0��50��1��75��2��110��3��150��4��300��5��600��6��1200��7��2400��8��4800��9��9600��10��19200�� 11��38400��12��57600��13��76800��14��115.2k;*/ 
	byte data_bit; //0-5λ 1-6λ 2-7λ 3-8λ
	byte stop_bit; //0-1λ 1-2λ
	byte parity; //0-��У�� 1-��У�� 2-żУ��
	byte flow_control;//0-�� 1-������ 2-Ӳ����
	int protocol;//Э�����ͣ�����ProtocolType
	int address; //��ַ 0-255
	int reserve[32];
};

//͸��ͨ������,ֻ���ڴ��ڵ�work_mode=2(͸��ͨ��)ʱ��Ч
struct tRs232Send
{
	int rs232_no;//��slotС�ڱ���ͨ������ʱ��,Ϊ���ش��ںţ�����Ϊ������ͨ����Ӧ�Ĵ��ں�
	int protocol;//Э�����ͣ�����protocolType,���=0xffffffff,��������Э�飬ֻҪֱ��ת�����ɡ�
	int len;//���ݳ���
	byte data[256];
	int reserve[32];
};

struct tVideoLostCfg{
	int slot;
	tHandle handle;
	HW_NET_SCHEDTIME schedule[MAX_DAY][MAX_SEGMENT];//��Ƶ��ʧ�����ʱ��Σ�ֻ������Щʱ��βŴ���
	int reserve[32];
};

struct tVoice{
	int type; //0-�������ĵ�����)  1-˫��
	int slot;
	int reserve[31];
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#define  HW_MOTION_MAX_ROWS 128
#define  HW_MOTION_MAX_COLS 128
typedef struct{
	int slot;
	int rows;//0-HW_MOTION_MAX_ROWS
	int cols;//0-HW_MOTION_MAX_COLS
	int reserved[32];
}tMotionRowCols;

typedef struct 
{
	int slot;
	int lev ;		/*0���-5��ͣ�6�ر�*/
	int	recDelay;	//0-6:10s,20s,30s,1m,2m,5m,10m	
	char data[HW_MOTION_MAX_ROWS * HW_MOTION_MAX_COLS / 8];
	tHandle handle;
	int reserved[32];
}tMotionEx;

typedef struct //�����ϴ�
{
	unsigned int slot; //��Ƶ���ںţ���0��ʼ
	long tmSec;        //ʱ�䣬��1970��1��1�ž���������
	unsigned int tmMSec;//���룬0-1000	
	char data[HW_MOTION_MAX_ROWS * HW_MOTION_MAX_COLS / 8];
	int reserved[32];
}tAlarmMotionDataEx;

#define  MAX_SMARTSEARCH_RECT 5
typedef struct{
	tRecFile recFile;
	int rt_count;
	RECT rt[MAX_SMARTSEARCH_RECT]; //����704 * 576
	int direction;//0-ȫ��  4-�� 6-��  8-�� 2-��
	int reserved[32];
}tSmartSearchEx;

//Ӳ����Ϣ��ȡ
typedef struct{	
	INT64 volume; //Ӳ������
	INT64 free;//��������
	int state;//Ӳ��״̬ 0- ���� 1-���� 2-������
	int reserved[32];
}tEachHarddiskInfo;

#define  MAX_HARD_DISK_COUNT 16
typedef struct{
	tEachHarddiskInfo disk[MAX_HARD_DISK_COUNT];
	int reserved[32];
}tHarddiskInfo;

/*****************		lzs		****************/
/*network setting*/
struct tNetworkSetInfo {
	int		port;
	char	sDvrIp[IP_LENGTH];				//dvr ip��ַ
	char	sDvrMaskIp[IP_LENGTH];			//dvr��������
	char	gateway[IP_LENGTH];
	int		byMACAddr[6];					//�������������ַ
	char	dns[IP_LENGTH];
	char	sMultiCastIP[IP_LENGTH];		 //�ಥ���ַ
	int		dwPPPOE;						//0-������,1-����
	char	sPPPoEUser[MAX_NAME_LEN];		//PPPoE�û���
	char	sPPPoEPassword[MAX_PASS_LEN];	// PPPoE����
	char	sPPPoEIP[IP_LENGTH];			//PPPoE IP��ַ(ֻ��)
	int		reserve[32];
};

/*	log	*/
//!!!!������0��ʾȫ����1�쳣��2������3������dvr������û�С�ȫ������0�쳣��1������2����
/* ���� */
//������
#define MAJOR_ALARM				0x3
//������
#define MINOR_ALARM_IN			0x0		/* �������� */
#define MINOR_ALARM_OUT			0x1		/* ������� */
#define MINOR_MOTDET_START		0x2		/* �ƶ���ⱨ����ʼ */
#define MINOR_MOTDET_STOP		0x3		/* �ƶ���ⱨ������ */
#define MINOR_HIDE_ALARM_START	0x4		/* �ڵ�������ʼ */
#define MINOR_HIDE_ALARM_STOP	0x5		/* �ڵ��������� */
#define MINOR_ALARM_UNKOWN		0x6		/* δ֪ */

/* �쳣 */
//������
#define MAJOR_EXCEPTION			0x1
//������
#define MINOR_VI_LOST			0x7		/* �źŶ�ʧ */
#define MINOR_ILLEGAL_ACCESS	0x8		/* �Ƿ����� */
#define MINOR_HD_FULL			0x9		/* Ӳ���� */
#define MINOR_HD_ERROR			0xA		/* Ӳ�̴��� */
#define MINOR_DCD_LOST			0xB		/* MODEM ���� */
#define MINOR_IP_CONFLICT		0xC		/* IP��ַ��ͻ */
#define MINOR_EXCEPTION_UNKOWN	0xD		/* δ֪ */

/* ���� */
//������
#define MAJOR_OPERATION			0x2
//������
#define MINOR_START_DVR			0xE		/* ���� */
#define MINOR_STOP_DVR			0xF		/* �ػ� */
#define MINOR_STOP_ABNORMAL		0x10	/* �Ƿ��ػ� */
// #define	MINOR_REBOOT_DVR		0x44	/* ���� */	//lzs

#define MINOR_LOCAL_LOGIN		0x11	/* ���ص�½ */
#define MINOR_LOCAL_LOGOUT		0x12	/* ����ע����½ */
#define MINOR_LOCAL_CFG_PARM	0x13	/* �������ò��� */
#define MINOR_LOCAL_PLAYBYFILE	0x14	/* ���ذ��ļ��ط� */
#define MINOR_LOCAL_PLAYBYTIME	0x15	/* ���ذ�ʱ��ط� */
#define MINOR_LOCAL_START_REC	0x16	/* ���ؿ�ʼ¼�� */
#define MINOR_LOCAL_STOP_REC	0x17	/* ����ֹͣ¼�� */
#define MINOR_LOCAL_PTZCTRL		0x18	/* ������̨���� */
#define MINOR_LOCAL_PREVIEW		0x19	/* ����Ԥ�� */
#define MINOR_LOCAL_MODIFY_TIME	0x1A	/* �����޸�ʱ�� */
#define MINOR_LOCAL_UPGRADE		0x1B	/* �������� */
// #define MINOR_LOCAL_COPYFILE	0x5b	/* ���ر����ļ� */

#define MINOR_REMOTE_LOGIN		0x1C	/* Զ�̵�¼ */
#define MINOR_REMOTE_LOGOUT		0x1D	/* Զ��ע����½ */
#define MINOR_REMOTE_START_REC	0x1E	/* Զ�̿�ʼ¼�� */
#define MINOR_REMOTE_STOP_REC	0x1F	/* Զ��ֹͣ¼�� */
#define MINOR_START_TRANS_CHAN	0x20	/* ��ʼ͸������ */
#define MINOR_STOP_TRANS_CHAN	0x21	/* ֹͣ͸������ */
#define MINOR_REMOTE_GET_PARM	0x22	/* Զ�̻�ò��� */
#define MINOR_REMOTE_CFG_PARM	0x23	/* Զ�����ò��� */
#define MINOR_REMOTE_GET_STATUS 0x24	/* Զ�̻��״̬ */
#define MINOR_REMOTE_ARM		0x25	/* Զ�̲��� */
#define MINOR_REMOTE_DISARM		0x26	/* Զ�̳��� */
#define MINOR_REMOTE_REBOOT		0x27	/* Զ������ */
#define MINOR_START_VT			0x28	/* ��ʼ�����Խ� */
#define MINOR_STOP_VT			0x29	/* ֹͣ�����Խ� */
#define MINOR_REMOTE_UPGRADE	0x2A	/* Զ������ */
#define MINOR_REMOTE_PLAYBYFILE	0x2B	/* Զ�̰��ļ��ط� */
#define MINOR_REMOTE_PLAYBYTIME	0x2C	/* Զ�̰�ʱ��ط� */
#define MINOR_REMOTE_PTZCTRL	0x2D	/* Զ����̨���� */
#define MINOR_OPERAION_UNKOWN	0x2E	/* δ֪ */


/*����*/
#define	PARAM_VIDEO_OUT			0x0		/*��Ƶ���*/
#define PARAM_IMAGE				0x1		/*ͼ��*/
#define PARAM_ENCODE			0x2		/*����*/
#define PARAM_NETWORK			0x3		/*����*/
#define PARAM_ALARM				0x4		/*����*/
#define PARAM_EXCEPTION			0x5		/*�쳣*/
#define PARAM_DECODE			0x6		/*����*/
#define PARAM_RS232				0x7		/*RS232*/
#define PARAM_PREVIEW			0x8		/*Ԥ��*/
#define PARAM_SECURITY			0x9		/*��ȫ*/
#define PARAM_DATETIME			0xA		/*����ʱ��*/
#define PARAM_FRAMETYPE			0xB		/*֡��ʽ*/
#define PARAM_UNKOWN			0xC		/*δ֪*/


struct tLogInfo {
	SYSTEMTIME	beg;	//��ʼʱ�����־��¼ʱ��
	SYSTEMTIME	end;
	int		slot;
	int		majorType;			//�μ���������
	int		minorType;			//�μ���������
	char	local_operator[MAX_NAME_LEN];		//���ز���Ա
	char	remote_ipaddr[IP_LENGTH];			//Զ�̲���ip
	char	remote_operator[MAX_NAME_LEN];		//Զ�̲���Ա
	int		paramType;			//��������
	int		disknumber;			//Ӳ�̺�
	int		nAlarmInPort;		//��������˿�
	int		nAlarmOutPort;		//��������˿�
	char	reserve[32];
};

/*	davinci user	*/
struct Davinci_user_t {
	char	username[MAX_NAME_LEN];
	char	password[MAX_PASS_LEN];
	int		privilege;
	char	reserve[32];
};
struct tDavinciUser {
	int		user_count;
	Davinci_user_t *user;
};

/*	smart search	*/
struct tSmartSearch {
	tRecFile	recFile;
	char		searchRect[72];		//����������Χ,��λȡ,1-��������Ҫ��������Χ��
};

/* alarm state */
struct tAlarmState {
	int		alarm_in_state[HW_MAX_ALARM_IN];		//��������״̬
	int		alarm_out_state[HW_MAX_ALARM_OUT];		//�������״̬
	int		rec_state[MAX_SLOT];					//¼��״̬ 0-��¼��1-¼��
	int		signal_state[MAX_SLOT];					//��Ƶ�ź�״̬ 0-�� 1-��
	int		disk_state[HW_MAX_DISKNUM];				//Ӳ��״̬	0-���� 1-��Ӳ�� 2-Ӳ�̻� 3-Ӳ��δ��ʽ�� 4-Ӳ���� 
	int		motion_state[MAX_SLOT];					//�ƶ���� 0-û���ƶ���ⱨ�� 1-��
	int 	dsp_state[HW_MAX_DSPNUM];				//dsp״̬  0-�쳣 1-����
	char	reserve[32];
};

/* devcie configuration */
struct tDevConfig {
	char	devName[MAX_NAME_LEN];				//�豸����
	char	devSerialID[HW_MAX_SERIALID];		//�豸���к�
	int		window_count;		//������
	int		alarm_in_count;		//����������
	int		alarm_out_count;	//���������		
	int		disk_count;			//Ӳ����
	int		dsp_count;			//dsp��
	int		rs232_count;		//������
	char	channelname[MAX_SLOT][32]; // ͨ������

	int		net_channel_count;  //����ͨ����
	char	reserve[28];
};

/* upgrade file header */
struct tUpgradeFileHeader {
	char	fileName[260];	
	long	fileSize;		//��λ�ֽ�
	char	md5[33];		//md5 code, 32 byte��33Ϊ������'\0'
	char	reserve[31];
};


/* error definition */
#define HW_NET_NOERROR 					0  //û�д���
#define HW_NET_PASSWORD_ERROR 			1  //�û����������
#define HW_NET_NOENOUGHPRI 				2  //Ȩ�޲���
#define HW_NET_NOINIT 					3  //û�г�ʼ��
#define HW_NET_CHANNEL_ERROR 			4  //ͨ���Ŵ���
#define HW_NET_OVER_MAXLINK 			5  //���ӵ�DVR�Ŀͻ��˸����������
#define HW_NET_VERSIONNOMATCH			6  //�汾��ƥ��
#define HW_NET_NETWORK_FAIL_CONNECT		7  //���ӷ�����ʧ��
#define HW_NET_NETWORK_SEND_ERROR		8  //�����������ʧ��
#define HW_NET_NETWORK_RECV_ERROR		9  //�ӷ�������������ʧ��
#define HW_NET_NETWORK_RECV_TIMEOUT		10 //�ӷ������������ݳ�ʱ
#define HW_NET_NETWORK_ERRORDATA		11 //���͵���������
#define HW_NET_ORDER_ERROR				12 //���ô������
#define HW_NET_OPERNOPERMIT				13 //�޴�Ȩ��
#define HW_NET_COMMANDTIMEOUT			14 //DVR����ִ�г�ʱ
#define HW_NET_ERRORSERIALPORT			15 //���ںŴ���
#define HW_NET_ERRORALARMPORT			16 //�����˿ڴ���
#define HW_NET_PARAMETER_ERROR 			17  //��������
#define HW_NET_CHAN_EXCEPTION			18	//������ͨ�����ڴ���״̬
#define HW_NET_NODISK					19	//û��Ӳ��
#define HW_NET_ERRORDISKNUM				20	//Ӳ�̺Ŵ���
#define HW_NET_DISK_FULL				21	//������Ӳ����
#define HW_NET_DISK_ERROR				22	//������Ӳ�̳���
#define HW_NET_NOSUPPORT				23	//��������֧��
#define HW_NET_BUSY						24	//������æ
#define HW_NET_MODIFY_FAIL				25	//�������޸Ĳ��ɹ�
#define HW_NET_PASSWORD_FORMAT_ERROR	26	//���������ʽ����ȷ
#define HW_NET_DISK_FORMATING			27	//Ӳ�����ڸ�ʽ����������������
#define HW_NET_DVRNORESOURCE			28	//DVR��Դ����
#define	HW_NET_DVROPRATEFAILED			29  //DVR����ʧ��
#define HW_NET_OPENHOSTSOUND_FAIL 		30  //��PC����ʧ��
#define HW_NET_DVRVOICEOPENED 			31  //�����������Խ���ռ��
#define	HW_NET_TIMEINPUTERROR			32  //ʱ�����벻��ȷ
#define	HW_NET_NOSPECFILE				33  //�ط�ʱ������û��ָ�����ļ�
#define HW_NET_CREATEFILE_ERROR			34	//�����ļ�����
#define	HW_NET_FILEOPENFAIL				35  //���ļ�����
#define	HW_NET_OPERNOTFINISH			36  //�ϴεĲ�����û�����
#define	HW_NET_GETPLAYTIMEFAIL			37  //��ȡ��ǰ���ŵ�ʱ�����
#define	HW_NET_PLAYFAIL					38  //���ų���
#define HW_NET_FILEFORMAT_ERROR			39  //�ļ���ʽ����ȷ
#define HW_NET_DIR_ERROR				40	//·������
#define HW_NET_ALLOC_RESOUCE_ERROR		41  //��Դ�������
#define HW_NET_AUDIO_MODE_ERROR			42	//����ģʽ����
#define HW_NET_NOENOUGH_BUF				43	//������̫С
#define HW_NET_CREATESOCKET_ERROR		44	//����SOCKET����
#define HW_NET_SETSOCKET_ERROR			45	//����SOCKET����
#define HW_NET_MAX_NUM					46	//�����ﵽ���
#define HW_NET_USERNOTEXIST				47	//�û�������
#define HW_NET_WRITEFLASHERROR			48  //дFLASH����
#define HW_NET_UPGRADEFAIL				49  //DVR����ʧ��
#define HW_NET_CARDHAVEINIT				50  //���뿨�Ѿ���ʼ����
#define HW_NET_PLAYERFAILED				51	//�������д���
#define HW_NET_MAX_USERNUM				52  //�û����ﵽ���
#define HW_NET_GETLOCALIPANDMACFAIL		53  //��ÿͻ��˵�IP��ַ�������ַʧ��
#define HW_NET_NOENCODEING				54	//��ͨ��û�б���
#define HW_NET_IPMISMATCH				55	//IP��ַ��ƥ��
#define HW_NET_MACMISMATCH				56	//MAC��ַ��ƥ��
#define HW_NET_UPGRADELANGMISMATCH		57	//�����ļ����Բ�ƥ��
#define HW_NET_DDRAWDEVICENOSUPPORT		58	//�����Կ���֧��
#define HW_NET_UPGRADEOCCUPIED			59	//������������Դ��ռ��
#define HW_NET_VOICE_EXIST				60 //�����Խ��Ѿ�����
#define HW_NET_MAX_LIVE_NUM				61 //Ԥ�������ﵽ���

//ipcam ͨ������
struct tChannelSet{
	int		slot;
	char username[32];	// �û���
	char password[32];	// ����
	char ipaddress[32];	// IP��ַ
	char macaddress[32];	// MAC��ַ
	int nport; 		// �˿ں�
	int nchannelnum;	// ���ӵ�ͨ����
};

/*
˵����
��Բ�ͬ��ntype��Чֵ��ͬ��
ntypeΪ0��howellЭ���IPC�豸��
username���û���
password������
ipaddress��IP��ַ
nport���˿ں�
nchannelnum�����ӵ�ͨ����

ntypeΪ1��ONVIF�豸

ntypeΪ2��RTSPЭ���IPC�豸��
ipaddress����������RTSP��URL(rtsp://[username[:password]@]ip_address[:rtsp_port]/server_URL[?param1=val1[&param2=val2]...[&paramN=valN]])
profilename����������RTSP��URL
busingtcp����TCP/UDP���ӻ����
*/
struct tChannelSetEx{
	int		slot;
	int     ntype;           // 0--howell device, 1--onvif device, 2--RTSP
	char    username[128];	// �û���
	char    password[128];	// ����
	char    ipaddress[256];	// IP��ַ  //serviceaddress
	char    profilename[128];   //profilename
	int     nport; 		    // �˿ں�
	int     nchannelnum;	// ���ӵ�ͨ����
	int     busingtcp; //bStreamUsingTcp
};

typedef enum {
	IPCAM_ESHUTTER_1_SEC=0, /* 1 sec */
	IPCAM_ESHUTTER_10_SEC, /* 1/10 sec */
	IPCAM_ESHUTTER_12_SEC, /* 1/12 sec */
	IPCAM_ESHUTTER_15_SEC,
	IPCAM_ESHUTTER_20_SEC,
	IPCAM_ESHUTTER_25_SEC,
	IPCAM_ESHUTTER_30_SEC,
	IPCAM_ESHUTTER_50_SEC,
	IPCAM_ESHUTTER_60_SEC,
	IPCAM_ESHUTTER_100_SEC0,
	IPCAM_ESHUTTER_120_SEC,
	IPCAM_ESHUTTER_240_SEC,
	IPCAM_ESHUTTER_480_SEC,
	IPCAM_ESHUTTER_960_SEC,
	IPCAM_ESHUTTER_1024_SEC,
	IPCAM_ESHUTTER_COUNT,
}HW_IPCAM_ESHUTTER_E;

typedef enum {
	HW_WB_INCANDESCENT=0,
	HW_WB_D4000,
	HW_WB_D5000,
	HW_WB_SUNNY,
	HW_WB_CLOUDY,
	HW_WB_FLASH,
	HW_WB_FLUORESCENT_WARM,
	HW_WB_FLUORESCENT_COLD,
	HW_WB_COUNT,
}HW_IPCAM_WB_E;

struct tIPCam_feature{
	int		slot;
	int		bae;		//�Ƿ��Զ��ع�
	int		eshutter;//����HW_IPCAM_ESHUTTER_E
	int		agcgain;//0-30
	int		blackwhitemode;		//��ת��
	int		badj;		//�Զ�����
	int		blacklevel;		//�ڵ�ƽ[-128,128]��badj��Чʱ�ò�����Ч

	int     bAutoWB; //�Զ���ƽ��
	int     wb_mode; //��ƽ��ģʽ
	int     rgb_gain[3];//rgb���� 0-16384

	int		noisefilter;		//��������0-5��5���badj��Чʱ�ò�����Ч
	int		sharp;				//���0-5��5���badj��Чʱ�ò�����Ч
	
	int		luma;				//����
	
	int		reserved[13];
};

//ǿ������,��ǰֻ��enable��max_luma_night ����
struct tBlackMaskBLC{
	int slot;

	int enable;
	
	/* ҹ���luma���ֵ,С�ڸ�ֵ������������� */
	int max_luma_night;
	
	/* ������ԭ����block��yֵӦ���ڴ˷�Χ��  */
	int block_y_max;
	int block_y_min;
	
	/* ���������yֵС�ڸ�ֵ��˵�������ȥ����Ҫ�ָ��Զ��ع� */
	int block_y_night_normal;
	
	int mode; //0: �̶����� 1:�̶����� 2:�Զ�
	int fixedAgc;
	int fixedEshutter;
	
	int max_eshutter;
	int min_eshutter;
	int max_agc;
	int min_agc;
	
	/* 
	* ����ѡ��3x3��9������ ��λ��ʾѡ���ѡ��
	* �� (1<<0|1<<3) ��ѡ���˵�1������͵�4������
	*/
	int area_map; 
	
	/* �������� */
	/* 0: nothing to set */
	/* 1<<0: enable */
	/* 1<<1: area_map */
	/* 1<<2: max_luma_night */
	/* 1<<3: block_y_max */
	/* 1<<4: block_y_min */
	/* 1<<5: block_y_night_normal */
	/* 1<<6: mode */
	/* 1<<7: fixedAgc */
	/* 1<<8: fixedEshutter */
	/* 1<<9: max_eshutter */
	/* 1<<10: min_eshutter */
	/* 1<<11: max_agc */
	/* 1<<12: min_agc */
	int set_mask;
	
	char reserve[32];
	
};



//ͨ��״̬��ΪNVR׼��
struct tChannelStatus{
	byte status[MAX_SLOT]; //0-δ���� 1-����
	int reserved[32];
};

//ͨ�����ͣ�ΪNVR�豸�ṩ
struct tChannelType{
	int		slot;
	int		slottype;	//ʵ��ͨ���汾��
	int		reserved[32];
};

#define  HW_MEDIA_TAG 0x48574D49
typedef enum{
	VDEC_H264 = 0,	
	ADEC_G711U = 1,
	ADEC_HISG711A = 2,
	VDEC_HISH264 = 3,
	ADEC_HISG711U = 4,
	ADEC_HISADPCM = 5,
	VDEC_MJPEG = 6,
}HW_DEC_TYPE;
typedef struct 				
{
	unsigned int    media_fourcc;			// "HKMI": 0x484B4D49 Hikvision Media Information,"HWMI":0x48574D49
	long dvr_version;
	long vdec_code;
	long adec_code; 

	unsigned char au_bits; // 8,16...
	unsigned char au_sample;//Kbps 8,16,64
	unsigned char au_channel;//1,2
	unsigned char hd_ver;//�汾�����Ϊ1,������ȷ��дw,h,fr

	unsigned char fr;//ʵ��֡��
	char reserve[3];
	unsigned short w;//��Ƶ��
	unsigned short h;//��Ƶ��	

	unsigned int    reserved[3];            // ����
}HW_MEDIAINFO;


//�ļ���Ϣ
struct tRecFileInfo{
	int		slot;
	SYSTEMTIME beg;
	SYSTEMTIME end;
	int		fileformattype;	//�ļ������ʽ���ͣ��μ�HW_DEVICE_TYPE
	int video_dec;
	int audio_dec;

	int ext_flag;//���Ϊ1,�����media_info������Ч
	HW_MEDIAINFO media_info;

	int		reserved[19];
};

struct tSubChannelSet{
	int slot;
	int used; //�Ƿ�������������0-������ 1-����,�������ã����������Ч

	int encodeType; /*0-CIF,1-D1,2-720p 3-1080p,0xff-qcif*/
	int qualityLev;/* 0-���,1-�κã�2-�Ϻã�3һ��*/
	int maxBps;		//0-����,1-��С 2-�� 3-�ϴ� 4-���CIF(2000,400,600,800,1000),D1(4000,800,1000,1400,1600),qcif(600,100,200,300,400)
	int vbr;		//0-cbr 1-vbr
	int framerate;  //0-ȫ֡�� 1-24 ʵ��֡��

	int reserve[256];
};

/*����Ǩ��*/
struct tLostDataTransfer{
	int slot;
	
	int enable;
	
	/* ¼���������� 0:������ 1:������*/
	int stream; 
	
	/* ¼��֡��,0: ��֡�� ,1-24ʵ��֡��*/
	int framerate; 
	
	/* �Ƿ�ѭ������,���ڱ������������� */
	int b_loop_write;
	
	char reserve[32];
	
};

typedef enum{
	WIN_D1 = 0,
	WIN_720p = 1,
	WIN_1080p = 2
}DECODER_WIN_FORMAT;
typedef struct{
	int win_idx;//�������
	char ip[32];
	int slot;
	int port;
	int stream_type;//�豸������������0-d1 1-720p 2- 1080p
	int reserv3[32];
}net_win_video_t;

typedef struct {
	char mode;	//0: day	1:night
	char reserve[32];
} net_test_filter_t;

typedef struct  
{
	int enable;
	int reserve[32];
}net_raid_t;

typedef struct 
{
	int id;              //array id
	int total_space;      //MB
	int free_space;         
	int backup_space; 
	int total_pids;            
	int bga;   //0 : none  1: rebuilding 2:migration 3
	int state; //0 ������1-��
	int rebuild_pos;
	int reserve[31];
}net_raid_state_t;

typedef struct  
{
	int slot;
	int is_sub;
	char buf[128];
	int len;
}net_head_t;

// ¼��Ӳ����Ϣ
struct tDisksInfo{
	int disk_counts; // ����¼���Ӳ����Ŀ
	int file_counts[64]; // ÿ��Ӳ�̵����¼���ļ���
	int badfile_counts[64]; // ÿ��Ӳ���л����ļ���
	int  bwritten[64]; // ����������д����	
	int reserved[64];
};

typedef struct {
    char ip[IP_LENGTH];
    int cycle;			/* 1~300s */
    char reserve[32];
} NetSnmpCfg;

typedef struct{
	int enable;
	int port;
	char ip[32];
	int reserve[32];
}net_udp_info;

typedef struct{
	int port;
	char ip[32];
	int slot;
	int is_sub;
	int reserve[32];
}udp_live_info_t;

struct tAutoFocus{
	int slot;
	char custom_focus;	/*1 ָ������0��ָ������*/
	//��Ϊ8 * 5
	char x;//0-7
	char y;//0-4
	char w;//1-7
	char h;//1-5
	char reserve[32];
};

/* �豸ID���� */
struct tSerialID {
	char serial_id[32];
	char reserve[32];
};

typedef struct 
{
	int slot;
	int enable;
	int scene_changed_threso; //�����仯��ֵ1-100
	int scene_occluded_threso;//�����ڵ���ֵ1-100
	int focus_changed_threso;//����仯��ֵ1-100
	char reserve[32];
}net_analyze_t;

typedef struct  
{
	int slot;//ͨ��
	int stream;//0:������ 1:������
	int enable;//1:���� 0:������
	int left;//(0-703)
	int top;//(0-575)
	int color;//0:��ɫ 1:��ɫ 2:��ɫ 3:��ɫ 4:��ɫ 5:��ɫ 6:��ɫ
	int outline;//0:������ 1:����
	int font_size;//�����С
	int aplha;//͸��ֵ,255��͸��,0ȫ͸��
	int show_week;//1:��ʾ���� 0:����ʾ����
	int mode;//0: YYYY-MM-DD HH:MM:SS 1:MM-DD-YYYY HH:MM:SS
	int reserve[4];
}net_custom_osd_date_t;

typedef struct  
{
	int slot;//ͨ��
	int stream;//0:������ 1:������
	int enable;//1:���� 0:������
	int left;//(0-703)
	int top;//(0-575)
	int color;//0:��ɫ 1:��ɫ 2:��ɫ 3:��ɫ 4:��ɫ 5:��ɫ 6:��ɫ
	int outline;//0:������ 1:����
	int font_size;//�����С
	int aplha;//͸��ֵ,255��͸��,0ȫ͸��
	char name[512];//����,֧�ֻ��У������Ҫ��ʾ������&�����ƣ���"row1:123&row2:456"�ͻ���ʾ����.
	int reserve[4];//����������Ϊ0
}net_custom_osd_name_t;

typedef struct
{
	int slot;//ͨ��
	int enable_noise_filter;//1:������������  0:��������������
	int noise_filter_value;//��������ֵ(��ǰ��Ч��
	int enable_echo_cancel;//1:������������  0:��������������(��ǰ��Ч)
	int echo_delay;//����������ʱ,��λms(��ǰ��Ч)
	int reserve[4];//����������Ϊ0
}net_audio_enhance_t;

typedef struct  
{
	int control_mode;//0-�ֶ� 1-�Զ�
	int sense;//������(1-3)���Զ�ģʽ��������
	int blackwhite;//0-��ɫ 1-�ڰ�,���ֶ�ģʽ��������
	int slot;//ͨ����
	char reserve[56];//����
}net_blackwhite_t;

typedef struct  
{
	int flag;
	char is_flip;
	char enable_lowest_shutter;
	char shutter;
	char noise_filter_auto;
	char noise_filter_level;
	char sharp_auto;
	char sharp_level;
	char envinrenment;
	char enable_uppest_agc;
	char agc_upper_limit;
	char gama;
	char reserve[53];
}net_ipcam_misc_t;

typedef struct  
{
	int gpio;//gpio ��
	int value;//0:�� 1:��
	char reserve[32];
}net_gpio_ctrl_t;

typedef struct  
{
	int slot;
	int mode;//0-NVR��������¼�� 1-ʵʱ¼�� 2-��¼�� 3-�ƶ����¼��
	int stream;
	int reserve[4];
}net_record_ctrl_t;

typedef struct  
{
	int slot;
	int clock_wise;//0-����ת 1-��ת90��
	int reserve[4];
}net_encode_rotate_t;

typedef struct  
{
	unsigned char vout_id;//ģ�����id,��ǰΪ0
	unsigned char enable;//1-���� 0-ֹͣ
	unsigned char reserve[64];
}net_vout_t;

typedef struct
 {
	 int slot;
	char buf[255];
	 int len; 
	 char reserve[4];
}net_rfid_alarm_t;

#endif
