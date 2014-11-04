// DlgQuality.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgQuality.h"
#include "howellnetsdk.h"

// CDlgQuality �Ի���

IMPLEMENT_DYNAMIC(CDlgQuality, CDialog)

CDlgQuality::CDlgQuality(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuality::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQuality)
	m_max_bps = 0;
	m_gop_size = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}

CDlgQuality::~CDlgQuality()
{
}

void CDlgQuality::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgQuality)
	DDX_Control(pDX, IDC_COMBO1, m_cbo_slot);
	DDX_Control(pDX, IDC_COMBO3, m_cbo_quality);
	DDX_Control(pDX, IDC_COMBO4, m_cbo_max_bps);
	DDX_Control(pDX, IDC_COMBO5, m_cbo_encode_type);
	DDX_Control(pDX, IDC_COMBO8, m_cbo_frame);
	DDX_Control(pDX, IDC_COMBO6, m_cbo_stream_type);
	DDX_Control(pDX, IDC_COMBO7, m_cbo_bps_type);
	DDX_Text(pDX, IDC_EDT_MAX_BPS, m_max_bps);
	DDX_Text(pDX, IDC_EDT_GOP, m_gop_size);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQuality, CDialog)
	//{{AFX_MSG_MAP(CDlgQuality)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeCombo4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgQuality ��Ϣ�������
extern USER_HANDLE g_server_id ;
extern int g_window_count;
void CDlgQuality::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	close();

	CDialog::OnClose();
}

void CDlgQuality::close()
{

}

BOOL CDlgQuality::OnInitDialog()
{
	CDialog::OnInitDialog();	

	CString str;
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("ͨ��%d",i + 1);
		m_cbo_slot.AddString(str);
	}

	m_cbo_quality.AddString("���");
	m_cbo_quality.AddString("�κ�");
	m_cbo_quality.AddString("�Ϻ�");
	m_cbo_quality.AddString("һ��");

	m_cbo_max_bps.AddString("����");
	m_cbo_max_bps.AddString("��С");
	m_cbo_max_bps.AddString("��");
	m_cbo_max_bps.AddString("�ϴ�");
	m_cbo_max_bps.AddString("���");
	m_cbo_max_bps.AddString("�Զ���");
	
	m_cbo_encode_type.AddString("CIF");
	m_cbo_encode_type.AddString("D1");
	m_cbo_encode_type.AddString("720P");
	m_cbo_encode_type.AddString("1080P");

	m_cbo_stream_type.AddString("��Ƶ��");
	m_cbo_stream_type.AddString("��Ƶ��");
	m_cbo_stream_type.AddString("������");

	m_cbo_bps_type.AddString("������");
	m_cbo_bps_type.AddString("������");

	m_cbo_frame.AddString("ȫ֡��");
	for(i = 1; i <= 24; i++)
	{
		str.Format("%d",i);
		m_cbo_frame.AddString(str);
	}
	return TRUE;
}

void CDlgQuality::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	refresh();
}

BOOL CDlgQuality::commit()
{
	UpdateData(TRUE);

	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	struct tVideoQuality video_quality;
	memset(&video_quality,0,sizeof(video_quality));
	video_quality.slot = slot;
	video_quality.encodeType = m_cbo_encode_type.GetCurSel();
	if(m_cbo_max_bps.GetCurSel() != m_cbo_max_bps.GetCount() - 1)
	{
		video_quality.maxBps = m_cbo_max_bps.GetCurSel();
	}else{
		if(m_max_bps <= 200
			|| m_max_bps >= 20000)
		{
			MessageBox("�Ƿ�����");
			return FALSE;
		}

		video_quality.maxBps = 0x80000000 | m_max_bps;
	}
	
	video_quality.vbr = m_cbo_bps_type.GetCurSel();
	video_quality.framerate = m_cbo_frame.GetCurSel();
	video_quality.qualityLev = m_cbo_quality.GetCurSel();
	video_quality.reserved = m_gop_size;
	if(HW_NET_SET_SetVideoQuality(g_server_id,&video_quality) == FALSE)
	{
		MessageBox("������Ƶͼ������ʧ��!");
		return FALSE;
	}
	
	struct tStreamType video_stream;
	memset(&video_stream,0,sizeof(video_stream));
	video_stream.slot = slot;
	video_stream.type = m_cbo_stream_type.GetCurSel() + 1;
	if(HW_NET_SET_SetStreamType(g_server_id,&video_stream) == FALSE)
	{
		MessageBox("������Ƶ����ʧ��!");
		return FALSE;
	}

	MessageBox("���óɹ�!");

	return TRUE;
}

BOOL CDlgQuality::refresh()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	struct tVideoQuality video_quality;
	memset(&video_quality,0,sizeof(video_quality));
	video_quality.slot = slot;
	if(HW_NET_SET_GetVideoQuality(g_server_id,&video_quality) == FALSE)
	{
		MessageBox("��ȡ��Ƶͼ������ʧ��!");
		return FALSE;
	}
	m_cbo_quality.SetCurSel(video_quality.qualityLev);
	if(video_quality.maxBps & 0x80000000)
	{	
		m_cbo_max_bps.SetCurSel(m_cbo_max_bps.GetCount() - 1);
		m_max_bps = video_quality.maxBps & ~(1 << 31);
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(TRUE);
	}else{
		m_cbo_max_bps.SetCurSel(video_quality.maxBps);
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(FALSE);
	}
	
	m_cbo_encode_type.SetCurSel(video_quality.encodeType);
	//m_cbo_encode_type.EnableWindow(FALSE);
	m_cbo_bps_type.SetCurSel(video_quality.vbr);
	if(video_quality.framerate > 24)
	{
		//remote bug;
		video_quality.framerate = 0;
	}
	m_cbo_frame.SetCurSel(video_quality.framerate);
	m_gop_size = video_quality.reserved;

	struct tStreamType video_stream;
	memset(&video_stream,0,sizeof(video_stream));
	video_stream.slot = slot;
	if(HW_NET_SET_GetStreamType(g_server_id,&video_stream) == FALSE)
	{
		MessageBox("��ȡ��Ƶ����ʧ��!");
		return FALSE;
	}

	m_cbo_stream_type.SetCurSel(video_stream.type - 1);
	
	UpdateData(FALSE);
	return TRUE;
}

void CDlgQuality::OnSelchangeCombo4() 
{
	// TODO: Add your control notification handler code here
	
	if(m_cbo_max_bps.GetCurSel() != m_cbo_max_bps.GetCount() - 1)
	{
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(FALSE);
	}else{
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(TRUE);
	}
}
