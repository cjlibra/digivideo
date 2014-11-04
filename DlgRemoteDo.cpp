// DlgRemoteDo.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgRemoteDo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRemoteDo dialog


CDlgRemoteDo::CDlgRemoteDo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRemoteDo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRemoteDo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRemoteDo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRemoteDo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRemoteDo, CDialog)
	//{{AFX_MSG_MAP(CDlgRemoteDo)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRemoteDo message handlers
extern USER_HANDLE g_server_id;
BOOL CDlgRemoteDo::refresh()
{
	return FALSE;
}

BOOL CDlgRemoteDo::commit()
{
	return FALSE;
}

void CDlgRemoteDo::close()
{
}

void CDlgRemoteDo::OnButton3() 
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME time;
	GetLocalTime(&time);

	if(HW_NET_SET_SetSystime(g_server_id,&time) == FALSE)
	{
		MessageBox("����ʱ��ʧ��!");
	}
}

void CDlgRemoteDo::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
	if(MessageBox("�Ƿ�����Զ������?","ϵͳ��ʾ",MB_YESNO) == IDYES)
	{
		if(HW_NET_Reboot(g_server_id) == FALSE)
		{
			MessageBox("��������ʧ��!");
		}else{
			MessageBox("Զ��������������...,���Ժ�������!");
		}
	}
}

void CDlgRemoteDo::OnButton4() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("�Ƿ�ر�Զ������?","ϵͳ��ʾ",MB_YESNO) == IDYES)
	{
		if(HW_NET_ShutDown(g_server_id) == FALSE)
		{
			MessageBox("�ر�����ʧ��!");
		}else{
			MessageBox("Զ�������ѹر�!");
		}
	}
}

void CDlgRemoteDo::OnButton8() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("�Ƿ�ָ�Ĭ��ֵ?","ϵͳ��ʾ",MB_YESNO) == IDYES)
	{
		if(HW_NET_Restore(g_server_id) == TRUE)
		{
			MessageBox("�ָ�Ĭ��ֵ�ɹ��������������豸!");
// 			MessageBox("ϵͳ����������!");
// 			HW_NET_Reboot(g_server_id);
		}else{
			MessageBox("�ָ�Ĭ��ֵʧ��!");
		}
	}
}

void CDlgRemoteDo::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		int upgradestate = 0;
		if (!HW_NET_SET_GetUpgradeState(g_server_id,&upgradestate))
		{		
			KillTimer(nIDEvent);		
			HW_NET_SET_StopSendUpgFile(g_server_id);
			return;
		}
		
		switch(upgradestate)	
		{
		case -1:// failed
			TRACE("����ʧ��!\n");
			break;
		case 0://idle
			TRACE("idle\n");
			break;
		case 1:// success
			TRACE("�����ɹ�\n");
			break;
		case 2:// in the process of upgrading 
			TRACE("����������...\n");
			break;				
		case 6:// the version of language is incorrect
			TRACE("sending head...\n");
			break;
		case 7:// the version of language is incorrect
			TRACE("sending file...\n");
			break;
		}
		
		if(upgradestate != 2 
			&& upgradestate != 6 
			&& upgradestate != 7) 
		{
			KillTimer(nIDEvent);
			HW_NET_SET_StopSendUpgFile(g_server_id);
			
			if(upgradestate == 1)
			{
				MessageBox("�������!����������Զ���豸!");
			}			
		}
	}

	if(nIDEvent == 2)
	{
		int progress;
		HW_NET_SET_GetFormatProgress(g_server_id,&progress);
		if(progress == 100)
		{
			KillTimer(nIDEvent);
			MessageBox("��ʽ�����!");		
		}
	}

	CDialog::OnTimer(nIDEvent); 
}

BOOL CDlgRemoteDo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRemoteDo::OnButton9() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, 
		NULL,
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"�����ļ�(*.*)|*.*||", this);
	
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	
	int ready=-1;
	if (!HW_NET_SET_UpgradeRequest(g_server_id,&ready))
	{		
		MessageBox("��������ʧ��!");
		return ;
	}
	
	if (ready!=0)
	{
		MessageBox("��ǰDVR���ڱ��������޷����������Ժ��ٽ���������");
		return;
	}
	
	if(!HW_NET_SET_SendUpgradeFile(g_server_id,dlg.GetPathName()))
	{
		MessageBox("��������ʧ��!");
		return;
	}	
	
	SetTimer(1,1000,NULL);
}

void CDlgRemoteDo::OnButton10() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("�Ƿ��ʽ��Զ������?","ϵͳ��ʾ",MB_YESNO) == IDYES)
	{
		if(HW_NET_SET_FormatHarddisk(g_server_id,0) == TRUE)
		{
			MessageBox("���ڸ�ʽ��Ӳ��,�����ĵȺ�");
			SetTimer(2,5000,NULL);
		}else{
			MessageBox("��ʽ������ʧ��!");
		}
	}
}
