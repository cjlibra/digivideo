#pragma once
#include "afxwin.h"

#include "sqlite3.h"



// CKaoqing 对话框

class CKaoqing : public CDialogEx
{
	DECLARE_DYNAMIC(CKaoqing)

public:
	CKaoqing(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKaoqing();

// 对话框数据
	enum { IDD = IDD_DLG_KAOQING };
	CIPAddressCtrl 	m_ip_kaoqing;
	void  Connectvideo();
	static void  GetRfidPic(CString str);
	void  OnButtonSelectiamge()  ;
	bool  LoadShowBMPFile(const char *pPathname) ;
	bool  Show_picture(CString imgPath);
	HBITMAP  BufferToHBITMAP() ;
	void  DrawImage(int x, int y, CDC *pDC) ;
	CString strPathName;
	HBITMAP m_hBitmap;
	char *m_pBMPBuffer;

	LONG m_bmW ;
	LONG m_bmH;

	sqlite3 * db;
	
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtloginKaoqing();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	 
	CStatic m_PictureControl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	CEdit m_username;
	CEdit m_password;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton3();
	CComboBox m_combochanctrl;
	afx_msg void OnCbnSelchangeCombo1();
	TCHAR chBuf[256];
	
	SYSTEMTIME stLocal;  
    
};
