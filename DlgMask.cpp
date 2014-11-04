// DlgMask.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgMask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMask dialog


CDlgMask::CDlgMask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMask::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMask)
	m_mask_enable = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgMask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMask)
	DDX_Control(pDX, IDC_CBO_SLOT, m_cbo_channel);
	DDX_Check(pDX, IDC_CHK_MASK_ENABLE, m_mask_enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMask, CDialog)
	//{{AFX_MSG_MAP(CDlgMask)
	ON_CBN_SELCHANGE(IDC_CBO_SLOT, OnSelchangeCboSlot)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMask message handlers
extern USER_HANDLE g_server_id ;
extern int g_window_count;
BOOL CDlgMask::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_video = NULL;
	m_bmousedown = false;
	memset(&m_mask_rt,0,sizeof(m_mask_rt));
	
	CString str;
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_channel.AddString(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgMask::connect_video()
{
	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}	
	
	if(m_video)
	{
		delete m_video;
		m_video = NULL;
	}
	
	m_video = new net_video_test(g_server_id,slot);
	m_video->start_preview(GetDlgItem(IDC_VIDEO_MASK)->GetSafeHwnd(),0);
	m_video->register_draw(draw_fun,(long)this);
	
	return TRUE;
}

void CDlgMask::close_video()
{
	if(m_video)
	{
		delete m_video;
		m_video = NULL;
	}
}

void CDlgMask::close()
{
	close_video();
}

BOOL CDlgMask::refresh()
{
	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	connect_video();
	
	tMask mask_info;
	memset(&mask_info,0,sizeof(mask_info));
	mask_info.slot = 0;
	
	if(HW_NET_SET_GetMask(g_server_id,&mask_info) == FALSE)
	{
		MessageBox("获取遮盖失败!");
		return FALSE;
	}

	if(mask_info.count > 0)
	{
		RECT rt;
		GetDlgItem(IDC_VIDEO_MASK)->GetClientRect(&rt);
		int pic_w = rt.right - rt.left;
		int pic_h = rt.bottom - rt.top;
		
		m_mask_rt.left = mask_info.rt[0].left * pic_w / 704;
		m_mask_rt.right = mask_info.rt[0].right * pic_w / 704;
		m_mask_rt.top = mask_info.rt[0].top * pic_h / 576;
		m_mask_rt.bottom = mask_info.rt[0].bottom * pic_h / 576;	
	}
	else
	{
		memset(&m_mask_rt,0,sizeof(m_mask_rt));
	}

	m_mask_enable = mask_info.count > 0;

	UpdateData(FALSE);	
	
	return TRUE;
}

BOOL CDlgMask::commit()
{
	UpdateData(TRUE);

	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	tMask mask_info;
	memset(&mask_info.slot,0,sizeof(mask_info));
	mask_info.slot = slot;
	mask_info.lev = 2;

	if(m_mask_enable)
	{
		mask_info.count = 1;
	}
	else
	{
		mask_info.count = 0;
	}	

	RECT rt;
	GetDlgItem(IDC_VIDEO_MASK)->GetClientRect(&rt);
	int pic_w = rt.right - rt.left;
	int pic_h = rt.bottom - rt.top;

	//转成704 * 576
	mask_info.rt[0].left = m_mask_rt.left * 704 / pic_w;
	mask_info.rt[0].right = m_mask_rt.right * 704 / pic_w;
	mask_info.rt[0].top = m_mask_rt.top * 576 / pic_h;
	mask_info.rt[0].bottom = m_mask_rt.bottom * 576 / pic_h;
	if(!HW_NET_SET_SetMask(g_server_id,&mask_info))
	{
		MessageBox("设置遮盖失败!");
		return FALSE;
	}

	MessageBox("设置成功!");
	
	return TRUE;
}

void CDlgMask::OnSelchangeCboSlot() 
{
	// TODO: Add your control notification handler code here
	refresh();
}

void __stdcall CDlgMask::draw_fun(PLAY_HANDLE handle, HDC hDc, LONG nUser)
{
	CDC* dc = CDC::FromHandle(hDc);
	CDlgMask* test= (CDlgMask*)nUser;	
	
	// draw user rect
	CPen pen_blue(0,1,RGB(0,0,255));
	dc->SelectObject(&pen_blue);
	
	dc->SelectStockObject(NULL_BRUSH);
	dc->Rectangle(&test->m_mask_rt);
}

void CDlgMask::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	close();

	CDialog::OnClose();
}

void CDlgMask::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	RECT rt;
	GetDlgItem(IDC_VIDEO_MASK)->GetWindowRect(&rt);
	ScreenToClient((LPPOINT)&rt);
	ScreenToClient((LPPOINT)&rt + 1);
	if(PtInRect(&rt,point))
	{
		m_bmousedown = true;
		m_ori_pt = point;
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMask::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bmousedown = false;

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMask::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rt;
	GetDlgItem(IDC_VIDEO_MASK)->GetWindowRect(&rt);
	
	int pic_w = rt.right - rt.left;
	int pic_h = rt.bottom - rt.top;
	
	ScreenToClient((LPPOINT)&rt);
	ScreenToClient((LPPOINT)&rt + 1);
	if(PtInRect(&rt,point)
		&& m_bmousedown)
	{		
		m_mask_rt.left = (m_ori_pt.x < point.x ? m_ori_pt.x : point.x) - rt.left;
		m_mask_rt.top= (m_ori_pt.y  < point.y ? m_ori_pt.y : point.y) - rt.top;
		m_mask_rt.right = m_mask_rt.left + abs(point.x - m_ori_pt.x) ;
		m_mask_rt.bottom = m_mask_rt.top + abs(point.y - m_ori_pt.y) ;		
	}

	CDialog::OnMouseMove(nFlags, point);
}
