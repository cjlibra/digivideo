// DlgLog.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLog dialog


CDlgLog::CDlgLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLog)
	DDX_Control(pDX, IDC_COMBO1, m_cbo_type);
	DDX_Control(pDX, IDC_DATE_BEG_Y, m_date);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialog)
	//{{AFX_MSG_MAP(CDlgLog)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLog message handlers
extern USER_HANDLE g_server_id;
extern int g_window_count;

void CDlgLog::close()
{

}

char* str_type[] = {
	"异常",
	"操作",
	"报警",
};

char* str_msg[] = {
	"报警输入",
	"报警输出",
	"移动侦测报警开始",
	"移动侦测报警结束",
	"遮挡报警开始",
	"遮挡报警结束",
	"未知",
	"信号丢失",
	"非法访问",
	"硬盘满",
	"硬盘错误",
	"MODEM 掉线",
	"IP地址冲突",
	"未知",
	"开机",
	"关机",
	"非法关机",
	"本地登陆",
	"本地注销登陆",
	"本地配置参数",
	"本地按文件回放",
	"本地按时间回放",
	"本地开始录像",
	"本地停止录像",
	"本地云台控制",
	"本地预览",
	"本地修改时间",
	"本地升级",
	"远程登录",
	"远程注销登陆",
	"远程开始录像",
	"远程停止录像",
	"开始透明传输",
	"停止透明传输",
	"远程获得参数",
	"远程配置参数",
	"远程获得状态",
	"远程布防",
	"远程撤防",
	"远程重启",
	"开始语音对讲",
	"停止语音对讲",
	"远程升级",
	"远程按文件回放",
	"远程按时间回放",
	"远程云台控制",
	"未知"
};

BOOL CDlgLog::refresh()
{
	m_list.DeleteAllItems();

	int log_type = m_cbo_type.GetCurSel();
	if(log_type < 0 || log_type >= 4)
	{
		return FALSE;
	}

	tLogInfo log_info;
	memset(&log_info,0,sizeof(log_info));

	log_info.majorType = log_type;
	m_date.GetTime(&log_info.beg);
	log_info.end = log_info.beg;
	log_info.beg.wHour = log_info.beg.wMinute = log_info.beg.wSecond = 0;
	log_info.end.wHour = 23; log_info.end.wMinute = log_info.end.wSecond = 59;

	long log_handle = HW_NET_SET_FindLog(g_server_id,&log_info);
	if(log_handle == -1)
	{
		MessageBox("搜索日志失败!");
		return FALSE;
	}

	int count = 0;
	CString str;
	while (1)
	{
		tLogInfo resLog;
		int ret = HW_NET_SET_FindNextLog(log_handle,&resLog);	
		if(ret == HWNET_ISFINDING)
		{
			Sleep(1);
			continue;
		}
		if (ret == HWNET_NOFILE )
		{
			TRACE("No Log(s) found.");
			break;
		}
		if(ret == HWNET_NOMOREFILES)
		{
			TRACE("%d Log(s) found.",count);
			break;
		}
		
		str.Format("%02d:%02d:%02d",resLog.end.wHour,resLog.end.wMinute,resLog.end.wSecond);
		m_list.InsertItem(count,str);
		m_list.SetItemText(count,1,str_type[resLog.majorType - 1]);
		
		m_list.SetItemText(count,2,resLog.local_operator);
		m_list.SetItemText(count,3,resLog.remote_operator);

		
		m_list.SetItemText(count,4,str_msg[resLog.minorType]);

		str.Format("%d",resLog.slot);
		m_list.SetItemText(count,5,str);
		str.Format("%d",resLog.nAlarmInPort);
		m_list.SetItemText(count,6,str);

		count++;
	}
	HW_NET_SET_FindLogClose(log_handle);

	
	return FALSE;
}

BOOL CDlgLog::commit()
{
	return FALSE;
}

BOOL CDlgLog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list.InsertColumn(0,"时间",LVCFMT_LEFT,100);
	m_list.InsertColumn(1,"类型",LVCFMT_LEFT,100);
	m_list.InsertColumn(2,"本地用户",LVCFMT_LEFT,100);
	m_list.InsertColumn(3,"网络用户",LVCFMT_LEFT,100);
	m_list.InsertColumn(4,"信息",LVCFMT_LEFT,100);	
	m_list.InsertColumn(5,"通道",LVCFMT_LEFT,100);
	m_list.InsertColumn(6,"报警器号",LVCFMT_LEFT,100);

	m_cbo_type.AddString("全部");
	m_cbo_type.AddString("异常");
	m_cbo_type.AddString("操作");
	m_cbo_type.AddString("报警");	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLog::OnButton1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}
