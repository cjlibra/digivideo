#pragma once


// CSetPass �Ի���

class CSetPass : public CDialogEx
{
	DECLARE_DYNAMIC(CSetPass)

public:
	CSetPass(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetPass();

// �Ի�������
	enum { IDD = IDD_A_SETPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_oldpass;
	CString m_newpass;
	CString m_newpass1;
};
