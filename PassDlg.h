#pragma once


// CPassDlg �Ի���

class CPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassDlg();

// �Ի�������
	enum { IDD = IDD_PASS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString password;
	afx_msg void OnBnClickedOk();
	CString m_passwd;
};
