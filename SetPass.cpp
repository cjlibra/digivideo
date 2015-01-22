// SetPass.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "SetPass.h"
#include "afxdialogex.h"

#include "AMainDlg.h"

static int _sql_getoldpass_callback(void * notused, int argc, char ** argv, char ** szColName);
static CString password_db;
// CSetPass 对话框

IMPLEMENT_DYNAMIC(CSetPass, CDialogEx)

CSetPass::CSetPass(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetPass::IDD, pParent)
	, m_oldpass(_T(""))
	, m_newpass(_T(""))
	, m_newpass1(_T(""))
{

}

CSetPass::~CSetPass()
{
}

void CSetPass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_oldpass);
	DDX_Text(pDX, IDC_EDIT2, m_newpass);
	DDX_Text(pDX, IDC_EDIT3, m_newpass1);
}


BEGIN_MESSAGE_MAP(CSetPass, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetPass::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetPass 消息处理程序


void CSetPass::OnBnClickedOk()
{
	 UpdateData(TRUE);
	 if (m_newpass != m_newpass1){
		 AfxMessageBox("新密码输入有误");
		 return;
	 }
	 int ret = -1;
	 char *pErrMsg = 0;
	 CAMainDlg *pAMain = (CAMainDlg *)GetParent();
	 const char * sSQL3 = "select * from user; ";
	 ret = sqlite3_exec( pAMain->db, sSQL3,_sql_getoldpass_callback, 0, &pErrMsg);
	 if (m_oldpass != password_db){
		 AfxMessageBox("旧密码输入有误");
		 return;
	 }
	 const char * sSQL4 = "update user set password='%s' where password = '%s' ; ";
	 char sSQL5[255];
	 sprintf(sSQL5,sSQL4,m_newpass,m_oldpass);
	 
	 ret = sqlite3_exec( pAMain->db, sSQL5,NULL, 0, &pErrMsg);
	 if (ret == 0) {
		 AfxMessageBox("密码修改成功");
		 CDialogEx::OnOK();
	 }else{
		 AfxMessageBox("密码修改出错");
		 return;
	 }


	 return;
	
}


static int _sql_getoldpass_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	 
	password_db = argv[2];
    return 0;
}