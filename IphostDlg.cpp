// IphostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "IphostDlg.h"
#include "afxdialogex.h"


#include "Kaoqing.h"

// CIphostDlg 对话框
static int _sql_select_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _sql_select1_callback(void * notused, int argc, char ** argv, char ** szColName);
CIphostDlg *thisp;
static int jj;
IMPLEMENT_DYNAMIC(CIphostDlg, CDialogEx)

CIphostDlg::CIphostDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIphostDlg::IDD, pParent)
	, m_cameraid(_T(""))
	, m_cardreaderid(_T(""))
	, m_comments(_T(""))
{
	thisp = this;
}

CIphostDlg::~CIphostDlg()
{
}

void CIphostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_iphostctrl);
	DDX_Text(pDX, IDC_EDIT1, m_cameraid);
	DDX_Text(pDX, IDC_EDIT2, m_cardreaderid);
	DDX_Text(pDX, IDC_EDIT3, m_comments);
	DDX_Control(pDX, IDC_TREE1, m_iphosttreectrl);
}

BOOL CIphostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	RefreshTree();




	return TRUE;


}

BEGIN_MESSAGE_MAP(CIphostDlg, CDialogEx)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CIphostDlg::OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BUTTON1, &CIphostDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CIphostDlg::OnNMClickTree1)
	ON_BN_CLICKED(IDC_BUTTON3, &CIphostDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CIphostDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CIphostDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CIphostDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CIphostDlg 消息处理程序


void CIphostDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
void CIphostDlg::RefreshTree()
{

	 this->m_iphosttreectrl.DeleteAllItems();
	 hItem = this->m_iphosttreectrl.InsertItem("摄像机组",NULL,NULL,NULL);

	 const char * sSQL4 = "select * from hosts";
	  CKaoqing *p=(CKaoqing *)this->GetParent();
	  char *pErrMsg = 0;
	 sqlite3_exec(p->db, sSQL4,_sql_select_callback, 0, &pErrMsg);

	 this->m_iphosttreectrl.Expand(hItem,TVE_EXPAND);


}

void CIphostDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	 
	 UpdateData(TRUE);

	 const char * sSQL3 = "insert into hosts (ip,cameraid,cardreaderid,comments) values('%s','%s','%s','%s');";
	 char sSQL4[255];
	 CString iptxt;
	 m_iphostctrl.GetWindowText(iptxt);
	 if ( iptxt == "0.0.0.0") {
		 return;

	 }
	 sprintf(sSQL4,sSQL3,iptxt,m_cameraid,m_cardreaderid,m_comments);
	 CKaoqing *p=(CKaoqing *)this->GetParent();

	 char * pErrMsg = 0;
	 int ret =1;
	 sqlite3_exec(p->db, sSQL4, NULL, 0, &pErrMsg);
	 if ( ret == SQLITE_OK ){
	     this->m_iphosttreectrl.InsertItem(iptxt,NULL,NULL,hItem);
	 }
	 jj = 0;
	 this->RefreshTree();
}


static int _sql_select_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	CString tmp = argv[1];
	thisp->m_iphosttreectrl.InsertItem(argv[1],NULL,NULL,thisp->hItem );
	jj++;
    return 0;
}
static int _sql_select1_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	thisp->m_iphostctrl.SetWindowText(argv[1]);
	thisp->m_cameraid = argv[2];
	thisp->m_cardreaderid=argv[3];;
	thisp->m_comments=argv[4];;
	thisp->UpdateData(FALSE);
    return 0;
}
void CIphostDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint   pt;     
    UINT   nFlags;     
    ::GetCursorPos(&pt);     
    ::ScreenToClient(thisp->m_iphosttreectrl.m_hWnd, &pt);     
    HTREEITEM   hItem   =   thisp->m_iphosttreectrl.HitTest(pt,&nFlags);     
	//HTREEITEM hItem=thisp->m_iphosttreectrl.GetSelectedItem();  
	CString str = thisp->m_iphosttreectrl.GetItemText(hItem);
// str = thisp->m_iphosttreectrl.GetItemText(hItem );
	const char * sSQL4 = "select * from hosts where ip = '%s';";
	char sSQL5[255];
	sprintf(sSQL5,sSQL4,str);
	CKaoqing *p=(CKaoqing *)this->GetParent();
	char *pErrMsg = 0;
    sqlite3_exec(p->db, sSQL5,_sql_select1_callback, 0, &pErrMsg);

	*pResult = 0;
}


void CIphostDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	HTREEITEM  hItem = thisp->m_iphosttreectrl.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("请选择ip");
		return;
	}
	CString tmp = thisp->m_iphosttreectrl.GetItemText(hItem);
	if (tmp == "摄像机组") {

		AfxMessageBox("请选择ip");
		return;
	}
	 UpdateData(TRUE);
	 const char * sSQL3 = "update hosts  set ip='%s',cameraid='%s',cardreaderid='%s',comments='%s' where ip = '%s' ;";
	 char sSQL4[255];
	 CString iptxt;
	 m_iphostctrl.GetWindowText(iptxt);
	 sprintf(sSQL4,sSQL3,iptxt,m_cameraid,m_cardreaderid,m_comments,iptxt);
	 CKaoqing *p=(CKaoqing *)this->GetParent();

	 char * pErrMsg = 0;
	 int ret =1;
	 sqlite3_exec(p->db, sSQL4, NULL, 0, &pErrMsg);
	  
	 this->RefreshTree();

}


void CIphostDlg::OnBnClickedButton4()
{
	 //UpdateData(TRUE);
	HTREEITEM  hItem = thisp->m_iphosttreectrl.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("请选择ip");
		return;
	}
	CString tmp = thisp->m_iphosttreectrl.GetItemText(hItem);
	if (tmp == "摄像机组") {

		AfxMessageBox("请选择ip");
		return;
	}
	 const char * sSQL3 = "delete from  hosts  where ip = '%s' ;";
	 char sSQL4[255];
	 CString iptxt;
	 m_iphostctrl.GetWindowText(iptxt);
	 sprintf(sSQL4,sSQL3, iptxt);
	 CKaoqing *p=(CKaoqing *)this->GetParent();

	 char * pErrMsg = 0;
	 int ret =1;
	 sqlite3_exec(p->db, sSQL4, NULL, 0, &pErrMsg);
	  
	 this->RefreshTree();
}


void CIphostDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CIphostDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM  hItem = thisp->m_iphosttreectrl.GetSelectedItem();
	if (hItem == NULL){
		AfxMessageBox("请选择ip");
		return;
	}
	CString tmp = thisp->m_iphosttreectrl.GetItemText(hItem);
	if (tmp == "摄像机组") {

		AfxMessageBox("请选择ip");
		return;
	}
	CKaoqing *p =(CKaoqing *)this->GetParent();
	p->SetDlgItemText(IDC_IPADDRESS_KAOQING,tmp);
	this->OnCancel();
	p->OnBnClickedBtloginKaoqing();

	
	 
}
