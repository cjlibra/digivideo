// AMainDlg.cpp : ʵ���ļ�
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


CAMainDlg *thisp;
static   int g_window_count = 0;
static  long g_server_version = 0;
static net_video_test* test = NULL;
static char filemovie[256];
static UINT64 g_recved_len = 0;
static UINT64 g_total_len = 0;

typedef struct _VIEWITEMINFO{
	int idle;
	CString ip;
	net_video_test* ptest;
	CWnd * pwin;

}VIEWITEMINFO;
static VIEWITEMINFO viewiteminfo[9];

typedef struct _LABELINFO{
	CString sLabel;
    SYSTEMTIME timeLast;
	int  labelleft;
	int id;

}LABELINFO;
static LABELINFO labelinfo[1024];

static __int64	TimeDiff(SYSTEMTIME  left,SYSTEMTIME  right)  ;
static int onlyone = 0;
// CAMainDlg �Ի���

IMPLEMENT_DYNAMIC(CAMainDlg, CDialogEx)

CAMainDlg::CAMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAMainDlg::IDD, pParent)
{
	thisp = this;
}

CAMainDlg::~CAMainDlg()
{
	sqlite3_close(db);
	if(test)
	{
		test->stop_preview();
		delete test;
		test = NULL;
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
END_MESSAGE_MAP()


// CAMainDlg ��Ϣ��������
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
	 hItem = this->m_mainiptree.InsertItem("�������",NULL,NULL,NULL);

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
BOOL CAMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    CWinApp	 *pApp=AfxGetApp();
    HICON hIcon=pApp->LoadIcon(IDR_MAINFRAME);
    SetIcon(hIcon,TRUE);//16 

	int ret = 1;
	thisp->db = 0;
	ret = sqlite3_open("medb.s3db", &thisp->db);
      
    if ( ret != SQLITE_OK )
    {
      fprintf(stderr, "�޷������ݿ�: %s", sqlite3_errmsg(thisp->db));
      return(1);
    }
      
    printf("���ݿ����ӳɹ�!\n");

	ShowWindow(SW_MAXIMIZE);
	SwitchView(3);

	RefreshTree(); //��ʾIPtree
	InitViewItemInfo(); //��ʾ�����Գ�ʼ��

	HW_NET_Init(5198);	
	HW_NET_SetUdpBasePort(8000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAMainDlg::On32773() //�豸���ã����������ip����
{
	CIphostDlg dlg;
	dlg.DoModal();

}


void CAMainDlg::OnMenu() //��Ƶ����
{
       CDlgVcatch  dlg;
	   dlg.DoModal();
}


void CAMainDlg::On32775() //ͼ������
{
	
	CRecordDlg  dlg;
	dlg.DoModal();
	  
}


void CAMainDlg::On32778() //����ip�����
{
	
	HTREEITEM  hItem = m_mainiptree.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("��ѡ��ip");
		return;
	}
	CString tmp = m_mainiptree.GetItemText(hItem);
	if (tmp == "�������") {

		AfxMessageBox("��ѡ��ip");
		return;
	}
	int whichviewitem = SearchViewItemIdle();
	if (whichviewitem == -1){
		AfxMessageBox("������ʾ�������ˣ���ֹͣ������������");
		return;
	}
	 
	USER_HANDLE g_server_id = INVALID_HANDLE; 
	if(g_server_id != INVALID_HANDLE)
	{
		//KillTimer(TIMER_HEARTBEAR);
		HW_NET_Logout(g_server_id);
		g_server_id = INVALID_HANDLE;
	}
	char ip[255];
	if(g_server_id == INVALID_HANDLE)
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
				sprintf(str,"ͨ��%d",i + 1);
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

			AfxMessageBox("��¼�ɹ�!");
			SYSTEMTIME stTime;

            GetLocalTime(&stTime);
			HW_NET_SET_SetSystime(g_server_id, &stTime);
			if(test)
			{
				delete test;
				test = NULL;
			}
	        
		 
			if(g_server_id != INVALID_HANDLE)
			{
				test = new net_video_test(g_server_id,0);
				viewiteminfo[whichviewitem].idle = 1;
				viewiteminfo[whichviewitem].ip =  ip;
				viewiteminfo[whichviewitem].ptest = test;
				test->start_preview(viewiteminfo[whichviewitem].pwin->m_hWnd,0);//m_cbo_connect_mode.GetCurSel());
				test->register_draw(draw_fun,(long)this);
				test->enable_audio_preview(1);
				::GetLocalTime(&stLocal);  
			    //��ʾʱ��ļ����  
				wsprintf(chBuf,_T("%u-%u-%u %u-%u-%u %u %d"), 
					   stLocal.wYear, stLocal.wMonth, stLocal.wDay,  
					   stLocal.wHour, stLocal.wMinute, stLocal.wSecond,  
					   stLocal.wMilliseconds,stLocal.wDayOfWeek);  
				sprintf(filemovie,"c:\\movie\\%s+%d+%s.mp4", ip,0,chBuf);
				test->save_to_file(filemovie);
			}

		}
		else
		{
			AfxMessageBox("��¼ʧ��!");
		}
	}
}

void CALLBACK CAMainDlg::draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser)
{
	CDC* dc = CDC::FromHandle(hDc);
	dc->SetTextColor(RGB(255,0,0));		
	dc->SetBkMode(TRANSPARENT);	

	CAMainDlg* dlg = (CAMainDlg*)nUser;

	
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
		CAMainDlg::GetRfidPic(str);  // ����rfidͼ��
	}


	CDC::DeleteTempMap();
}




void CAMainDlg::GetRfidPic(CString str)
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
	 char sSQL5[255]="insert into record(labelnum,date,pic) values('%s','%s','%s');";
	 sprintf(sSQL5,"insert into record(labelnum,date,pic) values('%s','%s','%s');",labelnum,datetimestr,filebmp);
	 ret = sqlite3_exec( thisp->db, sSQL5, 0, 0, &pErrMsg);
	 if ( ret != SQLITE_OK ){
		//AfxMessageBox(sqlite3_errmsg(this->myparent->thisp->db));
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
		                //AfxMessageBox(sqlite3_errmsg(this->myparent->thisp->db));
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
		            //AfxMessageBox(sqlite3_errmsg(this->myparent->thisp->db));
		            printf(sqlite3_errmsg(thisp->db));
		            break;
	           }
			   labelinfo[i].timeLast = stLocal;
			   sprintf(sSQL6,"insert into vinfo(lablenum,starttime,filename,endflag) values('%s','%s','%s',%d);",labelnum,datetimestr,filemovie,0);
			   ret = sqlite3_exec( thisp->db, sSQL6, 0, 0, &pErrMsg);
	           if ( ret != SQLITE_OK ){
		            //AfxMessageBox(sqlite3_errmsg(this->myparent->thisp->db));
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

int CAMainDlg::SearchViewItemIdle()
{
	for (int i=0;i<9;i++){
		if (viewiteminfo[i].idle  == 0){
			return i;

		}



	}

	return -1;

}
void CAMainDlg::InitViewItemInfo()
{
 
	CWnd *p[9];
	p[0] =  GetDlgItem(IDC_STATIC1);
	p[1] =  GetDlgItem(IDC_STATIC2);
	p[2] =  GetDlgItem(IDC_STATIC3);
	p[3] =  GetDlgItem(IDC_STATIC4);
	p[4] =  GetDlgItem(IDC_STATIC5);
	p[5] =  GetDlgItem(IDC_STATIC6);
	p[6] =  GetDlgItem(IDC_STATIC7);
	p[7] =  GetDlgItem(IDC_STATIC8);
	p[8] =  GetDlgItem(IDC_STATIC9);
	for (int i=0;i<9;i++){
		viewiteminfo[i].idle  = 0;
		viewiteminfo[i].ip = "";
	    viewiteminfo[i].ptest = NULL;
	    viewiteminfo[i].pwin = p[i];

	}


}

void CAMainDlg::On32780() //ֹͣ����
{
	HTREEITEM  hItem = m_mainiptree.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("��ѡ��ip");
		return;
	}
	CString tmp = m_mainiptree.GetItemText(hItem);
	if (tmp == "�������") {

		AfxMessageBox("��ѡ��ip");
		return;
	}

	for (int i=8;i>=0;i--){
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


void CAMainDlg::On32777()  //�������������net_demo
{
	Cnet_demoDlg dlg;
	dlg.DoModal();
}


void CAMainDlg::On32771()  //��Ա����
{
	CDlgRenyuan dlg;
	dlg.DoModal();
}


void CAMainDlg::On32772()  //��ǩ����
{
	CDlgRenyuan dlg;
	dlg.DoModal();
}


void CAMainDlg::On32781() //����
{
	 
	MessageBox("Visual RFID��һ���ں���Ƶʶ������Ƶ�ɼ������Ĵ�����,\n\nͨ��RFID��������Ƶ�����е��ˡ���������п��پ�׼ʶ��\n\nVisual RFID����������������������չ��Ҳ��������Ƶ����\n\n��RFID����������Ϣ�ںϵ�һ�γ��ԡ�","������Ƶ�ںϼ���",  MB_OK);
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