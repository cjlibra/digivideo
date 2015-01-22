// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "net_demo.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

#include "AMainDlg.h"

static CString username_db;
static CString password_db;
static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName);
// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
{
	goingnow = 0;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT2, m_password);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������


void CLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	   const char * sSQL3 = "select * from user;";
	   UpdateData(TRUE);	 
	   CAMainDlg *pAMain = (CAMainDlg *) this->GetParent();
	   char * pErrMsg = 0;
	   sqlite3_exec( pAMain->db, sSQL3, _sql_callback, 0, &pErrMsg);

	 
	   if (m_password == password_db && m_username == username_db){
		   pAMain->ShowWindow(SW_MAXIMIZE); 
		   CDialogEx::OnOK();
		   return;

	   }
	   AfxMessageBox("���벻��");
 }

 static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName)
{
      username_db = CString(argv[1]);
      password_db = CString(argv[2]);
      return 0;
 }

 void CLoginDlg::OnBnClickedCancel()
 {
	 if (goingnow == 0)	{

		 CDialogEx::OnCancel();
		 CAMainDlg *pAMain = (CAMainDlg *) this->GetParent();
		 pAMain->OnExit();
		 return;
	 }
	 AfxMessageBox("�����������������������˳������ȵ�½");
 }
