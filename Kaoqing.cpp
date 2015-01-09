// Kaoqing.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "net_demo.h"
#include "Kaoqing.h"
#include "afxdialogex.h"
#include "net_demoDlg.h"
#include "net_video_test.h"
#include "sqlite3.h"
//#include "sqlite3ext.h"

#include "DlgRenyuan.h"
#include "IphostDlg.h"


// CKaoqing �Ի���
static USER_HANDLE g_server_id = INVALID_HANDLE;
static   int g_window_count = 0;
static  long g_server_version = 0;
static   char g_server_ip[32];
static net_video_test* test = NULL;
static UINT64 g_recved_len = 0;
static UINT64 g_total_len = 0;
static CKaoqing *thisp = NULL;
static CString imgPathlast="";
static int onlyone = 0;

static char filemovie[256];

static CString username_db, password_db;
static PLAY_HANDLE HANDLEL;

static void CALLBACK draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser);
static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName);
static void CALLBACK hw_stream_callback(PLAY_HANDLE handle,
		INT64 stime,
		int type,
		char* buf,
		int len,
		long user);

typedef struct _LABELINFO{
	CString sLabel;
    SYSTEMTIME timeLast;
	int  labelleft;
	int id;

}LABELINFO;
static LABELINFO labelinfo[1024];

static __int64	TimeDiff(SYSTEMTIME  left,SYSTEMTIME  right)  ;



IMPLEMENT_DYNAMIC(CKaoqing, CDialogEx)

CKaoqing::CKaoqing(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKaoqing::IDD, pParent)
{

}

CKaoqing::~CKaoqing()
{
	 sqlite3_close(db);
}

BOOL CKaoqing::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	for (int i=0;i<1024;i++){
		labelinfo[i].sLabel = "";
//		labelinfo[i].timeLast=0;
		labelinfo[i].labelleft = 0;
		labelinfo[i].id = 0;

	}
	SetTimer(3,1000*10,NULL);

	thisp = this;

	m_ip_kaoqing.SetAddress(192,168,128,248);
	UpdateData(FALSE);
	HW_NET_Init(5198);	
	HW_NET_SetUdpBasePort(8000);
	
	//search device
#if 0
	hwsadp_init();
	hwsadp_settype(0);
	//hwsadp_sethandle(HandleDataCallBack);
	hwsadp_start();
#endif
	int ret = 1;
	db = 0;
	ret = sqlite3_open("medb.s3db", &db);
      
    if ( ret != SQLITE_OK )
    {
      fprintf(stderr, "�޷������ݿ�: %s", sqlite3_errmsg(db));
      return(1);
    }
      
    printf("���ݿ����ӳɹ�!\n");
	return TRUE;
}
void CKaoqing::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKaoqing)
	DDX_Control(pDX, IDC_IPADDRESS_KAOQING, m_ip_kaoqing);	
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC1, m_PictureControl);
	DDX_Control(pDX, IDC_USERNAME, m_username);
	DDX_Control(pDX, IDC_PASSWORD, m_password);
	DDX_Control(pDX, IDC_COMBO1, m_combochanctrl);
}


BEGIN_MESSAGE_MAP(CKaoqing, CDialogEx)
	ON_BN_CLICKED(IDC_BTLOGIN_KAOQING, &CKaoqing::OnBnClickedBtloginKaoqing)
	ON_BN_CLICKED(IDC_BUTTON1, &CKaoqing::OnBnClickedButton1)
	ON_WM_PAINT()
	
	ON_BN_CLICKED(IDOK, &CKaoqing::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CKaoqing::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, &CKaoqing::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CKaoqing::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CKaoqing ��Ϣ�������


void CKaoqing::OnBnClickedBtloginKaoqing()
{
	
	this->m_combochanctrl.ResetContent();
	UpdateData(FALSE);

	if(g_server_id != INVALID_HANDLE)
	{
		//KillTimer(TIMER_HEARTBEAR);
		HW_NET_Logout(g_server_id);
		g_server_id = INVALID_HANDLE;
	}

	if(g_server_id == INVALID_HANDLE)
	{
		char ip[255];
		BYTE f0,f1,f2,f3;
		m_ip_kaoqing.GetAddress(f0,f1,f2,f3);
		sprintf(ip,"%d.%d.%d.%d",f0,f1,f2,f3);
		sprintf(g_server_ip,"%d.%d.%d.%d",f0,f1,f2,f3);
		g_server_id = HW_NET_Login(ip,5198,"admin","12345");
		if(g_server_id != INVALID_HANDLE)
		{
		//	m_slots.ResetContent();	
			g_window_count = HW_NET_GetWindowCount(g_server_id);			
			char str[255];
			
			for(int i = 0; i < g_window_count; i++)
			{
				sprintf(str,"ͨ��%d",i + 1);
			 	this->m_combochanctrl.AddString(str);
			}
		 	this->m_combochanctrl.SetCurSel(0);
			
		//	m_svr_type.ResetContent();	
			
			HW_NET_SET_GetDvrVersion(g_server_id,&g_server_version);
/*
			if(VERSION_IPCAM(g_server_version)){
				m_svr_type.AddString("ip camera");
			}else if(VERSION_NVR(g_server_version)){
				m_svr_type.AddString("nvr");
			}else if(VERSION_HIS(g_server_version)
				|| VERSION_HIS_RAILWAY(g_server_version)){
				m_svr_type.AddString("his");
			}
			m_svr_type.SetCurSel(0);
*/
			HW_NET_SET_RegisterAlarm(g_server_id,1);  //m_enable_alarm);

			//SetTimer(TIMER_HEARTBEAR,5000,NULL);

			AfxMessageBox("��¼�ɹ�!");
			SYSTEMTIME stTime;

            GetLocalTime(&stTime);
			HW_NET_SET_SetSystime(g_server_id, &stTime);
			Connectvideo();

		}
		else
		{
			AfxMessageBox("��¼ʧ��!");
		}
	}
}


void CKaoqing::Connectvideo()
{
	
	if(test)
	{
		delete test;
		test = NULL;
	}
	

	if(g_server_id != INVALID_HANDLE)
	{
		test = new net_video_test(g_server_id,this->m_combochanctrl.GetCurSel());
		test->start_preview(GetDlgItem(IDC_VIDEO_KAOQING2)->GetSafeHwnd(),0);//m_cbo_connect_mode.GetCurSel());
		test->register_draw(draw_fun,(long)this);
		test->enable_audio_preview(1);
		::GetLocalTime(&stLocal);  
     //��ʾʱ��ļ����  
        wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
               stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
               stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
               stLocal.wMilliseconds,stLocal.wDayOfWeek);  
	    sprintf(filemovie,"c:\\movie\\%s+%d+%s.mp4",g_server_ip,this->m_combochanctrl.GetCurSel(),chBuf);
		test->save_to_file(filemovie);

		/*
		m_color_adjust_enable = FALSE;
		m_bright.SetPos(0);
		m_contrast.SetPos(0);
		m_saturation.SetPos(0);
		m_hue.SetPos(0);

		int yuv_buf;
		test->get_max_yuv_buf(yuv_buf);
		m_cbo_yuv_buf.SetCurSel(yuv_buf - 1);

		m_cbo_scale.SetCurSel(0);
		*/
		UpdateData(FALSE);
	}
	
}



static void CALLBACK draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser)
{
	CDC* dc = CDC::FromHandle(hDc);
	dc->SetTextColor(RGB(255,0,0));		
	dc->SetBkMode(TRANSPARENT);	

	Cnet_demoDlg* dlg = (Cnet_demoDlg*)nUser;

	
#if 0
#ifdef AUTO_ZOOM
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(test->window_handle());
	pWnd->GetClientRect(&rt);
	dc->DrawText("�ڴ����л�������������Ŵ�",&rt,DT_SINGLELINE|DT_LEFT|DT_NOCLIP);

	dc->SelectStockObject(NULL_BRUSH);
	CPen newPen(0,1,RGB(0,0,255));
	dc->SelectObject(&newPen);	
	rt = dlg->auto_zoom_test.get_zoom_rt();
	dc->Rectangle(&rt);
#else
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(test->window_handle());
	pWnd->GetClientRect(&rt);
	dc->DrawText("����������ͼ������,�Ҽ��ر����ţ����ֿ��ƷŴ���,˫������/ȫ���л�!",&rt,DT_SINGLELINE|DT_LEFT|DT_NOCLIP);
#endif
#endif

	//��ʾ����
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(test->window_handle());
	pWnd->GetClientRect(&rt);
	if(g_recved_len > 0)
	{
		CString str;
		str.Format("%uKbps / %uKbps",(unsigned long)g_recved_len / 1024 * 8,(unsigned long)g_total_len / 1024 * 8);
		dc->DrawText(str,&rt,DT_SINGLELINE|DT_BOTTOM|DT_NOCLIP);
	}

	if(g_rfid_data.len > 0
		&& (time(NULL) - g_rfid_data.tm) < 3)
	{		
		rt.top = rt.bottom - 20;
		CString str;
		for(int i = 0; i < g_rfid_data.len; i++)
		{
			CString tmp;
			tmp.Format("%02x",g_rfid_data.buf[i]);
			str = str + " " + tmp;
		}
		dc->DrawText(str.GetBuffer(0),&rt,DT_BOTTOM | DT_LEFT);	
		CKaoqing::GetRfidPic(str);  // ����rfidͼ��
	}


	CDC::DeleteTempMap();
}
static void CALLBACK hw_stream_callback(PLAY_HANDLE handle,
		INT64 stime,
		int type,
		char* buf,
		int len,
		long user)
	{	
		if(type == 8)
		{			
			//extra data	
			extra_data_head* extra_head = (extra_data_head*)buf;
			if(extra_head->type == 2)
			{
				//
				//extra_data* e = (extra_data*)(buf + sizeof(extra_data_head));
				//g_extra = *e;

				char* rfid_data = buf + sizeof(extra_data_head);
				int rfid_data_len = len - sizeof(extra_data_head);

				TRACE(">>>>>>>>>>>>>>s>%d\n",rfid_data_len);

				g_rfid_data.len = rfid_data_len;				
				g_rfid_data.tm = time(NULL);
				memcpy(g_rfid_data.buf,rfid_data,rfid_data_len);
			}
		}
	}
void CKaoqing::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	 HANDLEL = hwplay_open_local_file("c:\\movie\\bbb.mp4");

	
	 //hwplay_play(HANDLEL, GetDlgItem(IDC_STATIC1)->GetSafeHwnd());
	  hwplay_play(HANDLEL, NULL);

	 hwplay_register_stream_callback(HANDLEL,hw_stream_callback,(long)this);

	//hwplay_set_pos(HANDLEL ,2);
	 hwplay_open_sound(HANDLEL);

	hwplay_set_audio_volume(HANDLEL,0xffffffff);
	 return;

	int quality = 0;
	//test->save_to_bmp("jpg111.bmp");
	//OnButtonSelectiamge()  ;
	Show_picture("jpg111.bmp");
	return;


	

	

}

bool CKaoqing::Show_picture(CString imgPath)
{
	int height, width;
	CRect rect;//���������
	CRect rect1;
	CImage image; //����ͼƬ��
	image.Load(imgPath);
	imgPathlast = imgPath; //�������һ���ļ������ػ�
	height = image.GetHeight();
	width = image.GetWidth();
	
	m_PictureControl.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
	CDC *pDc = m_PictureControl.GetDC();//���pictrue�ؼ���Dc
	SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 

	if(width<=rect.Width() && height<=rect.Width()) //СͼƬ��������
	{
	rect1 = CRect(rect.TopLeft(), CSize(width,height));
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
	return TRUE;
	}
	else
	{
	float xScale=(float)rect.Width()/(float)width;
	float yScale=(float)rect.Height()/(float)height;
	float ScaleIndex=(xScale<=yScale?xScale:yScale);
	rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex,(int)height*ScaleIndex));
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
	}
	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	return TRUE;
}

 __int64  TimeDiff(SYSTEMTIME  left,SYSTEMTIME  right)  
{  
           CTime  tmLeft(left.wYear,left.wMonth,left.wDay,0,0,0);  
           CTime  tmRight(left.wYear,left.wMonth,left.wDay,0,0,0);  
           CTimeSpan  sp;  
           sp  =  tmLeft  -  tmRight;
           long  lLMinllis  =  (left.wHour*3600  +  left.wMinute*60  +  left.wSecond)*1000  +  left.wMilliseconds;  
           long  lRMinllis  =  (right.wHour*3600  +  right.wMinute*60  +  right.wSecond)*1000  +  right.wMilliseconds;  

           return  (__int64)sp.GetDays()*86400000  +  (lLMinllis  -  lRMinllis);  //�˴����غ��룬���ø����Լ��ĸ�ʽ��Ҫ����ת������ʱ����
}

void CKaoqing::GetRfidPic(CString str)
{
	static char tmpuid[20];
	
	char filebmp[255];
   // CString filebmp;
	//filebmp.Format(_T("%s.bmp"),str);one =
	sprintf(filebmp,"%s.bmp",str.GetBuffer(0));
	if (onlyone == 0){
       memcpy(tmpuid,filebmp+25,18);
	   onlyone = 1;
	   thisp->SetTimer(2,8000,NULL);
	}else{
	   if (memcmp(tmpuid,filebmp+25,18) == 0){
	      return;
	   }else{
	      memcpy(tmpuid,filebmp+25,18);
	   }
	
	
	}
	Beep( 750, 300 );
	
	TCHAR chBuf[256];
	SYSTEMTIME stLocal;  
    ::GetLocalTime(&stLocal);  
     //��ʾʱ��ļ����  
    wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
               stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
               stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
               stLocal.wMilliseconds,stLocal.wDayOfWeek);  
	sprintf(filebmp,"%s+%s.bmp",tmpuid,chBuf);
	test->save_to_bmp(filebmp);
	thisp->Show_picture(filebmp);


	 CString labelnum;
	 labelnum.Format("%s",tmpuid);
	 labelnum.Remove(' ');
	 char datetimestr[255];
	 sprintf(datetimestr,("%04u-%02u-%02u %02u:%02u:%02u"),  \
               stLocal.wYear, stLocal.wMonth, stLocal.wDay,   \
               stLocal.wHour, stLocal.wMinute, stLocal.wSecond) ;
               
	 int ret =1;
	 char * pErrMsg = 0;
	 char sSQL5[255]="insert into record(labelnum,date,pic) values('%s','%s','%s');";
	 sprintf(sSQL5,"insert into record(labelnum,date,pic) values('%s','%s','%s');",labelnum,datetimestr,filebmp);
	 ret = sqlite3_exec( thisp->db, sSQL5, 0, 0, &pErrMsg);
	 if ( ret != SQLITE_OK ){
		//AfxMessageBox(sqlite3_errmsg(this->myparent->db));
		printf(sqlite3_errmsg(thisp->db));
		return;
	 }
	 char sSQL6[255]="insert into vinfo(labelnum,starttime,endtime,filename,endflag) values('%s','%s','%s','%s',%d);";
	 for (int i=0;i<1024;i++){
		   if (labelinfo[i].sLabel != labelnum){
			   if (labelinfo[i].sLabel == ""){
				   labelinfo[i].sLabel = labelnum;
				   labelinfo[i].labelleft = 1;
				   labelinfo[i].timeLast = stLocal;
				   sprintf(sSQL6,"insert into vinfo(lablenum,starttime,filename,endflag) values('%s','%s','%s',%d);",labelnum,datetimestr,filemovie,0);
				   ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);				  
	               if ( ret != SQLITE_OK ){
		                //AfxMessageBox(sqlite3_errmsg(this->myparent->db));
		                printf(sqlite3_errmsg(thisp->db));
		                break;
	               }
				   labelinfo[i].id = sqlite3_last_insert_rowid( thisp->db);
				   break;;
			   }else{
				   continue;
			   }
		   }
		   if (TimeDiff(stLocal, labelinfo[i].timeLast)> 10*1000 ) {
			   labelinfo[i].labelleft = 1;
			   char datetimestr1[255];
			   sprintf(datetimestr1,("%04u-%02u-%02u %02u:%02u:%02u"),  \
                          labelinfo[i].timeLast.wYear, labelinfo[i].timeLast.wMonth, labelinfo[i].timeLast.wDay,   \
                          labelinfo[i].timeLast.wHour, labelinfo[i].timeLast.wMinute, labelinfo[i].timeLast.wSecond) ;
			   sprintf(sSQL6,"update vinfo set endtime = '%s',endflag= %d where id = %d ;",datetimestr1,1,labelinfo[i].id );
			   ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);
	           if ( ret != SQLITE_OK ){
		            //AfxMessageBox(sqlite3_errmsg(this->myparent->db));
		            printf(sqlite3_errmsg(thisp->db));
		            break;
	           }
			   labelinfo[i].timeLast = stLocal;
			   sprintf(sSQL6,"insert into vinfo(lablenum,starttime,filename,endflag) values('%s','%s','%s',%d);",labelnum,datetimestr,filemovie,0);
			   ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);
	           if ( ret != SQLITE_OK ){
		            //AfxMessageBox(sqlite3_errmsg(this->myparent->db));
		            printf(sqlite3_errmsg(thisp->db));
		            break;
	           }
			   labelinfo[i].id = sqlite3_last_insert_rowid( thisp->db);
			   break;

		   }else{
		  
			   labelinfo[i].labelleft++;
			   labelinfo[i].timeLast = stLocal;
			   break;
		  
		   }


	 }
	
	

}

void CKaoqing::OnButtonSelectiamge()  
{ 
    char  szFilter[] = "BMP Files (*.bmp)|*.bmp|All Files(*.*)|*.*||"; 
    CFileDialog dlg( TRUE,"BMP",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter ); 
    if(dlg.DoModal() == IDOK) 
    { 
        strPathName = dlg.GetPathName();         
        LoadShowBMPFile(strPathName);     
        m_hBitmap = BufferToHBITMAP();   
		RECT rt;
		CWnd* pWnd = CWnd::FromHandle(GetDlgItem(IDC_STATIC1)->GetSafeHwnd());
		pWnd->GetWindowRect(&rt);

		//Invalidate(); 
		CDC* pDC = CDC::FromHandle(::GetDC(GetDlgItem(IDC_STATIC1)->GetSafeHwnd()));
		DrawImage(rt.left, rt.top, pDC) ;
       // Invalidate(); 
    } 
} 
 
bool CKaoqing::LoadShowBMPFile(const char *pPathname) 
{ 
    CFile file; 
    if( !file.Open( pPathname, CFile::modeRead) ) 
        return FALSE; 
    DWORD  m_nFileLen; 
    m_nFileLen = file.GetLength(); 
    m_pBMPBuffer = new char[m_nFileLen + 1]; 
    if(!m_pBMPBuffer) 
        return FALSE; 
    if(file.Read(m_pBMPBuffer,m_nFileLen) != m_nFileLen) 
        return FALSE; 
    return TRUE; 
} 
 
HBITMAP CKaoqing::BufferToHBITMAP() 
{ 
    HBITMAP                hShowBMP; 
    LPSTR                hDIB,lpBuffer = m_pBMPBuffer; 
    LPVOID                lpDIBBits; 
    BITMAPFILEHEADER    bmfHeader; 
    DWORD                bmfHeaderLen; 
     
    bmfHeaderLen = sizeof(bmfHeader); 
    strncpy((LPSTR)&bmfHeader,(LPSTR)lpBuffer,bmfHeaderLen); 
    if (bmfHeader.bfType != (*(WORD*)"BM")) return NULL; 
    hDIB = lpBuffer + bmfHeaderLen; 
    BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ; 
    BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ; 
    m_bmW = bmiHeader.biWidth; 
    m_bmH = bmiHeader.biHeight; 
    lpDIBBits=(lpBuffer)+((BITMAPFILEHEADER *)lpBuffer)->bfOffBits; 
    CClientDC dc(this); 
    hShowBMP = CreateDIBitmap(dc.m_hDC,&bmiHeader,CBM_INIT,lpDIBBits,
                               &bmInfo,DIB_RGB_COLORS); 
    return hShowBMP; 
} 
 
void CKaoqing::DrawImage(int x, int y, CDC *pDC) 
{ 
    HBITMAP OldBitmap; 
    CDC MemDC; 
    MemDC.CreateCompatibleDC(pDC); 
    OldBitmap=(HBITMAP)MemDC.SelectObject(m_hBitmap); 
    CBitmap* pBitmap = CBitmap::FromHandle(m_hBitmap); 
    if(m_bmW == 0 || m_bmH == 0) 
        return ; 
    //CRect rect; 
   // GetClientRect(&rect);     
	CRect rect;
	CWnd* pWnd = CWnd::FromHandle(GetDlgItem(IDC_STATIC1)->GetSafeHwnd());
	pWnd->GetWindowRect(&rect);
    pDC->BitBlt(x,y,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY); 
    MemDC.SelectObject(OldBitmap); 
} 



 void CKaoqing::OnPaint()
 {
	 CPaintDC dc(this); // device context for painting
	 // TODO: �ڴ˴������Ϣ����������
	 if (imgPathlast != ""){
	   Show_picture( imgPathlast);
	 }
	 // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
 }





 void CKaoqing::OnBnClickedOk()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 if(test){
	     test->stop_preview();
		 delete test;
		
	 }
	 CDialogEx::OnOK();

	 
 }


 void CKaoqing::OnTimer(UINT_PTR nIDEvent)
 {
	 SYSTEMTIME stLocal;  
     ::GetLocalTime(&stLocal);  
	 if (nIDEvent == 3){
		 for (int i=0;i<1024;i++){
			 if (labelinfo[i].sLabel == ""){
				 break;
			 }
			 if (labelinfo[i].labelleft == 0){
				 continue;
			 }

			if ( TimeDiff(stLocal,labelinfo[i].timeLast) > 10*1000){
				char datetimestr1[255];
				char sSQL6[255];
				int ret =1;
				char * pErrMsg = 0;
				sprintf(datetimestr1,("%04u-%02u-%02u %02u:%02u:%02u"),  \
							  labelinfo[i].timeLast.wYear, labelinfo[i].timeLast.wMonth, labelinfo[i].timeLast.wDay,   \
							  labelinfo[i].timeLast.wHour, labelinfo[i].timeLast.wMinute, labelinfo[i].timeLast.wSecond) ;
				sprintf(sSQL6,"update vinfo set endtime = '%s',endflag= %d where id = %d ;",datetimestr1,1,labelinfo[i].id );
				ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);
				if ( ret != SQLITE_OK ){
					  //AfxMessageBox(sqlite3_errmsg(this->myparent->db));
					  printf(sqlite3_errmsg(thisp->db));
					  break;
				}
				labelinfo[i].labelleft = 0;
			}
		 }
	 }
	 if (nIDEvent == 2){
	   onlyone = 0;
	   KillTimer(2);
	 }
	 CDialogEx::OnTimer(nIDEvent);
 }


 void CKaoqing::OnBnClickedButton2()
 {
	  const char * sSQL3 = "select * from user;";
	  UpdateData(TRUE);
	  CString username, password;
	  m_username.GetWindowText(username );
	  m_password.GetWindowText( password);

	   char * pErrMsg = 0;
	   sqlite3_exec( db, sSQL3, _sql_callback, 0, &pErrMsg);

	   CDlgRenyuan dlg;
	   if (password == password_db && username == username_db){
		   dlg.DoModal();
		   return;
	   }
	   AfxMessageBox("���벻��");
 }
 static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName)
{
  int i;
  for ( i=0; i < argc; i++ )
  {
      printf( "%s = %s\n", szColName[i], argv[i] == 0 ? "NUL" : argv[i] );
  }

  username_db = CString(argv[1]);
  password_db = CString(argv[2]);
  return 0;
 }

 void CKaoqing::OnClose()
 {
	  if(test){
	     test->stop_preview();
		 delete test;
		
	 }

	 CDialogEx::OnClose();
 }


 void CKaoqing::OnBnClickedButton3()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	CIphostDlg dlg;
	dlg.DoModal();
	 
 }


 void CKaoqing::OnCbnSelchangeCombo1()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������

	 if(test)
	{
		delete test;
		test = NULL;
	}
	
	if(g_server_id != INVALID_HANDLE)
	{
		test = new net_video_test(g_server_id,this->m_combochanctrl.GetCurSel());
		test->start_preview(GetDlgItem(IDC_VIDEO_KAOQING2)->GetSafeHwnd(),0);
		test->register_draw(draw_fun,(long)this);
		test->enable_audio_preview(1);
		::GetLocalTime(&stLocal);  
     //��ʾʱ��ļ����  
        wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
               stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
               stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
               stLocal.wMilliseconds,stLocal.wDayOfWeek);  
	    sprintf(filemovie,"c:\\movie\\%s+%d+%s.mp4",g_server_ip,this->m_combochanctrl.GetCurSel(),chBuf);
		test->save_to_file(filemovie);
		/*
		m_color_adjust_enable = FALSE;
		m_bright.SetPos(0);
		m_contrast.SetPos(0);
		m_saturation.SetPos(0);
		m_hue.SetPos(0);

		int yuv_buf;
		test->get_max_yuv_buf(yuv_buf);
		m_cbo_yuv_buf.SetCurSel(yuv_buf - 1);

		m_cbo_scale.SetCurSel(0);

		UpdateData(FALSE);
		*/
		UpdateData(FALSE);
	}
 }
