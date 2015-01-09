#pragma once
#include "afxcmn.h"

#include "sqlite3.h"


// CAMainDlg �Ի���

class CAMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAMainDlg)

public:
	CAMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAMainDlg();

// �Ի�������
	enum { IDD = IDD_A_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_mainiptree;
	void  On32773();
	sqlite3 * db;
	HTREEITEM hItem ;
	void  RefreshTree();
	
	afx_msg void OnMenu();
	afx_msg void On32775();
};
