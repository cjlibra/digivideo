#pragma once
#include "atlcomtime.h"


// CExcelOutDlg �Ի���

class CExcelOutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExcelOutDlg)

public:
	CExcelOutDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExcelOutDlg();

// �Ի�������
	enum { IDD = IDD_A_EXCELOUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	COleDateTime m_datetime;
};
