// DlgVcatch.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgVcatch.h"
#include "afxdialogex.h"

CDlgVcatch  *thisp;

// CDlgVcatch 对话框

IMPLEMENT_DYNAMIC(CDlgVcatch, CDialogEx)

CDlgVcatch::CDlgVcatch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVcatch::IDD, pParent)
{

}

CDlgVcatch::~CDlgVcatch()
{
}

BOOL CDlgVcatch::OnInitDialog()
{
	CDialog::OnInitDialog();
	DWORD dwStyle;
	dwStyle =m_listvideo.GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT| LVS_SHOWSELALWAYS;
	m_listvideo.SetExtendedStyle(dwStyle);
	thisp = this;
	m_listvideo.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	m_listvideo.InsertColumn(1,"标签号",LVCFMT_CENTER,200);
	m_listvideo.InsertColumn(2,"起始时间",LVCFMT_CENTER,200);
	m_listvideo.InsertColumn(3,"终止时间",LVCFMT_CENTER,200);
	m_listvideo.InsertColumn(4,"视频名",LVCFMT_CENTER,50);
	return TRUE;
	
}

void CDlgVcatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listvideo);
}



BEGIN_MESSAGE_MAP(CDlgVcatch, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgVcatch::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgVcatch 消息处理程序


void CDlgVcatch::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}
