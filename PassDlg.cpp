// PassDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "PassDlg.h"
#include "afxdialogex.h"


// CPassDlg 对话框

IMPLEMENT_DYNAMIC(CPassDlg, CDialogEx)

CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassDlg::IDD, pParent)
	, m_passwd(_T(""))
{

}

CPassDlg::~CPassDlg()
{
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_passwd);
}


BEGIN_MESSAGE_MAP(CPassDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPassDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassDlg 消息处理程序


void CPassDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_passwd == password){
	  CDialogEx::OnOK();
	}else{
      AfxMessageBox("密码不对");
	  CDialogEx::OnCancel();
	}
}
