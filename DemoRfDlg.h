#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDemoRfDlg �Ի���

class CDemoRfDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDemoRfDlg)

public:
	CDemoRfDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDemoRfDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtloginDemorf();
	CIPAddressCtrl m_ip_str;

	void Connectvideo();
	bool Show_picture(CString imgPath);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1Demorf();
	CStatic m_picctrl;
	afx_msg void OnBnClickedBtloginRf();
	CIPAddressCtrl m_IP_RFREADER;
	afx_msg void OnBnClickedButUncon();
};
