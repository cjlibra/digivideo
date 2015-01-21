// WebWeb.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "net_demo.h"
#include "WebWeb.h"
#include "afxdialogex.h"


// CWebWeb �Ի���

IMPLEMENT_DYNAMIC(CWebWeb, CDialogEx)

CWebWeb::CWebWeb(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWebWeb::IDD, pParent)
{

}

CWebWeb::~CWebWeb()
{
}

void CWebWeb::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_webpage);
}


BEGIN_MESSAGE_MAP(CWebWeb, CDialogEx)
END_MESSAGE_MAP()


// CWebWeb ��Ϣ�������


BOOL CWebWeb::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowWindow(SW_SHOWMAXIMIZED);
	RECT rect;
	GetClientRect(&rect);
	m_webpage.MoveWindow(&rect);	
	m_webpage.Navigate("http://"+url , NULL, NULL, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
