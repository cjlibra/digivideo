// PassDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "net_demo.h"
#include "PassDlg.h"
#include "afxdialogex.h"


// CPassDlg �Ի���

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


// CPassDlg ��Ϣ�������


void CPassDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_passwd == password){
	  CDialogEx::OnOK();
	}else{
      AfxMessageBox("���벻��");
	  CDialogEx::OnCancel();
	}
}
