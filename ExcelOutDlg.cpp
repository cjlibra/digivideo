// ExcelOutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "ExcelOutDlg.h"
#include "afxdialogex.h"


#include "sqlite3.h"
#include "AMainDlg.h"

static int _searchvideo0_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _searchvideo1_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _searchlabelnum_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _searchren_callback(void * notused, int argc, char ** argv, char ** szColName);
typedef struct _PERSON{
	CString name;
	CString labelid;
	CString labelnum;
	CString earlytime;
	CString latetime;

}PERSON;

typedef struct _LABELID{
	CString id;
	CString num;
}LABELID;
static PERSON person[1024];
static LABELID labelid[2048];
static int personcount=0 ;
static int labelcount=0 ;

static int personpos=-1 ;
// CExcelOutDlg 对话框

IMPLEMENT_DYNAMIC(CExcelOutDlg, CDialogEx)

CExcelOutDlg::CExcelOutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExcelOutDlg::IDD, pParent)
	, m_datetime(COleDateTime::GetCurrentTime())
{

}

CExcelOutDlg::~CExcelOutDlg()
{
}

void CExcelOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_datetime);
}


BEGIN_MESSAGE_MAP(CExcelOutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CExcelOutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CExcelOutDlg 消息处理程序


void CExcelOutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	
	
	for (int i=0;i<1024;i++){
		person[i].name="";
		person[i].labelid="";
		person[i].labelnum="";
		person[i].earlytime="";
		person[i].latetime="";

	}
	personcount=0 ;
	labelcount=0 ;
	CAMainDlg *pWin = (CAMainDlg *) this->GetParent();
	char sSQL6[255] = " select name,lableid from person  ;";
	char * pErrMsg = 0;	 
	sqlite3_exec( pWin->db, sSQL6, _searchren_callback, 0, &pErrMsg);

	
	sprintf(sSQL6,"select id , num from label  ;"   );
	sqlite3_exec( pWin->db, sSQL6, _searchlabelnum_callback, 0, &pErrMsg);
	
	for (int i=0;i<personcount;i++){
		for (int j =0;j<labelcount;j++){
			if (labelid[j].id == person[i].labelid){
				person[i].labelnum = labelid[j].num;
				break;

			}
			continue;
		}

	}

	UpdateData(TRUE);
	CString daychose ;
	daychose = m_datetime.Format("%Y-%m-%d");
	for (int i=0;i<personcount;i++){
		personpos = i;
		sprintf(sSQL6 , " select lablenum,starttime from vinfo  where lablenum='%s' and starttime > '%s' and  starttime < '%s' order by id  desc limit 1;" , person[i].labelnum , daychose+" 00:00:00",daychose+" 23:59:59" );
		
		sqlite3_exec( pWin->db, sSQL6, _searchvideo0_callback, 0, &pErrMsg);

	}
	personpos = -1;

	for (int i=0;i<personcount;i++){
		personpos = i;
		sprintf(sSQL6 , " select lablenum,starttime from vinfo  where lablenum='%s' and starttime > '%s' and  starttime < '%s'  order by id  asc limit 1;" , person[i].labelnum , daychose+" 00:00:00",daychose+" 23:59:59" );
		
		sqlite3_exec( pWin->db, sSQL6, _searchvideo1_callback, 0, &pErrMsg);

	}
	personpos = -1;
}
static int _searchvideo0_callback(void * notused, int argc, char ** argv, char ** szColName)
{

	person[personpos].latetime = argv[1];
	return 0;
}

static int _searchvideo1_callback(void * notused, int argc, char ** argv, char ** szColName)
{


	person[personpos].earlytime = argv[1];
	return 0;
}
static int _searchlabelnum_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	labelid[labelcount].id=argv[0];
	labelid[labelcount].num=argv[1];
	labelcount++;
	return 0;
}
static int _searchren_callback(void * notused, int argc, char ** argv, char ** szColName)
{

	person[personcount].name = argv[0];
	person[personcount].labelid=argv[1];
	personcount++;
	return 0;

}