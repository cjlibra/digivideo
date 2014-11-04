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
	"�쳣",
	"����",
	"����",
};

char* str_msg[] = {
	"��������",
	"�������",
	"�ƶ���ⱨ����ʼ",
	"�ƶ���ⱨ������",
	"�ڵ�������ʼ",
	"�ڵ���������",
	"δ֪",
	"�źŶ�ʧ",
	"�Ƿ�����",
	"Ӳ����",
	"Ӳ�̴���",
	"MODEM ����",
	"IP��ַ��ͻ",
	"δ֪",
	"����",
	"�ػ�",
	"�Ƿ��ػ�",
	"���ص�½",
	"����ע����½",
	"�������ò���",
	"���ذ��ļ��ط�",
	"���ذ�ʱ��ط�",
	"���ؿ�ʼ¼��",
	"����ֹͣ¼��",
	"������̨����",
	"����Ԥ��",
	"�����޸�ʱ��",
	"��������",
	"Զ�̵�¼",
	"Զ��ע����½",
	"Զ�̿�ʼ¼��",
	"Զ��ֹͣ¼��",
	"��ʼ͸������",
	"ֹͣ͸������",
	"Զ�̻�ò���",
	"Զ�����ò���",
	"Զ�̻��״̬",
	"Զ�̲���",
	"Զ�̳���",
	"Զ������",
	"��ʼ�����Խ�",
	"ֹͣ�����Խ�",
	"Զ������",
	"Զ�̰��ļ��ط�",
	"Զ�̰�ʱ��ط�",
	"Զ����̨����",
	"δ֪"
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
		MessageBox("������־ʧ��!");
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
	m_list.InsertColumn(0,"ʱ��",LVCFMT_LEFT,100);
	m_list.InsertColumn(1,"����",LVCFMT_LEFT,100);
	m_list.InsertColumn(2,"�����û�",LVCFMT_LEFT,100);
	m_list.InsertColumn(3,"�����û�",LVCFMT_LEFT,100);
	m_list.InsertColumn(4,"��Ϣ",LVCFMT_LEFT,100);	
	m_list.InsertColumn(5,"ͨ��",LVCFMT_LEFT,100);
	m_list.InsertColumn(6,"��������",LVCFMT_LEFT,100);

	m_cbo_type.AddString("ȫ��");
	m_cbo_type.AddString("�쳣");
	m_cbo_type.AddString("����");
	m_cbo_type.AddString("����");	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLog::OnButton1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}
