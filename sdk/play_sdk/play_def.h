#ifndef play_include_h
#define play_include_h

#define HOWELL_PLAY_API	extern "C"__declspec(dllexport)
typedef unsigned long PLAY_HANDLE;
typedef unsigned long MOTION_HANDLE;

typedef void (CALLBACK draw_callback)(PLAY_HANDLE handle,
									  HDC hDc,
									  LONG nUser);
typedef void (CALLBACK yuv_callback)(PLAY_HANDLE handle, 
									 const unsigned char* yuv,
									 int len,
									 int width,
									 int height,
									 INT64 time,
									 long user);

typedef void (CALLBACK yuv_callback_ex)(PLAY_HANDLE handle,
									 const unsigned char* y,
									 const unsigned char* u,
									 const unsigned char* v,
									 int y_stride,
									 int uv_stride,
									 int width,
									 int height,
									 INT64 time,
									 long user);

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



#ifndef INVALID_HANDLE
#define  INVALID_HANDLE (-1)
#endif

HOWELL_PLAY_API PLAY_HANDLE __stdcall hwplay_open_stream(const char* head,int head_len,int buf_len,int open_mode);
HOWELL_PLAY_API PLAY_HANDLE __stdcall hwplay_open_local_file(const char* file_name);
HOWELL_PLAY_API PLAY_HANDLE __stdcall hwplay_open_net_file(const char*ip,short port,const char* log_name,const char* log_psw,int slot,SYSTEMTIME beg,SYSTEMTIME end);
HOWELL_PLAY_API PLAY_HANDLE __stdcall hwplay_open_net_live(const char*ip,short port,const char* log_name,const char* log_psw,int slot,int is_sub);
HOWELL_PLAY_API BOOL __stdcall hwplay_play(PLAY_HANDLE handle, HWND hWnd);
HOWELL_PLAY_API BOOL __stdcall hwplay_stop(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_input_data(PLAY_HANDLE handle, const char* buf,int len);
HOWELL_PLAY_API BOOL __stdcall hwplay_open_sound(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_close_sound(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_is_sound_open(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_pause(PLAY_HANDLE handle,BOOL bpause);
HOWELL_PLAY_API BOOL __stdcall hwplay_is_pause(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_speed(PLAY_HANDLE handle,float* speed);
HOWELL_PLAY_API BOOL __stdcall hwplay_set_speed(PLAY_HANDLE handle,float speed);
HOWELL_PLAY_API BOOL __stdcall hwplay_refresh(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_save_to_bmp(PLAY_HANDLE handle,const char* bmp_path);
HOWELL_PLAY_API BOOL __stdcall hwplay_save_to_jpg(PLAY_HANDLE handle,const char* jpg_path,int quality);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_yuv_source_data(PLAY_HANDLE handle,int left,int top,int w,int h,char* dst_buf,int buf_len);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_video_size(PLAY_HANDLE handle,int* w, int* h);
HOWELL_PLAY_API BOOL __stdcall hwplay_step_froward(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_step_back(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_register_draw_fun(PLAY_HANDLE handle,draw_callback* fun,long user_data);
HOWELL_PLAY_API BOOL __stdcall hwplay_register_yuv_callback(PLAY_HANDLE handle,yuv_callback* fun,long user_data);
HOWELL_PLAY_API BOOL __stdcall hwplay_register_yuv_callback_ex(PLAY_HANDLE handle,yuv_callback_ex* fun,long user_data);
HOWELL_PLAY_API BOOL __stdcall hwplay_zoom(PLAY_HANDLE handle,BOOL bzoom);
HOWELL_PLAY_API BOOL __stdcall hwplay_is_zoom(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_control_zoom(PLAY_HANDLE handle,int command);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_pos(PLAY_HANDLE handle,int* pos);
HOWELL_PLAY_API BOOL __stdcall hwplay_set_pos(PLAY_HANDLE handle,int pos);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_played_msec(PLAY_HANDLE handle,int* msec);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_total_msec(PLAY_HANDLE handle,int* totalmsec);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_current_frame(PLAY_HANDLE handle,int* frame_num);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_total_frame(PLAY_HANDLE handle,int* frame_num);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_stream_buf_len(PLAY_HANDLE handle, int* buf_len);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_stream_buf_remain(PLAY_HANDLE handle,int* buf_remain);
HOWELL_PLAY_API BOOL __stdcall hwplay_realloc_stream_buf_len(PLAY_HANDLE handle, int new_buf_len);
HOWELL_PLAY_API BOOL __stdcall hwplay_clear_stream_buf(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_throw_b(PLAY_HANDLE handle,int throw_num);
HOWELL_PLAY_API BOOL __stdcall hwplay_throw_p(PLAY_HANDLE handle,BOOL bthrow);
HOWELL_PLAY_API BOOL __stdcall hwplay_wait_sync(PLAY_HANDLE handle,BOOL bwait);
HOWELL_PLAY_API BOOL __stdcall hwplay_auto_adjust_size(PLAY_HANDLE handle,BOOL bauto);
HOWELL_PLAY_API BOOL __stdcall hwplay_zoom_rect(PLAY_HANDLE handle,BOOL benable,RECT* dst,RECT* src);
HOWELL_PLAY_API BOOL __stdcall hwplay_set_frame(PLAY_HANDLE handle,int frame_num);
HOWELL_PLAY_API BOOL __stdcall hwplay_save_hwnd_to_file(PLAY_HANDLE handle,const char* file_name,int file_type,RECT* rt);
HOWELL_PLAY_API BOOL __stdcall hwplay_save_src_to_file(PLAY_HANDLE handle,const char* file_name,int file_type,RECT* rt);
HOWELL_PLAY_API BOOL __stdcall hwplay_start_cut_file(const char* src_file,int beg_frame,int end_frame,const char* dst_file);
HOWELL_PLAY_API BOOL __stdcall hwplay_start_cut_file_by_time(const char* src_file,int beg_sec,int end_sec,const char* dst_file);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_cut_pos(int* pos);
HOWELL_PLAY_API BOOL __stdcall hwplay_stop_cut_file();
HOWELL_PLAY_API BOOL __stdcall hwplay_get_framenum_in_buf(PLAY_HANDLE handle,int* frame_num);
HOWELL_PLAY_API BOOL __stdcall hwplay_clear_framenum_in_buf(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_max_framenum_in_buf(PLAY_HANDLE handle,int* max_frame_num);
HOWELL_PLAY_API BOOL __stdcall hwplay_set_max_framenum_in_buf(PLAY_HANDLE handle,int max_frame_num);
HOWELL_PLAY_API BOOL __stdcall hwplay_start_join_file(const char* src_file,const char* dst_file);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_join_pos(int* pos);
HOWELL_PLAY_API BOOL __stdcall hwplay_stop_join_file();

//智能搜索支持
HOWELL_PLAY_API MOTION_HANDLE __stdcall hwplay_start_motion_search(PLAY_HANDLE handle,RECT search_rt);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_next_motion(MOTION_HANDLE handle,motion_node* mt_node,int* err_code);
HOWELL_PLAY_API BOOL __stdcall hwplay_stop_motion_search(MOTION_HANDLE handle);

//osd
HOWELL_PLAY_API BOOL __stdcall hwplay_get_osd(PLAY_HANDLE handle, SYSTEMTIME* sys);
HOWELL_PLAY_API BOOL __stdcall hwplay_set_osd(PLAY_HANDLE handle,SYSTEMTIME sys);
HOWELL_PLAY_API BOOL __stdcall hwplay_get_frame_osd(PLAY_HANDLE handle,int frame,SYSTEMTIME* sys);

//播放模式
HOWELL_PLAY_API BOOL  __stdcall hwplay_init_draw(BOOL used3d);

//图像处理
//色彩调节,参数范围(0-100)
HOWELL_PLAY_API BOOL  __stdcall hwplay_start_color_adjust(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL  __stdcall hwplay_set_color_value(PLAY_HANDLE handle,int bright,int contrast,int saturation,int hue);
HOWELL_PLAY_API BOOL  __stdcall hwplay_get_color_value(PLAY_HANDLE handle,int* bright,int* contrast,int* saturation,int* hue);
HOWELL_PLAY_API BOOL  __stdcall hwplay_get_color_default_value(PLAY_HANDLE handle,int* bright,int* contrast,int* saturation,int* hue);
HOWELL_PLAY_API BOOL  __stdcall hwplay_stop_color_adjust(PLAY_HANDLE handle);
//黑白模式
HOWELL_PLAY_API BOOL  __stdcall hwplay_start_balck_mode(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL  __stdcall hwplay_stop_black_mode(PLAY_HANDLE handle);
//锐度调节,参数范围(0-100)
HOWELL_PLAY_API BOOL  __stdcall hwplay_start_sharpen_adjust(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL  __stdcall hwplay_set_sharpen_value(PLAY_HANDLE handle,int sharpen);
HOWELL_PLAY_API BOOL  __stdcall hwplay_get_sharpen_value(PLAY_HANDLE handle,int* sharpen);
HOWELL_PLAY_API BOOL  __stdcall hwplay_get_sharpen_default_value(PLAY_HANDLE handle,int* sharpen);
HOWELL_PLAY_API BOOL  __stdcall hwplay_stop_sharpen_adjust(PLAY_HANDLE handle);
//梯度调节
HOWELL_PLAY_API BOOL  __stdcall hwplay_start_gradient_adjust(PLAY_HANDLE handle);
HOWELL_PLAY_API BOOL  __stdcall hwplay_set_gradient_mode(PLAY_HANDLE handle,BOOL bcolor,BOOL bcartoon);
HOWELL_PLAY_API BOOL  __stdcall hwplay_stop_gradient_adjust(PLAY_HANDLE handle);

#endif