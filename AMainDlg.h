#pragma once
#include "afxcmn.h"

#include "sqlite3.h"


// CAMainDlg 对话框

class CAMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAMainDlg)

public:
	CAMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAMainDlg();

// 对话框数据
	enum { IDD = IDD_A_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
