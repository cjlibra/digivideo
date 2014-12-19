#pragma once


// CPassDlg 对话框

class CPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPassDlg();

// 对话框数据
	enum { IDD = IDD_PASS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString password;
	afx_msg void OnBnClickedOk();
	CString m_passwd;
};
