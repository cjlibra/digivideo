// AMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "AMainDlg.h"
#include "afxdialogex.h"

#include "IphostDlg.h"
#include "DlgVcatch.h"
#include "RecordDlg.h"

CAMainDlg *thisp;
// CAMainDlg 对话框

IMPLEMENT_DYNAMIC(CAMainDlg, CDialogEx)

CAMainDlg::CAMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAMainDlg::IDD, pParent)
{
	thisp = this;
}

CAMainDlg::~CAMainDlg()
{
}

void CAMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_mainiptree);
}


BEGIN_MESSAGE_MAP(CAMainDlg, CDialogEx)
	ON_COMMAND(ID_32773, &CAMainDlg::On32773)
	ON_COMMAND(ID_Menu, &CAMainDlg::OnMenu)
	ON_COMMAND(ID_32775, &CAMainDlg::On32775)
END_MESSAGE_MAP()


// CAMainDlg 消息处理程序
static int _sql_select_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	CString tmp = argv[1];
	thisp->m_mainiptree.InsertItem(argv[1],NULL,NULL,thisp->hItem );
//	jj++;
    return 0;
}

void CAMainDlg::RefreshTree()
{

	 this->m_mainiptree.DeleteAllItems();
	 hItem = this->m_mainiptree.InsertItem("摄像机组",NULL,NULL,NULL);

	 const char * sSQL4 = "select * from hosts";
	 
	 char *pErrMsg = 0;
	 sqlite3_exec( db, sSQL4,_sql_select_callback, 0, &pErrMsg);

	 this->m_mainiptree.Expand(hItem,TVE_EXPAND);


}
BOOL CAMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int ret = 1;
	db = 0;
	ret = sqlite3_open("medb.s3db", &db);
      
    if ( ret != SQLITE_OK )
    {
      fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
      return(1);
    }
      
    printf("数据库连接成功!\n");

	ShowWindow(SW_MAXIMIZE);
	RECT clRect;
	GetClientRect(&clRect);
	RECT treeRect;
	treeRect.top = clRect.top+20;
	treeRect.left = clRect.left+20;
	treeRect.right = (clRect.right-clRect.left)/4+treeRect.left-100;
	treeRect.bottom = clRect.bottom-20;
	m_mainiptree.MoveWindow(&treeRect);
	CWnd * m_pviewItem1 = GetDlgItem(IDC_STATIC1);
	CWnd * m_pviewItem2 = GetDlgItem(IDC_STATIC2);
	CWnd * m_pviewItem3 = GetDlgItem(IDC_STATIC3);
	CWnd * m_pviewItem4 = GetDlgItem(IDC_STATIC4);
	CWnd * m_pviewItem5 = GetDlgItem(IDC_STATIC5);
	CWnd * m_pviewItem6 = GetDlgItem(IDC_STATIC6);
	CWnd * m_pviewItem7 = GetDlgItem(IDC_STATIC7);
	CWnd * m_pviewItem8 = GetDlgItem(IDC_STATIC8);
	CWnd * m_pviewItem9 = GetDlgItem(IDC_STATIC9);

	int viewitemheight = (clRect.bottom-clRect.top-20-20-5-5)/3;
	int  viewitemwidth = (clRect.right-treeRect.right-20-20-5-5)/3;

	RECT viewRect;
	viewRect.top = treeRect.top;
	viewRect.left = treeRect.right+20;
	viewRect.bottom = viewRect.top + viewitemheight;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem1->MoveWindow(&viewRect);
	viewRect.left = viewRect.right+5;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem2->MoveWindow(&viewRect);
	viewRect.left = viewRect.right+5;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem3->MoveWindow(&viewRect);
	viewRect.top = treeRect.top+viewitemheight+5;
	viewRect.left = treeRect.right+20;
	viewRect.bottom = viewRect.top + viewitemheight;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem4->MoveWindow(&viewRect);
	viewRect.left = viewRect.right+5;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem5->MoveWindow(&viewRect);
	viewRect.left = viewRect.right+5;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem6->MoveWindow(&viewRect);
	viewRect.top = treeRect.top+(viewitemheight+5)*2;
	viewRect.left = treeRect.right+20;
	viewRect.bottom = viewRect.top + viewitemheight;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem7->MoveWindow(&viewRect);
	viewRect.left = viewRect.right+5;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem8->MoveWindow(&viewRect);
	viewRect.left = viewRect.right+5;
	viewRect.right = viewRect.left + viewitemwidth;
	m_pviewItem9->MoveWindow(&viewRect);

	RefreshTree(); //显示IPtree

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAMainDlg::On32773() //设备配置，添加摄像机ip属性
{
	CIphostDlg dlg;
	dlg.DoModal();

}


void CAMainDlg::OnMenu() //视频搜索
{
       CDlgVcatch  dlg;
	   dlg.DoModal();
}


void CAMainDlg::On32775() //图像搜索
{
	
	CRecordDlg  dlg;
	dlg.DoModal();
	  
}
