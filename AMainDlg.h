#pragma once
#include "afxcmn.h"

#include "sqlite3.h"
#include "net_video_test.h"

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
	afx_msg void On32778();
	static void  GetRfidPic(CString str,void *pptest);
	static void CALLBACK draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser);
	SYSTEMTIME stLocal;  
	TCHAR chBuf[256];
	int startvideocount;
	int whichoneclick;
	int setcustom;
	int goingnow;

	void SwitchView(int percount);
	void InitViewItemInfo();
	int SearchViewItemIdle();
	afx_msg void On32780();
	afx_msg void On32777();
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32781();
	afx_msg void On32783();
	afx_msg void On32784();
	afx_msg void On32785();
	afx_msg void On32786();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void On32787();
	void  OutInitFile();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32792();
	afx_msg void On32793();
	afx_msg void On32794();
	afx_msg void On32795();
	afx_msg void On32788();
	afx_msg void On32789();
	afx_msg void On32790();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void On32988();
	afx_msg void On32989();
	afx_msg void On32990();
	afx_msg void On327192();
	afx_msg void On327193();
	afx_msg void On327194();
	afx_msg void On327195();
	afx_msg void On32805();
	afx_msg void On32776();
	void OnExit(void);
	afx_msg void On32806();
};
