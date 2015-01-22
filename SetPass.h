#pragma once


// CSetPass 对话框

class CSetPass : public CDialogEx
{
	DECLARE_DYNAMIC(CSetPass)

public:
	CSetPass(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetPass();

// 对话框数据
	enum { IDD = IDD_A_SETPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_oldpass;
	CString m_newpass;
	CString m_newpass1;
};
