// DlgRemoteSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgRemoteSet.h"

// CDlgRemoteSet �Ի���

IMPLEMENT_DYNAMIC(CDlgRemoteSet, CDialog)

CDlgRemoteSet::CDlgRemoteSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRemoteSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRemoteSet)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}

CDlgRemoteSet::~CDlgRemoteSet()
{
}

void CDlgRemoteSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRemoteSet)
	DDX_Control(pDX, IDC_TAB_SET, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRemoteSet, CDialog)
	//{{AFX_MSG_MAP(CDlgRemoteSet)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SET, OnTcnSelchangeTabSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgRemoteSet ��Ϣ�������
extern USER_HANDLE g_server_id ;
extern long g_server_version;

void CDlgRemoteSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	switch(m_tab.GetCurSel())
	{
	case 0: //OSD
		{
			m_dlg_osd->commit();
			break;
		}
	case 1: //ͼ������
		{
			m_dlg_quality->commit();
			break;
		}
	case 2://����
		{
			m_dlg_net->commit();
			break;
		}

	case 3://Զ�̲���
		{
			m_dlg_remote_do->commit();
			break;
		}

	case 4://ipc feature
		{
			m_dlg_ipc_feature->commit();
			break;;
		}

	case  5://����
		{
			m_dlg_serial_port->commit();
			break;
		}

	case  6://PTZ
		{
			m_dlg_ptz->commit();
			break;
		}

	case 7://�ƶ����
		{
			m_dlg_motion->commit();
			break;
		}	

	case  8:
		{
			m_dlg_sub_quality->commit();
			break;
		}

	case 9:
		{
			m_dlg_alarm->commit();
			break;
		}
		
	case 10:
		{
			m_dlg_log->commit();
			break;
		}
		
	case 11:
		{
			m_dlg_deviceinfo->commit();
			break;
		}

	case 12:
		{
			m_dlg_alarmout->commit();
			break;
		}
			
	case 13://������
		{
			m_dlg_work_sheet->commit();
			break;
		}	
		
	case 14://ͨ������
		{
			m_dlg_channel_set->commit();
			break;
		}

	case 15://��Ƶ��ʧ
		{
			m_dlg_vlost->commit();
			break;;
		}

	case 16://mask
		{
			m_dlg_mask->commit();
			break;
		}

	case 17://��Ƶ����
		{
			m_dlg_analyze->commit();
			break;
		}

	case 18://�Զ���osd
		{
			m_dlg_custom_osd->commit();
			break;
		}

	case 19:
		{
			//¼�����
			m_dlg_ipc_record_ctrl->commit();
			break;
		}
	}
}

BOOL CDlgRemoteSet::OnInitDialog()
{
	CDialog::OnInitDialog();	
	
	m_dlg_osd = NULL;m_dlg_quality = NULL;m_dlg_remote_do = NULL;m_dlg_net = NULL;m_dlg_work_sheet = NULL;
	m_dlg_alarm = NULL;m_dlg_ptz = NULL;m_dlg_log = NULL;m_dlg_mask_alarm = NULL;m_dlg_serial_port = NULL;m_dlg_motion = NULL;
	m_dlg_channel_set = NULL;
	m_dlg_ipc_feature = NULL;
	m_dlg_sub_quality = NULL;
	m_dlg_vlost = NULL;
	m_dlg_deviceinfo = NULL;
	m_dlg_alarmout = NULL;
	m_dlg_mask = NULL;
	m_dlg_analyze = NULL;
	m_dlg_custom_osd = NULL;
	m_dlg_ipc_record_ctrl = NULL;

	//OSD����
	m_tab.InsertItem(0,"OSD����");
	m_dlg_osd = new CDlgOsd(&m_tab);
	m_dlg_osd->Create(IDD_DLG_OSD,&m_tab);
	m_dlg_osd->ShowWindow(SW_NORMAL);
	
	//ͼ������	
	m_tab.InsertItem(1,"ͼ������");
	m_dlg_quality = new CDlgQuality(&m_tab);
	m_dlg_quality->Create(IDD_DLG_QUALITY,&m_tab);
	
	//��������
	m_tab.InsertItem(2,"����");
	m_dlg_net = new CDlgNet(&m_tab);
	m_dlg_net->Create(IDD_DLG_NET,&m_tab);
	
	//Զ�̲���
	m_tab.InsertItem(3,"����");
	m_dlg_remote_do = new CDlgRemoteDo(&m_tab);
	m_dlg_remote_do->Create(IDD_DLG_REMOTE_DO,&m_tab);

	//ipcam ����
	m_tab.InsertItem(4,"ipc ����");
	m_dlg_ipc_feature = new CDlgIPCFeature(&m_tab);
	m_dlg_ipc_feature->Create(IDD_DLG_IPC_FEATURE,&m_tab);

	//com
	m_tab.InsertItem(5,"��������");
	m_dlg_serial_port = new CDlgSerialPort(&m_tab);
	m_dlg_serial_port->Create(IDD_DLG_SERIAL,&m_tab);

	//ptz
	m_tab.InsertItem(6,"PTZ����");
	m_dlg_ptz = new CDlgPtz(&m_tab);
	m_dlg_ptz->Create(IDD_DLG_PTZ,&m_tab);

	//�ƶ����
	m_tab.InsertItem(7,"�ƶ����");
	m_dlg_motion = new CDlgMotion(&m_tab);
	m_dlg_motion->Create(IDD_DLG_MOTION,&m_tab);

	//����������
	m_tab.InsertItem(8,"����������");
	m_dlg_sub_quality = new CDlgSubQuality(&m_tab);
	m_dlg_sub_quality->Create(IDD_DLG_SUB_QUALITY,&m_tab);

	//alarm
	m_tab.InsertItem(9,"����������");
	m_dlg_alarm = new CDlgAlarm(&m_tab);
	m_dlg_alarm->Create(IDD_DLG_ALARM,&m_tab);

	m_tab.InsertItem(10,"��־");
	m_dlg_log = new CDlgLog(&m_tab);
	m_dlg_log->Create(IDD_DLG_LOG,&m_tab);

	m_tab.InsertItem(11,"�豸��Ϣ");
	m_dlg_deviceinfo = new CDlgDeviceInfo(&m_tab);
	m_dlg_deviceinfo->Create(IDD_DLG_DEVICE_INFO,&m_tab);

	m_tab.InsertItem(12,"�̵������");
	m_dlg_alarmout = new AlarmOut(&m_tab);
	m_dlg_alarmout->Create(IDD_DLG_ALARM_OUT,&m_tab);

	//������
	m_tab.InsertItem(13,"������");//���NVR
	m_dlg_work_sheet = new CDlgWorkSheet(&m_tab);
	m_dlg_work_sheet->Create(IDD_DLG_WORKSHEET,&m_tab);	
	
	//ͨ������
	m_tab.InsertItem(14,"ͨ������");//���NVR
	m_dlg_channel_set = new CDlgChannelSet(&m_tab);
	m_dlg_channel_set->Create(IDD_DLG_CHANNEL_SET,&m_tab);
	
	//��Ƶ��ʧ����
	m_tab.InsertItem(15,"��Ƶ��ʧ");//���NVR
	m_dlg_vlost = new CDlgVlost(&m_tab);
	m_dlg_vlost->Create(IDD_DLG_VLOST,&m_tab);
	
	//�ڸ�
	m_tab.InsertItem(16,"�ڸ�");
	m_dlg_mask = new CDlgMask(&m_tab);
	m_dlg_mask->Create(IDD_DLG_MASK,&m_tab);

	//��Ƶ����
	m_tab.InsertItem(17,"��Ƶ����");
	m_dlg_analyze = new CDlgAnalyze(&m_tab);
	m_dlg_analyze->Create(IDD_DLG_ANALYZE,&m_tab);

	//�Զ���OSD
	m_tab.InsertItem(18,"�Զ���OSD");
	m_dlg_custom_osd = new CDlgCustomOsd(&m_tab);
	m_dlg_custom_osd->Create(IDD_DLG_CUSTOM_OSD,&m_tab);

	//¼�����
	m_tab.InsertItem(19,"IPC SD��¼�����");
	m_dlg_ipc_record_ctrl = new CDlgRecordCtrl(&m_tab);
	m_dlg_ipc_record_ctrl->Create(IDD_DLG_RECORD_CTRL,&m_tab);

	return TRUE;
}

void CDlgRemoteSet::clean()
{
	if(m_dlg_osd)
	{
		//m_dlg_osd->OnClose();
		m_dlg_osd->close();
		delete m_dlg_osd;
		m_dlg_osd = NULL;
	}

	if(m_dlg_quality)
	{
		//m_dlg_quality->OnClose();
		m_dlg_quality->close();
		delete m_dlg_quality;
		m_dlg_quality = NULL;
	}

	if(m_dlg_alarm)
	{
		m_dlg_alarm->close();
		delete m_dlg_alarm;
		m_dlg_alarm = NULL;
	}

	if(m_dlg_mask_alarm)
	{
		m_dlg_mask_alarm->close();
		delete m_dlg_mask_alarm;
		m_dlg_mask_alarm = NULL;
	}

	if(m_dlg_work_sheet)
	{
		m_dlg_work_sheet->close();
		delete m_dlg_work_sheet;
		m_dlg_work_sheet = NULL;
	}

	if(m_dlg_motion)
	{
		m_dlg_motion->close();
		delete m_dlg_motion;
		m_dlg_motion = NULL;
	}

	if(m_dlg_net)
	{
		m_dlg_net->close();
		delete m_dlg_net;
		m_dlg_net = NULL;
	}

	if(m_dlg_serial_port)
	{
		m_dlg_serial_port->close();
		delete m_dlg_serial_port;
		m_dlg_serial_port = NULL;
	}

	if(m_dlg_ptz)
	{
		m_dlg_ptz->close();
		delete m_dlg_ptz;
		m_dlg_ptz = NULL;
	}

	if(m_dlg_log)
	{
		m_dlg_log->close();
		delete m_dlg_log;
		m_dlg_log = NULL;
	}

	if(m_dlg_remote_do)
	{
		m_dlg_remote_do->close();
		delete m_dlg_remote_do;
		m_dlg_remote_do = NULL;
	}

	if(m_dlg_channel_set)
	{
		m_dlg_channel_set->close();
		delete m_dlg_channel_set;
		m_dlg_channel_set = NULL;
	}

	if(m_dlg_ipc_feature)
	{
		m_dlg_ipc_feature->close();
		delete m_dlg_ipc_feature;
		m_dlg_ipc_feature = NULL;
	}

	if(m_dlg_sub_quality)
	{
		m_dlg_sub_quality->close();
		delete m_dlg_sub_quality;
		m_dlg_sub_quality = NULL;
	}

	if(m_dlg_vlost)
	{
		m_dlg_vlost->close();
		delete m_dlg_vlost;
		m_dlg_vlost = NULL;
	}

	if(m_dlg_deviceinfo)
	{
		m_dlg_deviceinfo->close();
		delete m_dlg_deviceinfo;
		m_dlg_deviceinfo = NULL;
	}

	if(m_dlg_alarmout)
	{
		m_dlg_alarmout->close();
		delete m_dlg_alarmout;
		m_dlg_alarmout = NULL;
	}

	if(m_dlg_mask)
	{
		m_dlg_mask->close();
		delete m_dlg_mask;
		m_dlg_mask = NULL;
	}

	if(m_dlg_analyze)
	{
		m_dlg_analyze->close();
		delete m_dlg_analyze;
		m_dlg_analyze = NULL;
	}

	if(m_dlg_custom_osd)
	{
		m_dlg_custom_osd->close();
		delete m_dlg_custom_osd;
		m_dlg_custom_osd = NULL;
	}

	if(m_dlg_ipc_record_ctrl)
	{
		delete m_dlg_ipc_record_ctrl;
		m_dlg_ipc_record_ctrl = NULL;
	}
}

void CDlgRemoteSet::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	clean();

	CDialog::OnClose();
}

void CDlgRemoteSet::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	clean();

	OnCancel();
}

void CDlgRemoteSet::OnTcnSelchangeTabSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	if(m_dlg_osd)
	{
		m_dlg_osd->ShowWindow(SW_HIDE);		
		m_dlg_osd->close_video();
	}
	
	if(m_dlg_quality)
	{
		m_dlg_quality->ShowWindow(SW_HIDE);	
	}
	
	if(m_dlg_alarm)
	{
		m_dlg_alarm->ShowWindow(SW_HIDE);
	}
	
	if(m_dlg_mask_alarm)
	{
		m_dlg_mask_alarm->ShowWindow(SW_HIDE);
	}
	
	if(m_dlg_work_sheet)
	{
		m_dlg_work_sheet->ShowWindow(SW_HIDE);
	}
	
	if(m_dlg_motion)
	{
		m_dlg_motion->ShowWindow(SW_HIDE);
		m_dlg_motion->close_video();
	}
	
	if(m_dlg_net)
	{
		m_dlg_net->ShowWindow(SW_HIDE);
	}
	
	if(m_dlg_serial_port)
	{
		m_dlg_serial_port->ShowWindow(SW_HIDE);
	}
	
	if(m_dlg_ptz)
	{
		m_dlg_ptz->ShowWindow(SW_HIDE);
	}
	
	if(m_dlg_log)
	{
		m_dlg_log->ShowWindow(SW_HIDE);
	}
	
	if(m_dlg_remote_do)
	{
		m_dlg_remote_do->ShowWindow(SW_HIDE);
	}

	if(m_dlg_channel_set)
	{
		m_dlg_channel_set->ShowWindow(SW_HIDE);
	}

	if(m_dlg_ipc_feature)
	{
		m_dlg_ipc_feature->ShowWindow(SW_HIDE);
		m_dlg_ipc_feature->close_video();
	}

	if(m_dlg_sub_quality)
	{
		m_dlg_sub_quality->ShowWindow(SW_HIDE);
	}

	if(m_dlg_vlost)
	{
		m_dlg_vlost->ShowWindow(SW_HIDE);
	}

	if(m_dlg_deviceinfo)
	{
		m_dlg_deviceinfo->ShowWindow(SW_HIDE);
	}

	if(m_dlg_alarmout)
	{
		m_dlg_alarmout->ShowWindow(SW_HIDE);
	}

	if(m_dlg_mask)
	{
		m_dlg_mask->ShowWindow(SW_HIDE);
	}

	if(m_dlg_analyze)
	{
		m_dlg_analyze->ShowWindow(SW_HIDE);
	}

	if(m_dlg_custom_osd)
	{
		m_dlg_custom_osd->ShowWindow(SW_HIDE);
	}

	if(m_dlg_ipc_record_ctrl)
	{
		m_dlg_ipc_record_ctrl->ShowWindow(SW_HIDE);
	}
	
	switch(m_tab.GetCurSel())
	{
	case 0:
		{
			//osd			
			m_dlg_osd->ShowWindow(SW_NORMAL);	
			m_dlg_osd->connect_video();
			break;
		}
	case 1:
		{
			//ͼ������			
			m_dlg_quality->ShowWindow(SW_NORMAL);			
			break;
		}

	case 2:
		{
			//����
			m_dlg_net->ShowWindow(SW_NORMAL);
			m_dlg_net->refresh();
			break;
		}

	case 3:
		{
			//Զ�̲���
			m_dlg_remote_do->ShowWindow(SW_NORMAL);
			break;
		}

	case 4:
		{
			//ipc feature
			m_dlg_ipc_feature->ShowWindow(SW_NORMAL);
			break;
		}

	case 5:
		{
			//����
			m_dlg_serial_port->ShowWindow(SW_NORMAL);
			break;;
		}

	case 6:
		{
			//PTZ
			m_dlg_ptz->ShowWindow(SW_NORMAL);
			break;;
		}

	case 7:
		{
			//�ƶ����
			m_dlg_motion->ShowWindow(SW_NORMAL);
			m_dlg_motion->connect_video();
			break;
		}	

	case 8:
		{
			//����������
			m_dlg_sub_quality->ShowWindow(SW_NORMAL);
			break;
		}

	case 9:
		{
			//����������
			m_dlg_alarm->ShowWindow(SW_NORMAL);
			break;
		}

	case 10:
		{
			//��־����
			m_dlg_log->ShowWindow(SW_NORMAL);
			break;
		}

	case 11://device info
		{
			m_dlg_deviceinfo->ShowWindow(SW_NORMAL);
			m_dlg_deviceinfo->refresh();
			break;
		}

	case 12:
		{
			m_dlg_alarmout->ShowWindow(SW_NORMAL);
			//�̵������
			break;
		}

	case 13:
		{
			//������
			m_dlg_work_sheet->ShowWindow(SW_NORMAL);
			break;
		}

	case 14:
		{
			//ͨ������
			m_dlg_channel_set->ShowWindow(SW_NORMAL);
			break;
		}

	case 15:
		{
			//��Ƶ��ʧ
			m_dlg_vlost->ShowWindow(SW_NORMAL);
			break;
		}

	case 16:
		{
			//mask
			m_dlg_mask->ShowWindow(SW_NORMAL);
			break;
		}

	case 17:
		{
			//��Ƶ����
			m_dlg_analyze->ShowWindow(SW_NORMAL);
			break;
		}
	
	case 18:
		{
			//�Զ���OSD
			m_dlg_custom_osd->ShowWindow(SW_NORMAL);
			break;
		}

	case 19:
		{
			//¼�����
			m_dlg_ipc_record_ctrl->ShowWindow(SW_NORMAL);
			break;
		}
	}

	*pResult = 0;
}
