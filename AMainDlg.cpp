// AMainDlg.cpp : 实现文件
//

 

#include "stdafx.h"
#include "net_demo.h"
#include "AMainDlg.h"
#include "afxdialogex.h"

#include "IphostDlg.h"
#include "DlgVcatch.h"
#include "RecordDlg.h"

#include "net_demoDlg.h"
#include "DlgRenyuan.h"
#include "ExcelOutDlg.h"
#include "WebWeb.h"




CAMainDlg *thisp;
static   int g_window_count = 0;
static  long g_server_version = 0;
static net_video_test* test[1024] = {NULL};
static char filemovie[256];
static UINT64 g_recved_len = 0;
static UINT64 g_total_len = 0;

typedef struct _VIEWITEMINFO{
	int idle;
	CString ip;
	net_video_test* ptest;
	CWnd * pwin;

}VIEWITEMINFO;
static VIEWITEMINFO viewiteminfo[16];

typedef struct _LABELINFO{
	CString sLabel;
	CString ip;
    SYSTEMTIME timeLast;
	int  labelleft;
	int id;
	

}LABELINFO;
static LABELINFO labelinfo[1024];

static __int64	TimeDiff(SYSTEMTIME  left,SYSTEMTIME  right)  ;
static int onlyone = 0;
// CAMainDlg 对话框

IMPLEMENT_DYNAMIC(CAMainDlg, CDialogEx)

CAMainDlg::CAMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAMainDlg::IDD, pParent)
{
	thisp = this;
	startvideocount = 0;
	setcustom = -1;
}

CAMainDlg::~CAMainDlg()
{
	sqlite3_close(db);
	for (int i=0;i<startvideocount;i++){
		if(   test[i]->is_preview())
		{
			test[i]->stop_preview();
			 
		}
		if(test[i] )
		{
			 
			delete test[i];
			test[i] = NULL;
		}
	}
	 
}

void CAMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_mainiptree);
}


BEGIN_MESSAGE_MAP(CAMainDlg, CDialogEx)
	ON_COMMAND(ID_32773, &CAMainDlg::On32773)
	ON_COMMAND(ID_Menu, &CAMainDlg::OnMenu)
	ON_COMMAND(ID_32775, &CAMainDlg::On32775)
	ON_COMMAND(ID_32778, &CAMainDlg::On32778)
	ON_COMMAND(ID_32780, &CAMainDlg::On32780)
	ON_COMMAND(ID_32777, &CAMainDlg::On32777)
	ON_COMMAND(ID_32771, &CAMainDlg::On32771)
	ON_COMMAND(ID_32772, &CAMainDlg::On32772)
	ON_COMMAND(ID_32781, &CAMainDlg::On32781)
	ON_COMMAND(ID_32783, &CAMainDlg::On32783)
	ON_COMMAND(ID_32784, &CAMainDlg::On32784)
	ON_COMMAND(ID_32785, &CAMainDlg::On32785)
	ON_COMMAND(ID_32786, &CAMainDlg::On32786)
	ON_WM_TIMER()
	ON_COMMAND(ID_32787, &CAMainDlg::On32787)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CAMainDlg::OnNMRClickTree1)
	ON_COMMAND(ID_32792, &CAMainDlg::On32792)
	ON_COMMAND(ID_32793, &CAMainDlg::On32793)
	ON_COMMAND(ID_32794, &CAMainDlg::On32794)
	ON_COMMAND(ID_32795, &CAMainDlg::On32795)
	ON_COMMAND(ID_32788, &CAMainDlg::On32788)
	ON_COMMAND(ID_32789, &CAMainDlg::On32789)
	ON_COMMAND(ID_32790, &CAMainDlg::On32790)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_32988, &CAMainDlg::On32988)
	ON_COMMAND(ID_32989, &CAMainDlg::On32989)
	ON_COMMAND(ID_32990, &CAMainDlg::On32990)
	ON_COMMAND(ID_327192, &CAMainDlg::On327192)
	ON_COMMAND(ID_327193, &CAMainDlg::On327193)
	ON_COMMAND(ID_327194, &CAMainDlg::On327194)
	ON_COMMAND(ID_327195, &CAMainDlg::On327195)
	ON_COMMAND(ID_32805, &CAMainDlg::On32805)
END_MESSAGE_MAP()


// CAMainDlg 消息处理程序
static int _sql_select_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	CString tmp = argv[1];
	thisp->m_mainiptree.InsertItem(argv[1],NULL,NULL,thisp->hItem );
//	jj++;
    return 0;
}

void CAMainDlg::RefreshTree()
{

	 this->m_mainiptree.DeleteAllItems();
	 hItem = this->m_mainiptree.InsertItem("摄像机组",NULL,NULL,NULL);

	 const char * sSQL4 = "select * from hosts";
	 
	 char *pErrMsg = 0;
	 sqlite3_exec( thisp->db, sSQL4,_sql_select_callback, 0, &pErrMsg);

	 this->m_mainiptree.Expand(hItem,TVE_EXPAND);


}
void CAMainDlg::SwitchView(int percount)
{
	  CWnd * m_pviewItem1 = GetDlgItem(IDC_STATIC1);	  
	  CWnd * m_pviewItem2 = GetDlgItem(IDC_STATIC2);
	  CWnd * m_pviewItem3 = GetDlgItem(IDC_STATIC3);
	  CWnd * m_pviewItem4 = GetDlgItem(IDC_STATIC4);
	  CWnd * m_pviewItem5 = GetDlgItem(IDC_STATIC5);
	  CWnd * m_pviewItem6 = GetDlgItem(IDC_STATIC6);
	  CWnd * m_pviewItem7 = GetDlgItem(IDC_STATIC7);
	  CWnd * m_pviewItem8 = GetDlgItem(IDC_STATIC8);
	  CWnd * m_pviewItem9 = GetDlgItem(IDC_STATIC9);
	  CWnd * m_pviewItem10 = GetDlgItem(IDC_STATIC10);
	  CWnd * m_pviewItem11 = GetDlgItem(IDC_STATIC11);
	  CWnd * m_pviewItem12 = GetDlgItem(IDC_STATIC12);
	  CWnd * m_pviewItem13 = GetDlgItem(IDC_STATIC13);
	  CWnd * m_pviewItem14 = GetDlgItem(IDC_STATIC14);
	  CWnd * m_pviewItem15 = GetDlgItem(IDC_STATIC15);
	  CWnd * m_pviewItem16 = GetDlgItem(IDC_STATIC16);
	  m_pviewItem1->ShowWindow(0);
	  m_pviewItem2->ShowWindow(0);
	  m_pviewItem3->ShowWindow(0);
	  m_pviewItem4->ShowWindow(0);
	  m_pviewItem5->ShowWindow(0);
	  m_pviewItem6->ShowWindow(0);
	  m_pviewItem7->ShowWindow(0);
	  m_pviewItem8->ShowWindow(0);
	  m_pviewItem9->ShowWindow(0);
	  m_pviewItem10->ShowWindow(0);
	  m_pviewItem11->ShowWindow(0);
	  m_pviewItem12->ShowWindow(0);
	  m_pviewItem13->ShowWindow(0);
	  m_pviewItem14->ShowWindow(0);
	  m_pviewItem15->ShowWindow(0);
	  m_pviewItem16->ShowWindow(0);

	  RECT clRect;
	  RECT treeRect;
	  int viewitemheight;
	  int  viewitemwidth;
	  RECT viewRect;
	  switch ( percount) {
		case 1 :
		 
			GetClientRect(&clRect);
		
			treeRect.top = clRect.top+20;
			treeRect.left = clRect.left+20;
			treeRect.right = (clRect.right-clRect.left)/4+treeRect.left-100;
			treeRect.bottom = clRect.bottom-20;
			m_mainiptree.MoveWindow(&treeRect);		 
		
			viewitemheight = (clRect.bottom-clRect.top-20-20-5-5)/1;
			viewitemwidth = (clRect.right-treeRect.right-20-20-5-5)/1;

			viewRect.top = treeRect.top;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem1->MoveWindow(&viewRect);

			m_pviewItem1->ShowWindow(1);
		
			break;
		case 2 :
			GetClientRect(&clRect);
		 
			treeRect.top = clRect.top+20;
			treeRect.left = clRect.left+20;
			treeRect.right = (clRect.right-clRect.left)/4+treeRect.left-100;
			treeRect.bottom = clRect.bottom-20;
			m_mainiptree.MoveWindow(&treeRect);

			viewitemheight = (clRect.bottom-clRect.top-20-20-5)/2;
			viewitemwidth = (clRect.right-treeRect.right-20-20-5)/2;

			viewRect.top = treeRect.top;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem1->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem2->MoveWindow(&viewRect);
		
			viewRect.top = treeRect.top+viewitemheight+5;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem3->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem4->MoveWindow(&viewRect);

			m_pviewItem1->ShowWindow(1);
			m_pviewItem2->ShowWindow(1);
			m_pviewItem3->ShowWindow(1);
			m_pviewItem4->ShowWindow(1);
		
			break;
		case 3 :
		
			GetClientRect(&clRect);
		 
			treeRect.top = clRect.top+20;
			treeRect.left = clRect.left+20;
			treeRect.right = (clRect.right-clRect.left)/4+treeRect.left-100;
			treeRect.bottom = clRect.bottom-20;
			m_mainiptree.MoveWindow(&treeRect);

			viewitemheight = (clRect.bottom-clRect.top-20-20-5-5)/3;
			viewitemwidth = (clRect.right-treeRect.right-20-20-5-5)/3;

			viewRect.top = treeRect.top;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem1->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem2->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem3->MoveWindow(&viewRect);
			viewRect.top = treeRect.top+viewitemheight+5;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem4->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem5->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem6->MoveWindow(&viewRect);
			viewRect.top = treeRect.top+(viewitemheight+5)*2;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem7->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem8->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem9->MoveWindow(&viewRect);

			m_pviewItem1->ShowWindow(1);
			m_pviewItem2->ShowWindow(1);
			m_pviewItem3->ShowWindow(1);
			m_pviewItem4->ShowWindow(1);
			m_pviewItem5->ShowWindow(1);
			m_pviewItem6->ShowWindow(1);
			m_pviewItem7->ShowWindow(1);
			m_pviewItem8->ShowWindow(1);
			m_pviewItem9->ShowWindow(1);
			break;

        case 4 :
		
			GetClientRect(&clRect);
		 
			treeRect.top = clRect.top+20;
			treeRect.left = clRect.left+20;
			treeRect.right = (clRect.right-clRect.left)/4+treeRect.left-100;
			treeRect.bottom = clRect.bottom-20;
			m_mainiptree.MoveWindow(&treeRect);

			viewitemheight = (clRect.bottom-clRect.top-20-20-5-5-5)/4;
			viewitemwidth = (clRect.right-treeRect.right-20-20-5-5-5)/4;

			viewRect.top = treeRect.top;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem1->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem2->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem3->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem4->MoveWindow(&viewRect);

			viewRect.top = treeRect.top+viewitemheight+5;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem5->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem6->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem7->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem8->MoveWindow(&viewRect);

			viewRect.top = treeRect.top+(viewitemheight+5)*2;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem9->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem10->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem11->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem12->MoveWindow(&viewRect);

			viewRect.top = treeRect.top+(viewitemheight+5)*3;
			viewRect.left = treeRect.right+20;
			viewRect.bottom = viewRect.top + viewitemheight;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem13->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem14->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem15->MoveWindow(&viewRect);
			viewRect.left = viewRect.right+5;
			viewRect.right = viewRect.left + viewitemwidth;
			m_pviewItem16->MoveWindow(&viewRect);

			m_pviewItem1->ShowWindow(1);
			m_pviewItem2->ShowWindow(1);
			m_pviewItem3->ShowWindow(1);
			m_pviewItem4->ShowWindow(1);
			m_pviewItem5->ShowWindow(1);
			m_pviewItem6->ShowWindow(1);
			m_pviewItem7->ShowWindow(1);
			m_pviewItem8->ShowWindow(1);
			m_pviewItem9->ShowWindow(1);
			m_pviewItem10->ShowWindow(1);
			m_pviewItem11->ShowWindow(1);
			m_pviewItem12->ShowWindow(1);
			m_pviewItem13->ShowWindow(1);
			m_pviewItem14->ShowWindow(1);
			m_pviewItem15->ShowWindow(1);
			m_pviewItem16->ShowWindow(1);
			break;
		}


}
void CAMainDlg::OutInitFile()
{


	CreateDirectory("movie",NULL);
	SetTimer(2,1000*60*30,NULL);
	return;
	/*

	CString filename[6];
	int IDRid[6];
	CString sufix[6];

	 filename[0] = "D3DX9_41.dll";
		IDRid[0] = IDR_IDR_11;
		 sufix[0]="dll";
	 filename[1] = "HikPlayM4.dll";
		 IDRid[1] = IDR_IDR_21;
		 sufix[1]="dll";
	 filename[2] = "hwclient.dll";
		 IDRid[2] = IDR_IDR_31;
		 sufix[2]="dll";
	 filename[3] = "hwsadp.dll";
		 IDRid[3] = IDR_IDR_41;
		 sufix[3]="dll";
	 filename[4] = "medb.s3db";
		 IDRid[4] = IDR_IDR_51;
		 sufix[4]="s3db";
	 filename[5] = "play_sdk.dll";
		 IDRid[5]= IDR_IDR_61;
		 sufix[5]="dll";

	int fileexit = 1;
	FILE*  fp;
	for (int i=0;i<6;i++){
	    if ((fp= fopen(filename[i],"rb"))==NULL){
			fileexit = 0;
			break;

		}
	    fclose(fp);
	}
	
	if (fileexit  == 1) return;

	for (int i=0;i<6;i++){
		 
		HRSRC hResource = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDRid[i]), TEXT(sufix[i]));
		if(hResource)
		{
			 //加载资源
			 HGLOBAL hg = ::LoadResource(GetModuleHandle(NULL), hResource);  
			 if(hg)  
			 {
				 //锁定资源
				 LPVOID pData = ::LockResource(hg);
				 if(pData)
				 {
					  //获取资源大小
					  DWORD dwSize = SizeofResource(GetModuleHandle(NULL), hResource);
					  CFile rFile;
					  if(rFile.Open(filename[i], CFile::modeCreate | CFile::modeReadWrite))
					  {
						   rFile.Write(pData, dwSize);
						   rFile.Close();
					  }
				 }
			 }
		}
	}
	*/
}
BOOL CAMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(3,1000*10,NULL);
	OutInitFile();

    CWinApp	 *pApp=AfxGetApp();
    HICON hIcon=pApp->LoadIcon(IDR_MAINFRAME);
    SetIcon(hIcon,TRUE);//16 

	int ret = 1;
	thisp->db = 0;
	ret = sqlite3_open("medb.s3db", &thisp->db);
      
    if ( ret != SQLITE_OK )
    {
      fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(thisp->db));
      return(1);
    }
      
    printf("数据库连接成功!\n");

	ShowWindow(SW_MAXIMIZE);
	SwitchView(3);

	RefreshTree(); //显示IPtree
	InitViewItemInfo(); //显示框属性初始化

	HW_NET_Init(5198);	
	//HW_NET_SetUdpBasePort(8000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAMainDlg::On32773() //设备配置，添加摄像机ip属性
{
	CIphostDlg dlg;
	dlg.DoModal();

}


void CAMainDlg::OnMenu() //视频搜索
{
       CDlgVcatch  dlg;
	   dlg.DoModal();
}


void CAMainDlg::On32775() //图像搜索
{
	
	CRecordDlg  dlg;
	dlg.DoModal();
	  
}


void CAMainDlg::On32778() //启动ip摄像机
{
	
	HTREEITEM  hItem = m_mainiptree.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("请选择ip");
		return;
	}
	CString tmp = m_mainiptree.GetItemText(hItem);
	if (tmp == "摄像机组") {

		AfxMessageBox("请选择ip");
		return;
	}
	int whichviewitem = SearchViewItemIdle();
	if (whichviewitem == -1){
		AfxMessageBox("播放显示窗都满了，请停止几个后再启动");
		return;
	}
	 
	USER_HANDLE g_server_id = INVALID_HANDLE; 
	//if(g_server_id != INVALID_HANDLE)
	//{
	//	//KillTimer(TIMER_HEARTBEAR);
	//	HW_NET_Logout(g_server_id);
	//	g_server_id = INVALID_HANDLE;
	//}
	char ip[255];
	if(1||g_server_id == INVALID_HANDLE)
	{
		
		sprintf(ip,"%s",tmp);
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

			//AfxMessageBox("登录成功!");
			SYSTEMTIME stTime;

            GetLocalTime(&stTime);
			HW_NET_SET_SetSystime(g_server_id, &stTime);
			/*if(test)
			{
				delete test;
				test = NULL;
			}*/
	        
		 
			if(g_server_id != INVALID_HANDLE)
			{
				test[startvideocount ] = new net_video_test(g_server_id,0);
				viewiteminfo[whichviewitem].idle = 1;
				viewiteminfo[whichviewitem].ip =  ip;
				viewiteminfo[whichviewitem].ptest = test[startvideocount ];
				test[startvideocount ]->ip = ip;
				test[startvideocount ] ->start_preview(viewiteminfo[whichviewitem].pwin->m_hWnd,0);//m_cbo_connect_mode.GetCurSel());
				//test[startvideocount ] ->start_preview(NULL,0);//m_cbo_connect_mode.GetCurSel());
				
				test[startvideocount ] ->register_draw(draw_fun,(long)test[startvideocount ]);
				test[startvideocount ] ->enable_audio_preview(1);
				::GetLocalTime(&stLocal);  
			    //显示时间的间隔。  
				wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
					   stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
					   stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
					   stLocal.wMilliseconds,stLocal.wDayOfWeek);  
				sprintf(filemovie,"movie\\%s+%d+%s.mp4", ip,0,chBuf);
				test[startvideocount ]->filemovie = filemovie;
				test[startvideocount]->save_to_file(filemovie);
				startvideocount++;
			}

		}
		else
		{
			CString tmpstr = "登录 ";
			AfxMessageBox(tmpstr+ip+" 失败!");
		}
	}
}

void CALLBACK CAMainDlg::draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser )
{
	CAMainDlg* dlg = thisp;
	net_video_test *ntest = (net_video_test*) nUser;

	//CAMainDlg* dlg = (CAMainDlg*)nUser;
	//CWnd * dlg1 = (CWnd *) nUser;
	//HDC hhDc = ::GetDC(dlg1->m_hWnd);
	//CDC* dc = CDC::FromHandle(hhdc);

	CWnd* pWnd = CWnd::FromHandle(ntest->window_handle());
	//CWnd* pWnd  = dlg1;
	//CDC* dc = pWnd->GetDC();
	CDC* dc = CDC::FromHandle(hDc);
	dc->SetTextColor(RGB(255,0,0));		
	dc->SetBkMode(TRANSPARENT);	

	
	
	
#if 0
#ifdef AUTO_ZOOM
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(ntest->window_handle());
	pWnd->GetClientRect(&rt);
	dc->DrawText("在窗口中画框来启动点击放大",&rt,DT_SINGLELINE|DT_LEFT|DT_NOCLIP);

	dc->SelectStockObject(NULL_BRUSH);
	CPen newPen(0,1,RGB(0,0,255));
	dc->SelectObject(&newPen);	
	rt = dlg->auto_zoom_test.get_zoom_rt();
	dc->Rectangle(&rt);
#else
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(ntest->window_handle());
	pWnd->GetClientRect(&rt);
	dc->DrawText("鼠标左键启动图像缩放,右键关闭缩放，滚轮控制放大倍速,双击窗口/全屏切换!",&rt,DT_SINGLELINE|DT_LEFT|DT_NOCLIP);
#endif
#endif

	//显示数据
	RECT rt;
	
	pWnd->GetClientRect(&rt);
	if(g_recved_len > 0)
	{
		CString str;
		str.Format("%uKbps / %uKbps",(unsigned long)g_recved_len / 1024 * 8,(unsigned long)g_total_len / 1024 * 8);
		dc->DrawText(str,&rt,DT_SINGLELINE|DT_BOTTOM|DT_NOCLIP);
	}

	if(ntest->t_rfid_data.len > 0
		&& (time(NULL) - ntest->t_rfid_data.tm) < 3)
	{		
		rt.top = rt.bottom - 20;
		CString str;
		for(int i = 0; i < ntest->t_rfid_data.len; i++)
		{
			CString tmp;
			tmp.Format("%02x",ntest->t_rfid_data.buf[i]);
			str = str + " " + tmp;
		}
		
		dc->DrawText(str.GetBuffer(0),&rt,DT_BOTTOM | DT_LEFT);	
		
		TRACE("this is rfid is %s\n",str);
		CAMainDlg::GetRfidPic(str ,ntest);  // 产生rfid图像
	}


	CDC::DeleteTempMap();
}




void CAMainDlg::GetRfidPic(CString str, void *pptest)
{
	net_video_test *ntest = (net_video_test*) pptest;
	static char tmpuid[20];
	
	char filebmp[255];
   // CString filebmp;
	//filebmp.Format(_T("%s.bmp"),str);one =
	sprintf(filebmp,"%s.bmp",str.GetBuffer(0));
	 
    memcpy(tmpuid,filebmp+25,18);
	   
	
	
	 
	Beep( 750, 300 );
	
	TCHAR chBuf[256];
	SYSTEMTIME stLocal;  
    ::GetLocalTime(&stLocal);  
     //显示时间的间隔。  
    wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
               stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
               stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
               stLocal.wMilliseconds,stLocal.wDayOfWeek);  
	sprintf(filebmp,"%s+%s+%s.bmp",ntest->ip,tmpuid,chBuf);
	ntest->save_to_bmp(filebmp);
//	thisp->Show_picture(filebmp);


	 CString labelnum;
	 labelnum.Format("%s",tmpuid);
	 labelnum.Remove(' ');
	 char datetimestr[255];
	 sprintf(datetimestr,("%04u-%02u-%02u %02u:%02u:%02u"),  \
               stLocal.wYear, stLocal.wMonth, stLocal.wDay,   \
               stLocal.wHour, stLocal.wMinute, stLocal.wSecond) ;
               
	 int ret =1;
	 char * pErrMsg = 0;
	 char sSQL5[255]="insert into record(labelnum,date,pic,ip) values('%s','%s','%s','%s');";
	 sprintf(sSQL5,"insert into record(labelnum,date,pic,ip) values('%s','%s','%s','%s');",labelnum,datetimestr,filebmp,ntest->ip);
	 ret = sqlite3_exec( thisp->db, sSQL5, 0, 0, &pErrMsg);
	 if ( ret != SQLITE_OK ){		 
		AfxMessageBox(sqlite3_errmsg(thisp->db));
		return;
	 }
	 char sSQL6[255]="insert into vinfo(labelnum,starttime,endtime,filename,endflag,ip) values('%s','%s','%s','%s',%d,'%s');";
	 int bingoindex = -1;
	 static int sumcount = 0;
	 for (int i=0;i<1024;i++){
		   if (labelinfo[i].sLabel == labelnum){
			   bingoindex = i;
			   break;
		   }
		   continue;
	 }
	 if (   bingoindex == -1){  //数据列表中没有找到rfid，说明是新的
			labelinfo[sumcount].sLabel = labelnum;
			labelinfo[sumcount].labelleft = 1;
			labelinfo[sumcount].timeLast = stLocal;
			labelinfo[sumcount].ip  = ntest->ip;
			sprintf(sSQL6,"insert into vinfo(lablenum,starttime,filename,endflag,ip) values('%s','%s','%s',%d,'%s');",labelnum,datetimestr,ntest->filemovie,0,ntest->ip);
			ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);				  
			if ( ret != SQLITE_OK ){				 
				AfxMessageBox(sqlite3_errmsg(thisp->db));
				return;
			}
			labelinfo[sumcount].id = sqlite3_last_insert_rowid( thisp->db);
			sumcount++ ;
			return;
	 }		 
		   
	if (TimeDiff(stLocal, labelinfo[bingoindex].timeLast)> 10*1000 ||  labelinfo[bingoindex].ip != ntest->ip) {
		labelinfo[bingoindex].labelleft = 1;
		char datetimestr1[255];
		sprintf(datetimestr1,("%04u-%02u-%02u %02u:%02u:%02u"),  \
                    labelinfo[bingoindex].timeLast.wYear, labelinfo[bingoindex].timeLast.wMonth, labelinfo[bingoindex].timeLast.wDay,   \
                    labelinfo[bingoindex].timeLast.wHour, labelinfo[bingoindex].timeLast.wMinute, labelinfo[bingoindex].timeLast.wSecond) ;
		sprintf(sSQL6,"update vinfo set endtime = '%s',endflag= %d where id = %d ;",datetimestr1,1,labelinfo[bingoindex].id );
		ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);
	    if ( ret != SQLITE_OK ){		    
		    AfxMessageBox(sqlite3_errmsg(thisp->db));
		    return;
	    }
		labelinfo[bingoindex].timeLast = stLocal;
		sprintf(sSQL6,"insert into vinfo(lablenum,starttime,filename,endflag,ip) values('%s','%s','%s',%d,'%s');",labelnum,datetimestr,filemovie,0,ntest->ip);
		ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);
	    if ( ret != SQLITE_OK ){		    
		    AfxMessageBox(sqlite3_errmsg(thisp->db));
		    return;
	    }
		labelinfo[bingoindex].id = sqlite3_last_insert_rowid( thisp->db);
		labelinfo[bingoindex].ip = ntest->ip;
		return;

	}else{
		  
		labelinfo[bingoindex].labelleft++;
		labelinfo[bingoindex].timeLast = stLocal;
		return;
		  
	}


	 
	
	

}


 __int64  TimeDiff(SYSTEMTIME  left,SYSTEMTIME  right)  
{  
           CTime  tmLeft(left.wYear,left.wMonth,left.wDay,0,0,0);  
           CTime  tmRight(left.wYear,left.wMonth,left.wDay,0,0,0);  
           CTimeSpan  sp;  
           sp  =  tmLeft  -  tmRight;
           long  lLMinllis  =  (left.wHour*3600  +  left.wMinute*60  +  left.wSecond)*1000  +  left.wMilliseconds;  
           long  lRMinllis  =  (right.wHour*3600  +  right.wMinute*60  +  right.wSecond)*1000  +  right.wMilliseconds;  

           return  (__int64)sp.GetDays()*86400000  +  (lLMinllis  -  lRMinllis);  //此处返回毫秒，可用根据自己的格式需要进行转换，如时分秒
}

int CAMainDlg::SearchViewItemIdle()
{
	if (setcustom == -1){
		for (int i=0;i<16;i++){
			if (viewiteminfo[i].idle  == 0){
				return i;

			}



		}
		return -1;
	}

	return setcustom;

	

}
void CAMainDlg::InitViewItemInfo()
{
 
	CWnd *p[16];
	p[0] =  GetDlgItem(IDC_STATIC1);
	p[1] =  GetDlgItem(IDC_STATIC2);
	p[2] =  GetDlgItem(IDC_STATIC3);
	p[3] =  GetDlgItem(IDC_STATIC4);
	p[4] =  GetDlgItem(IDC_STATIC5);
	p[5] =  GetDlgItem(IDC_STATIC6);
	p[6] =  GetDlgItem(IDC_STATIC7);
	p[7] =  GetDlgItem(IDC_STATIC8);
	p[8] =  GetDlgItem(IDC_STATIC9);
	p[9] =  GetDlgItem(IDC_STATIC10);
	p[10] =  GetDlgItem(IDC_STATIC11);
	p[11] =  GetDlgItem(IDC_STATIC12);
	p[12] =  GetDlgItem(IDC_STATIC13);
	p[13] =  GetDlgItem(IDC_STATIC14);
	p[14] =  GetDlgItem(IDC_STATIC15);
	p[15] =  GetDlgItem(IDC_STATIC16);
	for (int i=0;i<16;i++){
		viewiteminfo[i].idle  = 0;
		viewiteminfo[i].ip = "";
	    viewiteminfo[i].ptest = NULL;
	    viewiteminfo[i].pwin = p[i];

	}


}

void CAMainDlg::On32780() //停止播放
{
	HTREEITEM  hItem = m_mainiptree.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("请选择ip");
		return;
	}
	CString tmp = m_mainiptree.GetItemText(hItem);
	if (tmp == "摄像机组") {

		AfxMessageBox("请选择ip");
		return;
	}

	for (int i=15;i>=0;i--){
		if (viewiteminfo[i].ip == tmp ){

			viewiteminfo[i].ptest->stop_preview();
			//delete viewiteminfo[i].ptest;
			viewiteminfo[i].ip = "";
			viewiteminfo[i].ptest = NULL;
			viewiteminfo[i].idle  = 0;
			return;;

		}


	}
	return;

}


void CAMainDlg::On32777()  //摄像机功能配置net_demo
{
	Cnet_demoDlg dlg;
	dlg.DoModal();
}


void CAMainDlg::On32771()  //人员配置
{
	CDlgRenyuan dlg;
	dlg.DoModal();
}


void CAMainDlg::On32772()  //标签配置
{
	CDlgRenyuan dlg;
	dlg.DoModal();
}


void CAMainDlg::On32781() //关于
{
	 
	MessageBox("Visual RFID是一种融合射频识别与视频采集技术的传感器,\n\n通过RFID技术对视频场景中的人、车、物进行快速精准识别，\n\nVisual RFID既是现有物联网技术的拓展，也是智能视频技术\n\n与RFID技术进行信息融合的一次尝试。","关于视频融合技术",  MB_OK);
}


void CAMainDlg::On32783() //1x1
{
	SwitchView(1);
}


void CAMainDlg::On32784()//2x2
{
	SwitchView(2);
}


void CAMainDlg::On32785()//3x3
{
	SwitchView(3);
}


void CAMainDlg::On32786()//4x4
{
	SwitchView(4);
}


void CAMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	SYSTEMTIME stLocal;  
     ::GetLocalTime(&stLocal);  
	 
	 if (nIDEvent == 2){
		    sqlite3 * db;
		    char datetimestr1[255];
			char sSQL6[255];
			int ret =1;
			char * pErrMsg = 0;
			int i = 0;
			db=(sqlite3 *)&datetimestr1;
			if (stLocal.wMonth < 3 || stLocal.wMinute < 10 ) {
				return;
			}
			sprintf(datetimestr1,("%04u-%02u-%02u %02u:%02u:%02u"),  \
							labelinfo[i].timeLast.wYear, labelinfo[i].timeLast.wMonth, labelinfo[i].timeLast.wDay,   \
							labelinfo[i].timeLast.wHour, labelinfo[i].timeLast.wMinute, labelinfo[i].timeLast.wSecond) ;
			sprintf(sSQL6,"select * from  vinfo  where id = %d  ;",datetimestr1 );
			ret = sqlite3_exec( db, sSQL6, 0, 0, &pErrMsg);
			memset(pErrMsg-15,0,200);
			if ( ret != SQLITE_OK ){
					printf(sqlite3_errmsg(db));
					
			}
			return;
	 }
	 if (nIDEvent == 3){
		 for (int i=0;i<1024;i++){			
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
					  printf(sqlite3_errmsg(thisp->db));
					  continue;
				}
				labelinfo[i].labelleft = 0;
			}
		 }
	 }

	CDialogEx::OnTimer(nIDEvent);
}


void CAMainDlg::On32787() //考勤数据导出为excel
{
	CExcelOutDlg dlg;
	dlg.DoModal();
}


void CAMainDlg::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint   pt;     
    UINT   nFlags;     
    ::GetCursorPos(&pt);     
    ::ScreenToClient(m_mainiptree.m_hWnd, &pt);     
    HTREEITEM   hItem   =   m_mainiptree.HitTest(pt,&nFlags);  

    if(hItem == NULL)  return;   //检查是否有item选中

	CPoint _point;  
    ::GetCursorPos(&_point);  
    CMenu _menu;  
    VERIFY(_menu.LoadMenu(IDR_MENU2));  
    //::SetMenu(m_hWnd,&_menu);  
    CMenu* _pPopmenu = _menu.GetSubMenu(0);  
    ASSERT(_pPopmenu != NULL);  
    _pPopmenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN,_point.x,_point.y,this);  

	*pResult = 0;
}


void CAMainDlg::On32792() //1x1
{
	SwitchView(1);
}


void CAMainDlg::On32793() //2x2
{
	SwitchView(2);
}


void CAMainDlg::On32794() //3x3
{
	SwitchView(3);
}


void CAMainDlg::On32795() //4x4
{
	SwitchView(4);
}


void CAMainDlg::On32788() //列表menu启动
{
	On32778();
}


void CAMainDlg::On32789() //列表menu暂停
{
	// TODO: 在此添加命令处理程序代码
}


void CAMainDlg::On32790() //列表menu停止
{
	On32780();
}


void CAMainDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonDown(nFlags, point);
}


BOOL CAMainDlg::PreTranslateMessage(MSG* pMsg)
{
	 if(pMsg->message == WM_RBUTTONDOWN){

		 for (int i=0;i<16;i++){
			 RECT rect;
			 viewiteminfo[i].pwin->GetWindowRect(&rect);
			 if (pMsg->pt.x>rect.left && pMsg->pt.x <rect.right &&   pMsg->pt.y >rect.top &&  pMsg->pt.y <rect.bottom){
				
				CMenu _menu;  
				VERIFY(_menu.LoadMenu(IDR_MENU3));  
				//::SetMenu(m_hWnd,&_menu);  
				CMenu* _pPopmenu = _menu.GetSubMenu(0);  
				ASSERT(_pPopmenu != NULL);  
				 
				if (viewiteminfo[i].idle == 1){
					_pPopmenu->EnableMenuItem(ID_32988,  MF_GRAYED);//menu启动为灰
					_pPopmenu->EnableMenuItem(ID_32990,  MF_ENABLED);//menu停止为能
				}else{
					_pPopmenu->EnableMenuItem(ID_32988,  MF_ENABLED);//menu启动为能
					_pPopmenu->EnableMenuItem(ID_32990,  MF_GRAYED);//menu停止为灰
				}
				_pPopmenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN,pMsg->pt.x,pMsg->pt.y,this);
				whichoneclick = i;
				break;
			 }
		 }
		 
		 
			 
			
		 

	 }

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAMainDlg::On32988() //menu 启动
{
	
	setcustom = whichoneclick;
	On32778(); //启动ip摄像机
	setcustom = -1;
}


void CAMainDlg::On32989() //menu 暂停
{
	// TODO: 在此添加命令处理程序代码
}


void CAMainDlg::On32990() //menu停止
{
	viewiteminfo[whichoneclick].ptest->stop_preview();
	//delete viewiteminfo[whichoneclick].ptest;
	viewiteminfo[whichoneclick].ip = "";
	viewiteminfo[whichoneclick].ptest = NULL;
	viewiteminfo[whichoneclick].idle  = 0;
}


void CAMainDlg::On327192()//1x1
{
	SwitchView(1);
}


void CAMainDlg::On327193()//2x2
{
	SwitchView(2);
}


void CAMainDlg::On327194()//3x3
{
	SwitchView(3);
}


void CAMainDlg::On327195() //4x4
{
	SwitchView(4);
}


void CAMainDlg::On32805()//网页配置
{
	HTREEITEM  hItem = m_mainiptree.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("请选择ip");
		return;
	}
	CString tmp = m_mainiptree.GetItemText(hItem);
	if (tmp == "摄像机组") {

		AfxMessageBox("请选择ip");
		return;
	}

	CWebWeb dlg;
	dlg.url = tmp;
	dlg.DoModal();
}
