#ifndef play_type_h
#define play_type_h

#ifndef WIN32
#include "hw_config.h"
#endif

enum{
	ZOOM_LEFT = 4,
	ZOOM_RIGHT = 6,
	ZOOM_UP = 8,
	ZOOM_DOWN = 2,
	ZOOM_STOP = 5,
};

enum{
	PLAY_LIVE = 0,
	PLAY_FILE = 1,
};

enum{
	FILE_BMP         = 0,
	FILE_JPG         = 1,
};

enum{
	MOTION_RESULT_SUCCESS = 0,
	MOTION_RESULT_NO_MORE_DATA = 1,
	MOTION_RESULT_INVALID_HANDLE = 2,
	MOTION_RESULT_INVALID_PARAM = 3,
};

typedef struct{
	int row;
	int col;
	int time;
	int frame_idx;
	char data[72];
}motion_node;

typedef struct{
	unsigned long time;//�ڲ�ʱ��
	unsigned long time2;
	long sys_time;//ϵͳʱ��ʱ��(1970�����ڵ�utcʱ��)
	int frame_idx;//֡���(��1��ʼ)
	long file_pos;//�ļ�λ��
	long i_file_pos;//i֡λ��
	int frame_type;//֡���� 0:P֡ 1:I֡
	int msec;//ʱ��(ms)
}frame_info;

#endif
