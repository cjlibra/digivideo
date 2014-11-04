#ifndef hw_sadp_include_h
#define hw_sadp_include_h

#if (defined(WIN32) || defined(_WIN32_WCE))	
#ifdef HWSADP_EXPORTS
#define HWSADP_API __declspec(dllexport)
#else
#define HWSADP_API __declspec(dllimport)
#endif
#else
#define HWSADP_API
#define CALLBACK
#endif

#define MAX_CHAR_VERSION		40
#define MAX_CHAR_SERIES			40
#define MAX_CHAR_MAC			32
#define MAX_CHAR_IP			32
#define MAX_CHAR_NETMASK		32

typedef struct ServerInfo{
	char version[MAX_CHAR_VERSION];			//版本
	char series[MAX_CHAR_SERIES];			//序列号
	char MAC[MAX_CHAR_MAC];			//物理MAC地址
	char IP[MAX_CHAR_IP];			//IP地址
	char subnetmask[MAX_CHAR_NETMASK];		//子网掩码
	unsigned int dev_type;		//设备类型
	unsigned int port;			//端口号
	unsigned int enccnt;		//通道数量
	unsigned int hdiskcnt;		//磁盘数量
}ServerInfo;


HWSADP_API int hwsadp_init();
HWSADP_API int hwsadp_release();
HWSADP_API int hwsadp_settype(int ntype);
HWSADP_API int hwsadp_setsenddata(struct ServerInfo* psinfo);
HWSADP_API int hwsadp_setdevicename(char *pdevicename);
HWSADP_API int hwsadp_inquiry();
HWSADP_API int hwsadp_sethandle(void(CALLBACK *HandleDataCallBack)(struct ServerInfo* psinfo,int result));
HWSADP_API int hwsadp_modifydata(char *pbuf, int nlen);
HWSADP_API int hwsadp_setmodifyhandle(void(CALLBACK *HandleRec)(char *pbuf));
HWSADP_API int hwsadp_start();
HWSADP_API int hwsadp_stop();

#endif
