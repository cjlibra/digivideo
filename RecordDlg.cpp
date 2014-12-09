// RecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "RecordDlg.h"
#include "afxdialogex.h"

#include "Kaoqing.h"

static int _searchren_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _searchlabelnum_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _searchrecord_callback(void * notused, int argc, char ** argv, char ** szColName);
static int idfromname;
static CString labelnumfromid;
static int jj;
CRecordDlg *thisp;

// CRecordDlg 对话框

IMPLEMENT_DYNAMIC(CRecordDlg, CDialogEx)

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordDlg::IDD, pParent)
	, m_nameren(_T(""))
	, m_datestring(COleDateTime::GetCurrentTime())
{

}

CRecordDlg::~CRecordDlg()
{
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nameren);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_datestring);
	DDX_Control(pDX, IDC_LIST1, m_listrecord);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datecrl);
	DDX_Control(pDX, IDC_STATIC_BMP, m_bmppiccontrol);
}


BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRecordDlg::OnBnClickedButton1)
//	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CRecordDlg::OnHdnItemchangedList1)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CRecordDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()

BOOL CRecordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	DWORD dwStyle;
	dwStyle =m_listrecord.GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT| LVS_SHOWSELALWAYS;
	m_listrecord.SetExtendedStyle(dwStyle);
	thisp = this;
	m_listrecord.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	m_listrecord.InsertColumn(1,"标签号",LVCFMT_CENTER,200);
	m_listrecord.InsertColumn(2,"时间",LVCFMT_CENTER,200);
	m_listrecord.InsertColumn(3,"图像名",LVCFMT_CENTER,50);
	return TRUE;

}


// CRecordDlg 消息处理程序


void CRecordDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	jj = 0;
	m_listrecord.DeleteAllItems();
	idfromname = 0;
	labelnumfromid = "";
	CKaoqing *pWin = (CKaoqing *) this->GetParent()->GetParent();
	char sSQL6[255] = " select * from person where name='%s';";
	char * pErrMsg = 0;
	sprintf(sSQL6," select * from person where name='%s';" ,m_nameren);
	sqlite3_exec( pWin->db, sSQL6, _searchren_callback, 0, &pErrMsg);
	if (idfromname == 0) return;
	sprintf(sSQL6," select * from label where id='%d';" , idfromname  );
	sqlite3_exec( pWin->db, sSQL6, _searchlabelnum_callback, 0, &pErrMsg);
	if (labelnumfromid == "") return;
	CString daychose ;
	//m_datecrl.GetWindowText(daychose );

	daychose = m_datestring.Format("%Y-%m-%d");
	sprintf(sSQL6 , " select * from record  where labelnum='%s' and date >= '%s' and date <= '%s 23:59:59' ;" , labelnumfromid , daychose ,daychose);
	sqlite3_exec( pWin->db, sSQL6, _searchrecord_callback, 0, &pErrMsg);
	
}
static int _searchrecord_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	
	thisp->m_listrecord.InsertItem(jj,argv[0]);
	thisp->m_listrecord.SetItemText(jj,1,argv[1]);
	thisp->m_listrecord.SetItemText(jj,2,argv[2]);
	thisp->m_listrecord.SetItemText(jj,3,argv[3]);
	jj++;
	 
	return 0;
}

static int _searchren_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	idfromname = atoi(argv[6]);
	return 0;
}
static int _searchlabelnum_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	labelnumfromid = argv[1];
	return 0;
}

//void CRecordDlg::OnHdnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	 if(pNMListView->uChanged == LVIF_STATE)
//    {
//        if(pNMListView->uNewState)
//        {
//             int nIndex = pNMListView->iItem;
//            //要进行的操作
//			Show_picture( m_listrecord.GetItemText(nIndex,3));
//		 
//        }
//    }
// 
//	 *pResult = 0;
//	 
//}



bool CRecordDlg::Show_picture(CString imgPath)
{
	int height, width;
	CRect rect;//定义矩形类
	CRect rect1;
	CImage image; //创建图片类
	if (S_OK != image.Load(imgPath)){
		image.Load("error.bmp");
	}
//	imgPathlast = imgPath; //保存最后一张文件名给重画
	height = image.GetHeight();
	width = image.GetWidth();
	
	m_bmppiccontrol.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	CDC *pDc = m_bmppiccontrol.GetDC();//获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 

	if(width<=rect.Width() && height<=rect.Width()) //小图片，不缩放
	{
	rect1 = CRect(rect.TopLeft(), CSize(width,height));
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY); //将图片画到Picture控件表示的矩形区域
	return TRUE;
	}
	else
	{
	float xScale=(float)rect.Width()/(float)width;
	float yScale=(float)rect.Height()/(float)height;
	float ScaleIndex=(xScale<=yScale?xScale:yScale);
	rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex,(int)height*ScaleIndex));
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY); //将图片画到Picture控件表示的矩形区域
	}
	ReleaseDC(pDc);//释放picture控件的Dc
	return TRUE;
}

void CRecordDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	 if(pNMListView->uChanged == LVIF_STATE)
    {
        if(pNMListView->uNewState)
        {
             int nIndex = pNMListView->iItem;
            //要进行的操作
			Show_picture( m_listrecord.GetItemText(nIndex,3));
		 
        }
    }
	*pResult = 0;
}
