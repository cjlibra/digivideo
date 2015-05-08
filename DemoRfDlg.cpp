// DemoRfDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "DemoRfDlg.h"
#include "afxdialogex.h"


#include "net_video_test.h"

#include "HikReader.h"
#include "rfid_structs_define.h"
#include "RFIDReader_Packets.h"


#pragma comment(lib,"RFIDReaderNet.lib")

static USER_HANDLE g_server_id = INVALID_HANDLE;
static   int g_window_count = 0;
static  long g_server_version = 0;
static net_video_test* test = NULL;

static char DevID[32];

static CDemoRfDlg *thisp;

typedef struct {
	CString epc;
	SYSTEMTIME lasttime;
	int empty ;
}lable_t;


static lable_t lstatus[1024];


int getSecondsCha(SYSTEMTIME last,SYSTEMTIME now)
{
	COleDateTime dTimeF(last); 
    COleDateTime dTimeS(now); 
    COleDateTimeSpan dTimeSpan = dTimeS - dTimeF; 
    int nSecnonSpan = 0;
    nSecnonSpan =dTimeSpan.GetTotalSeconds();
    return nSecnonSpan ;

}
void makeCopyTime(SYSTEMTIME &two ,SYSTEMTIME &one)
{

   two.wYear  =  one.wYear;
   two.wMonth  = one.wMonth;
   two.wDayOfWeek = one.wDayOfWeek;
   two.wDay = one.wDay;
   two.wHour = one.wHour;
   two.wMinute = one.wMinute;
   two.wSecond = one.wSecond;
   two.wMilliseconds =one.wMilliseconds;


}
bool isNeedShot(CString epc)
{
	
	SYSTEMTIME stLocal;
	
	for(int i=0;i<1024;i++){

		if (lstatus[i].epc == epc ) {
			::GetLocalTime(&stLocal); 
			int secondscha = getSecondsCha(lstatus[i].lasttime,stLocal);
			if (secondscha > 20){
				makeCopyTime(lstatus[i].lasttime , stLocal);
				TRACE("%s---->20 %d\n",epc ,stLocal.wMilliseconds);
				return true;
			}else{
				makeCopyTime(lstatus[i].lasttime , stLocal);
				TRACE("%s----<20 %d\n",epc,stLocal.wMilliseconds);
				return false;

			}
			 
		}

	}

	for(int i=0;i<1024;i++){
		if (lstatus[i].empty == 0){
			lstatus[i].empty = 1;
			lstatus[i].epc = epc;
			::GetLocalTime(&stLocal); 
			makeCopyTime(lstatus[i].lasttime , stLocal);
			TRACE("%s----empty=0  %d\n",epc,stLocal.wMilliseconds);
			return true;




		}

	}

	return false;

}

INT32S GetInventoryCallback(RFID_RADIO_HANDLE	handle, void *context)
{
	
	INVENTORY_REPORT_RESULT *data = (INVENTORY_REPORT_RESULT *)context;
	SYSTEMTIME stLocal;
	::GetLocalTime(&stLocal); 
	

	
	
	//MySingleTagInfo report;
	//memcpy(report.cid, data->EPC, 1024);
	 
	 
	/*
	report.nAntenna = atoi(data->AntennaID);
	report.nCardType = 2;
	report.ncidLen = strlen(data->EPC);
	strcpy(report.szRSSI, data->RSSI);
	strcpy( report.TimeStamp, data->TimeStamp);
	strcpy(report.DeviceID, data->DeviceID);

	strcpy(report.TID, data->TID);
	strcpy(report.Frequency, data->Frequency);

	CUHFDemoNewDlg* dlg =(CUHFDemoNewDlg *)theApp.m_pMainWnd;
	CDlgInventory *dlgInv = dlg->getDlgInv();

	*/
	CWnd* pWnd = thisp->GetDlgItem(IDC_PIC_DEMORF);
	 
	CDC* dc = pWnd->GetDC();;
	dc->SetTextColor(RGB(255,0,0));		
	dc->SetBkMode(TRANSPARENT);	

	char epcstr[100]={0};
	CString tmp;
	tmp.Format("%s", data->EPC);
	sprintf(epcstr,"%s",data->EPC);

	if (isNeedShot(tmp) == false) return 0;
	
	char chBuf[255]={0};
	char filebmp[1024]={0};
	 
			   
    wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
            stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
            stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
            stLocal.wMilliseconds,stLocal.wDayOfWeek);  
	sprintf(filebmp,"%s+%s.bmp",epcstr,chBuf);
	bool result;
	if (test!=NULL){
		result = test->save_to_bmp(filebmp);
	}
	if (result == true){
		thisp->Show_picture(filebmp);
	}
    RECT rt;
	     
	pWnd->GetClientRect(&rt);
	rt.top = rt.bottom - 20;
	dc->DrawText(epcstr ,&rt,DT_BOTTOM | DT_LEFT);	
	thisp->ReleaseDC(dc);

	 

	
	
	
	return 0;
}




// CDemoRfDlg 对话框

IMPLEMENT_DYNAMIC(CDemoRfDlg, CDialogEx)

CDemoRfDlg::CDemoRfDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemoRfDlg::IDD, pParent)
{
	thisp = this;
	for (int i=0 ;i<1024;i++){
        lstatus[i].epc="";
		::GetLocalTime(&lstatus[i].lasttime);
		lstatus[i].empty = 0;


	}
}

CDemoRfDlg::~CDemoRfDlg()
{
	if (test != NULL) {
		// test->stop_preview();
		 delete test;
		 test=NULL;

	}
	BOOL val;
	if (g_server_id != INVALID_HANDLE){
	    val = HW_NET_Logout (g_server_id ) ;
		val = 0;
		val = HW_NET_Release();
	}
}

void CDemoRfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_DEMORF, m_ip_str);
	DDX_Control(pDX, IDC_PIC_DEMORF, m_picctrl);
	DDX_Control(pDX, IDC_IPADDRESS_RFIP, m_IP_RFREADER);
}


BEGIN_MESSAGE_MAP(CDemoRfDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTLOGIN_DEMORF, &CDemoRfDlg::OnBnClickedBtloginDemorf)
	ON_BN_CLICKED(IDC_BUTTON1_DEMORF, &CDemoRfDlg::OnBnClickedButton1Demorf)
	ON_BN_CLICKED(IDC_BTLOGIN_RF, &CDemoRfDlg::OnBnClickedBtloginRf)
	ON_BN_CLICKED(IDC_BUT_UNCON, &CDemoRfDlg::OnBnClickedButUncon)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDemoRfDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CDemoRfDlg 消息处理程序


void CDemoRfDlg::OnBnClickedBtloginDemorf()
{
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
		m_ip_str.GetAddress(f0,f1,f2,f3);
		sprintf(ip,"%d.%d.%d.%d",f0,f1,f2,f3);
//		sprintf(g_server_ip,"%d.%d.%d.%d",f0,f1,f2,f3);
		g_server_id = HW_NET_Login(ip,5198,"admin","12345");
		if(g_server_id != INVALID_HANDLE)
		{
		//	m_slots.ResetContent();	
			g_window_count = HW_NET_GetWindowCount(g_server_id);			
			char str[255];
			
			for(int i = 0; i < g_window_count; i++)
			{
				sprintf(str,"通道%d",i + 1);
			 	//this->m_combochanctrl.AddString(str);
			}
		 	//this->m_combochanctrl.SetCurSel(0);
			
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

			AfxMessageBox("登录成功!");
			SYSTEMTIME stTime;

            GetLocalTime(&stTime);
			HW_NET_SET_SetSystime(g_server_id, &stTime);
		 	Connectvideo();

		}
		else
		{
			AfxMessageBox("登录失败!");
		}
	}
}




void  CDemoRfDlg::Connectvideo()
{
	SYSTEMTIME stLocal; 
	TCHAR chBuf[256];
	if(test)
	{
		delete test;
		test = NULL;
	}
	

	if(g_server_id != INVALID_HANDLE)
	{
		test = new net_video_test(g_server_id,0);
		test->start_preview(GetDlgItem(IDC_VIDEO_DEMORF)->GetSafeHwnd(),0);//m_cbo_connect_mode.GetCurSel());
		//test->register_draw(draw_fun,(long)this);
		//test->enable_audio_preview(1);
		::GetLocalTime(&stLocal);  
     //显示时间的间隔。  
        wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
               stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
               stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
               stLocal.wMilliseconds,stLocal.wDayOfWeek);  
	    //sprintf(filemovie,"movie\\%s+%d+%s.mp4",g_server_ip,0,chBuf);
		//test->save_to_file(filemovie);

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

BOOL CDemoRfDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	HW_NET_Init(5198);	
	HW_NET_SetUdpBasePort(8000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDemoRfDlg::OnBnClickedButton1Demorf()
{
	test->save_to_bmp("1.bmp");
	Sleep(1000*3);

	Show_picture("1.bmp");

}



bool CDemoRfDlg::Show_picture(CString imgPath)
{
	int height, width;
	CRect rect;//定义矩形类
	CRect rect1;
	CImage image; //创建图片类
	image.Load(imgPath);
//	imgPathlast = imgPath; //保存最后一张文件名给重画
	height = image.GetHeight();
	width = image.GetWidth();
	
	m_picctrl.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	CDC *pDc = m_picctrl.GetDC();//获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 

	if(width<=rect.Width() && height<=rect.Width()) //小图片，不缩放
	{
	rect1 = CRect(rect.TopLeft(), CSize(width,height));
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY); //将图片画到Picture控件表示的矩形区域
	return TRUE;
	}
	else
	{
	float xScale=(float)rect.Width()/(float)width;
	float yScale=(float)rect.Height()/(float)height;
	float ScaleIndex=(xScale<=yScale?xScale:yScale);
	rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex,(int)height*ScaleIndex));
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY); //将图片画到Picture控件表示的矩形区域
	}
	ReleaseDC(pDc);//释放picture控件的Dc
	return TRUE;
}

void CDemoRfDlg::OnBnClickedBtloginRf()
{
	UpdateData(true);
	char ip[255];
	BYTE f0,f1,f2,f3;
	m_IP_RFREADER.GetAddress(f0,f1,f2,f3);
	sprintf(ip,"%d.%d.%d.%d",f0,f1,f2,f3);
	
	int re = Connect(ip,DevID , 7880);
	if (re == 0){
		//AfxMessageBox("suscess");

	}else{

		AfxMessageBox("fail");
		return;

	}

	re = -1;
	re = ISO_6C_StartPeriodInventory(DevID, GetInventoryCallback,TRUE);

	if (re == 0){
	//	AfxMessageBox("ISO suscess");

	}else{

		AfxMessageBox("ISO fail");
		return;

	}
		int re1 = ISO_6C_AutoReportTag( DevID, GetInventoryCallback, TRUE);
	if (re1 == 0){
	//	AfxMessageBox("auto suscess");

	}else{

		AfxMessageBox("auto fail");
		return;

	}


}


void CDemoRfDlg::OnBnClickedButUncon()
{
	// TODO: 在此添加控件通知处理程序代码
	int re1 = ISO_6C_StopPeriodInventory (DevID);
	if (re1 == 0){
		AfxMessageBox("StopPeriodInventory suscess");

	}else{

		AfxMessageBox("StopPeriodInventory fail");
		 

	}

	int re = Disconnect( DevID);
	if (re == 0){
		AfxMessageBox("unconnect suscess");

	}else{

		AfxMessageBox("unconnect fail");
		 

	}


	
}


void CDemoRfDlg::OnBnClickedButtonStop()
{
	if (test != NULL) {
		// test->stop_preview();
		 delete test;
		 test=NULL;

	}
	BOOL val;
	if (g_server_id != INVALID_HANDLE){
	    val = HW_NET_Logout (g_server_id ) ;
		val = 0;
		val = HW_NET_Release();
		g_server_id = INVALID_HANDLE;
	}
}
