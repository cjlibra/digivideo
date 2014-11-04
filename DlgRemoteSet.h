#pragma once

#include "DlgOsd.h"
#include "DlgQuality.h"
#include "DlgAlarm.h"
#include "DlgMaskAlarm.h"
#include "DlgWorkSheet.h"
#include "DlgMotion.h"
#include "DlgNet.h"
#include "DlgSerialPort.h"
#include "DlgPtz.h"
#include "DlgLog.h"
#include "DlgRemoteDo.h"
#include "DlgChannelSet.h"
#include "DlgIPCFeature.h"
#include "DlgSubQuality.h"
#include "DlgVlost.h"
#include "DlgDeviceInfo.h"
#include "DlgMask.h"
#include "AlarmOut.h"
#include "DlgAnalyze.h"
#include "DlgCustomOsd.h"
#include "DlgRecordCtrl.h"

#include "afxcmn.h"

// CDlgRemoteSet 对话框

class CDlgRemoteSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgRemoteSet)

public:
	CDlgRemoteSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRemoteSet();

// 对话框数据
	//{{AFX_DATA(CDlgRemoteSet)
	enum { IDD = IDD_DLG_SET };

	CTabCtrl m_tab;
	//}}AFX_DATA
	
	CDlgOsd* m_dlg_osd;
	CDlgQuality* m_dlg_quality;
	CDlgAlarm* m_dlg_alarm;
	CDlgMaskAlarm* m_dlg_mask_alarm;
	CDlgWorkSheet* m_dlg_work_sheet;
	CDlgMotion* m_dlg_motion;
	CDlgNet* m_dlg_net;
	CDlgSerialPort* m_dlg_serial_port;
	CDlgPtz* m_dlg_ptz;
	CDlgLog* m_dlg_log;
	CDlgRemoteDo* m_dlg_remote_do;
	CDlgChannelSet* m_dlg_channel_set;
	CDlgIPCFeature* m_dlg_ipc_feature;
	CDlgSubQuality* m_dlg_sub_quality;
	CDlgVlost* m_dlg_vlost;
	CDlgDeviceInfo* m_dlg_deviceinfo;
	AlarmOut* m_dlg_alarmout;
	CDlgMask* m_dlg_mask;
	CDlgAnalyze* m_dlg_analyze;
	CDlgCustomOsd* m_dlg_custom_osd;
	CDlgRecordCtrl* m_dlg_ipc_record_ctrl;

	void clean();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgRemoteSet)
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();	
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTcnSelchangeTabSet(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRemoteSet)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
};
