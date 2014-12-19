// DlgRenyuan.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgRenyuan.h"
#include "afxdialogex.h"
#include "Kaoqing.h"
#include "RecordDlg.h"
#include "DlgVcatch.h"

#include "sqlite3.h"

static int _listshow_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _getlabelid_callback(void * notused, int argc, char ** argv, char ** szColName);
static int _listshowren_callback(void * notused, int argc, char ** argv, char ** szColName);
CDlgRenyuan *thisp;
static int m_label_dbid;
static int jj;
static int j;
// CDlgRenyuan 对话框

IMPLEMENT_DYNAMIC(CDlgRenyuan, CDialogEx)

CDlgRenyuan::CDlgRenyuan(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRenyuan::IDD, pParent)
	, m_labelcomment(_T(""))
	, m_onlycode(_T(""))
	, m_renname(_T(""))
	, m_rensex(_T(""))
	, m_workcode(_T(""))
	, m_imagepath(_T(""))
	, m_apartment(_T(""))
	, m_labelinfo(_T(""))
	, m_passstr(_T(""))
{
	
}

CDlgRenyuan::~CDlgRenyuan()
{
}

void CDlgRenyuan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_labelcomment);
	DDX_Text(pDX, IDC_EDIT7, m_onlycode);
	DDX_Control(pDX, IDC_LIST1, m_listlabel);
	DDX_Control(pDX, IDC_LIST2, m_listren);
	DDX_Text(pDX, IDC_EDIT1, m_renname);
	DDX_Text(pDX, IDC_EDIT2, m_rensex);
	DDX_Text(pDX, IDC_EDIT3, m_workcode);
	DDX_Text(pDX, IDC_EDIT5, m_imagepath);
	DDX_Text(pDX, IDC_EDIT8, m_apartment);
	DDX_CBString(pDX, IDC_COMBO2, m_labelinfo);
	DDX_Control(pDX, IDC_COMBO2, m_labelctl);
	DDX_Control(pDX, IDC_STATIC_RENTOU, m_rentoupic);
	DDX_Text(pDX, IDC_EDIT9, m_passstr);
}


BEGIN_MESSAGE_MAP(CDlgRenyuan, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgRenyuan::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgRenyuan::OnBnClickedButton3)
	ON_WM_CREATE()
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, &CDlgRenyuan::OnNMSetfocusList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlgRenyuan::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgRenyuan::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgRenyuan::OnBnClickedButton4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CDlgRenyuan::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgRenyuan::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgRenyuan::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgRenyuan::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgRenyuan::OnBnClickedButton9)
END_MESSAGE_MAP()


// CDlgRenyuan 消息处理程序
BOOL CDlgRenyuan::OnInitDialog()
{
	CDialog::OnInitDialog();
	jj = 0;
	j=0;
	thisp =  this;
	myparent =(CKaoqing *)GetParent();
	DWORD dwStyle;
	dwStyle =m_listlabel.GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT| LVS_SHOWSELALWAYS;
	m_listlabel.SetExtendedStyle(dwStyle);
	dwStyle =m_listren.GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT| LVS_SHOWSELALWAYS;
	m_listren.SetExtendedStyle(dwStyle);

	UpdateData(TRUE);
	m_listlabel.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	m_listlabel.InsertColumn(1,"注释",LVCFMT_CENTER,100);
	m_listlabel.InsertColumn(2,"唯一码",LVCFMT_CENTER,200);
	UpdateData(FALSE);
	m_listren.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	m_listren.InsertColumn(1,"姓名",LVCFMT_CENTER,100);
	m_listren.InsertColumn(2,"性别",LVCFMT_CENTER,50);
	m_listren.InsertColumn(3,"工号",LVCFMT_CENTER,50);
	m_listren.InsertColumn(4,"部门",LVCFMT_CENTER,100);
	m_listren.InsertColumn(5,"标签",LVCFMT_CENTER,50);
	//m_listren.InsertColumn(6,"检索号",LVCFMT_CENTER,50);
	m_listren.InsertColumn(6,"头像",LVCFMT_CENTER,50);

	char *sSQL_list = " select * from label;";
	char * pErrMsg = 0;
	sqlite3_exec( myparent->db, sSQL_list, _listshow_callback, 0, &pErrMsg);

	char *sSQL_list1 = " select * from person;";
	sqlite3_exec( myparent->db, sSQL_list1, _listshowren_callback, 0, &pErrMsg);

	return TRUE;
}


static int _listshowren_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	
	thisp->m_listren.InsertItem(jj,argv[0]);
	thisp->m_listren.SetItemText(jj,1,argv[1]);
	thisp->m_listren.SetItemText(jj,2,atoi(argv[2])!=1?"女":"男");
	thisp->m_listren.SetItemText(jj,3,argv[3]);
	thisp->m_listren.SetItemText(jj,4,argv[5]);
	thisp->m_listren.SetItemText(jj,5,argv[6]);
	thisp->m_listren.SetItemText(jj,6,argv[4]);
	jj++;
	return 0;


}
void CDlgRenyuan::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgRenyuan::OnBnClickedButton3()
{
	char * pErrMsg = 0;
	int ret = 1;
	char  sSQL2[255] = " ";
	UpdateData(TRUE);
	myparent =(CKaoqing *)GetParent();
	sprintf(sSQL2,"insert into label(comment,num) values('%s','%s');",m_labelcomment,m_onlycode);
	ret = sqlite3_exec( this->myparent->db, sSQL2, 0, 0, &pErrMsg);
	if ( ret != SQLITE_OK ){
		AfxMessageBox(sqlite3_errmsg(this->myparent->db));
		return;
	}
	
	CString tmp;
	int listrowscount = m_listlabel.GetItemCount();
	tmp.Format("%d",listrowscount+1 );
	m_listlabel.InsertItem(listrowscount,tmp);
	tmp.Format("%s", m_labelcomment);
	m_listlabel.SetItemText(listrowscount,1,m_labelcomment);
	tmp.Format("%s", m_onlycode);
	m_listlabel.SetItemText(listrowscount,2,tmp );


	m_labelcomment  = "" ;
	m_onlycode = "";
	UpdateData(FALSE);

	RefreshDlg();


	
}


int CDlgRenyuan::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	

	return 0;
}



 static int _listshow_callback(void * notused, int argc, char ** argv, char ** szColName)
{
   
  thisp->m_listlabel.InsertItem(j,argv[0]);
  thisp->m_listlabel.SetItemText(j,1,argv[2]);
  thisp->m_listlabel.SetItemText(j,2,argv[1]);
  j++;
 
  thisp->m_labelctl.InsertString(-1,argv[1]);
  return 0;
 }

 void CDlgRenyuan::OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 // TODO: 在此添加控件通知处理程序代码
	 *pResult = 0;
 }


 void CDlgRenyuan::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	 if(pNMListView->uChanged == LVIF_STATE)
    {
        if(pNMListView->uNewState)
        {
             int nIndex = pNMListView->iItem;
            //要进行的操作
			 m_labelcomment = m_listlabel.GetItemText(nIndex,1);
			 m_onlycode = m_listlabel.GetItemText(nIndex,2);
			 UpdateData(FALSE);
        }
    }
 
	 *pResult = 0;
 }


 void CDlgRenyuan::OnBnClickedButton5()
 {
	 int nIndex = m_listlabel.GetSelectionMark();
     if (nIndex <0)
     {
        AfxMessageBox("没有选中行");
		return;
     }
	 else{
	    char * pErrMsg = 0;
		int ret = 1;
		char  sSQL2[255] = " ";
		UpdateData(TRUE);
		CString tm_labelcomment = m_listlabel.GetItemText(nIndex,1);
		CString tm_onlycode = m_listlabel.GetItemText(nIndex,2);
		sprintf(sSQL2,"update  label set comment = '%s',num='%s' where comment='%s' and num='%s' ;",m_labelcomment,m_onlycode,tm_labelcomment,tm_onlycode);
		ret = sqlite3_exec( this->myparent->db, sSQL2, 0, 0, &pErrMsg);
		if ( ret != SQLITE_OK ){
			AfxMessageBox(sqlite3_errmsg(this->myparent->db));
			return;
		}
		m_listlabel.SetItemText(nIndex,1,m_labelcomment);
		m_listlabel.SetItemText(nIndex,2,m_onlycode);
	 }

	 RefreshDlg();
 }


 void CDlgRenyuan::OnBnClickedButton4()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 UpdateData(TRUE);
	 int renlistcount = m_listren.GetItemCount();
	 CString tmp;
	 tmp.Format("%d",renlistcount+1);
	 m_listren.InsertItem(renlistcount,tmp);
	 m_listren.SetItemText(renlistcount,1,m_renname);
	 m_listren.SetItemText(renlistcount,2,m_rensex);
	 m_listren.SetItemText(renlistcount,3,m_workcode);
	 m_listren.SetItemText(renlistcount,4,m_apartment);
	 m_listren.SetItemText(renlistcount,5,m_labelinfo);
	 m_listren.SetItemText(renlistcount,6,m_imagepath);

	 int xingbie = 1;
	 if (m_rensex.Trim() != "男"){
		 xingbie = 0;
	 }
	 char * pErrMsg = 0;
     int ret = 1;
	 char  sSQL2[255] = " ";
	 
	 sprintf(sSQL2,"select * from label where num = '%s';",m_labelinfo);
	 ret = sqlite3_exec( this->myparent->db, sSQL2, _getlabelid_callback, 0, &pErrMsg);
	 if ( ret != SQLITE_OK ){
			AfxMessageBox(sqlite3_errmsg(this->myparent->db));
			return;
	 }
	 sprintf(sSQL2,"insert into person(name,sex,workcode,image,apartment,lableid,password) values('%s','%d','%s','%s','%s','%d','%s') ;"   \
		 ,m_renname,xingbie,m_workcode,m_imagepath,m_apartment,m_label_dbid,m_passstr);
	 ret = sqlite3_exec( this->myparent->db, sSQL2, 0, 0, &pErrMsg);
	 if ( ret != SQLITE_OK ){
			AfxMessageBox(sqlite3_errmsg(this->myparent->db));
			return;
	 }
	 int nColumnCount = m_listren.GetHeaderCtrl()->GetItemCount();
	 // Delete all of the columns.
	 for (int i=0; i < nColumnCount; i++)
	 {
		m_listren.DeleteColumn(0);
		
	 }
	 nColumnCount = m_listlabel.GetHeaderCtrl()->GetItemCount();
	 for (int i=0; i < nColumnCount; i++)
	 {
		m_listlabel.DeleteColumn(0);
		
	 }
	 m_listren.DeleteAllItems();
	 m_listlabel.DeleteAllItems();
	 OnInitDialog();

 }
 static int _getlabelid_callback(void * notused, int argc, char ** argv, char ** szColName)
 {

	
	m_label_dbid =  atoi(argv[0]);
	 
	 return 0;

 }

 void CDlgRenyuan::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	 if(pNMListView->uChanged == LVIF_STATE)
    {
        if(pNMListView->uNewState)
        {
             int nIndex = pNMListView->iItem;
            //要进行的操作
			 

			 m_renname = m_listren.GetItemText(nIndex,1);
			 m_rensex = m_listren.GetItemText(nIndex,2);
			 m_workcode = m_listren.GetItemText(nIndex,3);
			 m_apartment = m_listren.GetItemText(nIndex,4);
			 m_labelinfo = m_listren.GetItemText(nIndex,5);
			 int nlistlabelcount = m_listlabel.GetItemCount();
			 for (int i=0;i<nlistlabelcount;i++){
			    if ( m_labelinfo  == m_listlabel.GetItemText(i,0)){
					m_labelinfo = m_listlabel.GetItemText(i,2);
					break;
				}
			 }
			 m_imagepath = m_listren.GetItemText(nIndex,6);
			 UpdateData(FALSE);
			 Show_picture(m_imagepath);
        }
    }
 
	 *pResult = 0;
 }


 void CDlgRenyuan::OnBnClickedButton6()
 {
	 LPCTSTR lpszDefExt = "Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	 CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszDefExt );
	 if (IDOK == dlg.DoModal()){
		 UpdateData(TRUE);
		 m_imagepath = dlg.GetPathName();
		 UpdateData(FALSE);
		 Show_picture(m_imagepath);
		 
	 }
	 return;
 }


 void CDlgRenyuan::OnBnClickedButton8()
 {
	 CRecordDlg dlg;
	 dlg.DoModal();
 }

void CDlgRenyuan::RefreshDlg()
{
	 m_labelctl.ResetContent();
	 int nColumnCount = m_listren.GetHeaderCtrl()->GetItemCount();
	 // Delete all of the columns.
	 for (int i=0; i < nColumnCount; i++)
	 {
		m_listren.DeleteColumn(0);
		
	 }
	 nColumnCount = m_listlabel.GetHeaderCtrl()->GetItemCount();
	 for (int i=0; i < nColumnCount; i++)
	 {
		m_listlabel.DeleteColumn(0);
		
	 }
	 m_listren.DeleteAllItems();
	 m_listlabel.DeleteAllItems();
	 OnInitDialog();


}
bool CDlgRenyuan::Show_picture(CString imgPath)
{
	int height, width;
	CRect rect;//定义矩形类
	CRect rect1;
	CImage image; //创建图片类
	HRESULT hr = image.Load(imgPath);
	if (hr != S_OK) {
		image.Load("error.bmp");
	}
	
//	imgPathlast = imgPath; //保存最后一张文件名给重画
	height = image.GetHeight();
	width = image.GetWidth();
	
	m_rentoupic.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	CDC *pDc = m_rentoupic.GetDC();//获得pictrue控件的Dc
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

void CDlgRenyuan::OnBnClickedButton7()
{
	 int nIndex = m_listren.GetSelectionMark();
     if (nIndex <0)
     {
        AfxMessageBox("没有选中行");
		return;
     }
	 else{
	    char * pErrMsg = 0;
		int ret = 1;
		char  sSQL2[255] = " ";
		UpdateData(TRUE);
		 
	    sprintf(sSQL2,"select * from label where num = '%s';",m_labelinfo);
		m_label_dbid = 0;
	    ret = sqlite3_exec( this->myparent->db, sSQL2, _getlabelid_callback, 0, &pErrMsg);
	    if ( ret != SQLITE_OK ){
			AfxMessageBox(sqlite3_errmsg(this->myparent->db));
			return;
	    }
		int personid = atoi(m_listren.GetItemText(nIndex,0));
		sprintf(sSQL2,"update  person set name = '%s',sex='%s',workcode='%s' ,image='%s' ,apartment='%s',lableid=%d ,password='%s' where id=%d ;",m_renname,m_rensex,m_workcode,m_imagepath, m_apartment,m_label_dbid ,m_passstr ,personid );
		ret = sqlite3_exec( this->myparent->db, sSQL2, 0, 0, &pErrMsg);
		if ( ret != SQLITE_OK ){
			AfxMessageBox(sqlite3_errmsg(this->myparent->db));
			return;
		}
		
	 }
	 int nColumnCount = m_listren.GetHeaderCtrl()->GetItemCount();
	 // Delete all of the columns.
	 for (int i=0; i < nColumnCount; i++)
	 {
		m_listren.DeleteColumn(0);
		
	 }
	 nColumnCount = m_listlabel.GetHeaderCtrl()->GetItemCount();
	 for (int i=0; i < nColumnCount; i++)
	 {
		m_listlabel.DeleteColumn(0);
		
	 }
	 m_listren.DeleteAllItems();
	 m_listlabel.DeleteAllItems();
	 OnInitDialog();
}


void CDlgRenyuan::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgVcatch dlg;
	dlg.DoModal();
}
