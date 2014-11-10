// DlgVcatch.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgVcatch.h"
#include "afxdialogex.h"

#include "Kaoqing.h"
#include "net_video_test.h"

CDlgVcatch  *thisp;
static int jj;
static int idfromname;
static CString labelnumfromid;
static PLAY_HANDLE HANDLEL;
static int _searchvideo_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _searchren_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _searchlabelnum_callback(void * notused, int argc, char ** argv, char ** szColName);
static CString GetDatetimeFromFile(CString filename);

// CDlgVcatch 对话框

IMPLEMENT_DYNAMIC(CDlgVcatch, CDialogEx)

CDlgVcatch::CDlgVcatch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVcatch::IDD, pParent)
	, m_name(_T(""))
	, m_dateole(COleDateTime::GetCurrentTime())
{

}

CDlgVcatch::~CDlgVcatch()
{
}

BOOL CDlgVcatch::OnInitDialog()
{
	CDialog::OnInitDialog();
	DWORD dwStyle;
	dwStyle =m_listvideo.GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT| LVS_SHOWSELALWAYS;
	m_listvideo.SetExtendedStyle(dwStyle);
	thisp = this;
	m_listvideo.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	m_listvideo.InsertColumn(1,"标签号",LVCFMT_CENTER,200);
	m_listvideo.InsertColumn(2,"起始时间",LVCFMT_CENTER,200);
	m_listvideo.InsertColumn(3,"终止时间",LVCFMT_CENTER,200);
	m_listvideo.InsertColumn(4,"视频名",LVCFMT_CENTER,50);
	return TRUE;
	
}

void CDlgVcatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listvideo);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dateole);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datectrl);
}



BEGIN_MESSAGE_MAP(CDlgVcatch, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgVcatch::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgVcatch::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgVcatch::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDlgVcatch 消息处理程序


void CDlgVcatch::OnBnClickedButton1()
{
	UpdateData(TRUE);
	jj = 0;
	m_listvideo.DeleteAllItems();
	idfromname = 0;
	labelnumfromid = "";
	CKaoqing *pWin = (CKaoqing *) this->GetParent()->GetParent();
	char sSQL6[255] = " select * from person where name='%s';";
	char * pErrMsg = 0;
	sprintf(sSQL6," select * from person where name='%s';" ,m_name);
	sqlite3_exec( pWin->db, sSQL6, _searchren_callback, 0, &pErrMsg);
	if (idfromname == 0) return;
	sprintf(sSQL6," select * from label where id='%d';" , idfromname  );
	sqlite3_exec( pWin->db, sSQL6, _searchlabelnum_callback, 0, &pErrMsg);
	if (labelnumfromid == "") return;
	CString daychose ;
	daychose = m_dateole.Format("%Y-%m-%d");
	//m_datectrl.GetWindowText(daychose );
	sprintf(sSQL6 , " select * from vinfo  where lablenum='%s' and starttime > datetime('%s', 'utc', '+0 days');" , labelnumfromid , daychose );
	sqlite3_exec( pWin->db, sSQL6, _searchvideo_callback, 0, &pErrMsg);
}

static int _searchvideo_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	thisp->m_listvideo.InsertItem(jj,argv[0]);
	thisp->m_listvideo.SetItemText(jj,1,argv[1]);
	thisp->m_listvideo.SetItemText(jj,2,argv[2]);
	thisp->m_listvideo.SetItemText(jj,3,argv[3]);
    thisp->m_listvideo.SetItemText(jj,4,argv[4]);
	jj++;

	return 0;


}
static int _searchren_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	idfromname = atoi(argv[6]);
	return 0;
}

static int _searchlabelnum_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	labelnumfromid = argv[1];
	return 0;
}

void CDlgVcatch::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	    CString starttime ;
	    CString endtime  ;
	    CString filename ;
		POSITION pos = m_listvideo.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox("请选中列表行");
			return;
		}
		else
		{
			//while (pos)
			//{
				int iItem = m_listvideo.GetNextSelectedItem(pos);
				starttime = m_listvideo.GetItemText(iItem, 2);
				endtime  = m_listvideo.GetItemText(iItem, 3);
				filename = m_listvideo.GetItemText(iItem, 4);

			//}
		}
		int nYear, nMonth, nDate, nHour, nMin, nSec;
        sscanf(starttime, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
        COleDateTime sTimeBegin(nYear, nMonth, nDate, nHour, nMin, nSec); 
		sscanf(endtime, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
		COleDateTime sTimeEnd(nYear, nMonth, nDate, nHour, nMin, nSec); 
		CString timefromfilename;
		timefromfilename = GetDatetimeFromFile(filename);
		sscanf(timefromfilename, "%d-%d-%d %d-%d-%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
		COleDateTime sTimeFile(nYear, nMonth, nDate, nHour, nMin, nSec); 

		COleDateTimeSpan timeSpan;    //计算时间差
        timeSpan = sTimeBegin - sTimeFile; 
        long expi_second = timeSpan.GetSeconds();

		int msec;
		msec = expi_second/2;
		hwplay_stop(HANDLEL);
		HANDLEL = hwplay_open_local_file(filename);	
	    hwplay_play(HANDLEL, GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd());
		hwplay_get_played_msec(HANDLEL,&msec);

}


static CString GetDatetimeFromFile(CString filename)
{
	int firstspace = filename.Find(" ",5);
	int secondspace = filename.Find(" ",firstspace+1);
	CString result = filename.Mid(5,secondspace-5);
	return result;

}

void CDlgVcatch::OnBnClickedButton3()
{
	 hwplay_stop(HANDLEL);
}
