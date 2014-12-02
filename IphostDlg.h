#pragma once
#include "afxcmn.h"


// CIphostDlg �Ի���

class CIphostDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIphostDlg)

public:
	CIphostDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIphostDlg();

// �Ի�������
	enum { IDD = IDD_IPHOST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	CIPAddressCtrl m_iphostctrl;
	CString m_cameraid;
	CString m_cardreaderid;
	CString m_comments;
	CTreeCtrl m_iphosttreectrl;
	afx_msg void OnBnClickedButton1();
	HTREEITEM hItem ;
	void  RefreshTree();
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
};
