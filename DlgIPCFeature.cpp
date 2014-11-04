// DlgIPCFeature.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgIPCFeature.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIPCFeature dialog


CDlgIPCFeature::CDlgIPCFeature(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIPCFeature::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIPCFeature)
	m_ae_enable = FALSE;
	m_auto_focus_rect_enable = FALSE;
	m_enable_flip = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgIPCFeature::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIPCFeature)
	DDX_Control(pDX, IDC_COMBO2, m_cbo_shutter);
	DDX_Control(pDX, IDC_COMBO5, m_cbo_db);
	DDX_Control(pDX, IDC_COMBO1, m_cbo_channel);
	DDX_Check(pDX, IDC_CHECK1, m_ae_enable);
	DDX_Check(pDX, IDC_CHK_RECT_ENABLE, m_auto_focus_rect_enable);
	DDX_Check(pDX, IDC_CHK_ENABLE_FLIP, m_enable_flip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIPCFeature, CDialog)
	//{{AFX_MSG_MAP(CDlgIPCFeature)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_AUTO_FOCUS, OnBtnAutoFocus)
	ON_BN_CLICKED(IDC_CHK_RECT_ENABLE, OnChkRectEnable)
	ON_BN_CLICKED(IDC_BTN_ENABLE_FLIP, OnBtnEnableFlip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIPCFeature message handlers
static char*  g_str_ipc_eshutter[]= 
{
	"1 sec",
	"1/10 sec",
	"1/12 sec",
	"1/15 sec",
	"1/20 sec",
	"1/25 sec",
	"1/30 sec",
	"1/50 sec",
	"1/60 sec",
	"1/100 sec",
	"1/120 sec",
	"1/240 sec",
	"1/480 sec",
	"1/960 sec",
	"1/1024 sec"
};

extern USER_HANDLE g_server_id ;
extern int g_window_count;

BOOL CDlgIPCFeature::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_video = NULL;
	m_bmousedown = false;
	memset(&m_auto_focus_rt,0,sizeof(m_auto_focus_rt));

	CString str;
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_channel.AddString(str);
	}
	
	for(i = 0 ; i < sizeof(g_str_ipc_eshutter) / 4; i++)
	{
		m_cbo_shutter.AddString(g_str_ipc_eshutter[i]);
	}

	for(i = 0; i < 31; i++)
	{
		str.Format("%d db",i);
		m_cbo_db.AddString(str);
	}	

	RECT rt;
	GetDlgItem(IDC_VIDEO_IPC)->GetClientRect(&rt);
	int w = rt.right / 8;
	int h = rt.bottom / 5;
	rt.right = (rt.right / w) * w;
	rt.bottom = (rt.bottom / h) * h;
	GetDlgItem(IDC_VIDEO_IPC)->MoveWindow(&rt);

	m_mask_bmp.LoadBitmap(IDB_FOCUS_MASK);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIPCFeature::close()
{
	close_video();
}

BOOL CDlgIPCFeature::commit()
{
	UpdateData(TRUE);
	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	struct tIPCam_feature ipc_feture;
	//重要
	memset(&ipc_feture,0,sizeof(ipc_feture));

	ipc_feture.slot = slot;
	ipc_feture.bae = m_ae_enable;
	ipc_feture.eshutter = m_cbo_shutter.GetCurSel();
	ipc_feture.agcgain = m_cbo_db.GetCurSel();

	if(HW_NET_SET_IPCam_SetFeature(g_server_id,&ipc_feture) == FALSE)
	{
		MessageBox("设置IPC信息失败!");
		return FALSE;
	}
	MessageBox("设置成功!");

	return TRUE;
}

BOOL CDlgIPCFeature::refresh()
{
	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	connect_video();

	struct tIPCam_feature ipc_feature;
	memset(&ipc_feature,0,sizeof(ipc_feature));
	ipc_feature.slot = slot;

	if(HW_NET_SET_IPCam_GetFeature(g_server_id,&ipc_feature) == FALSE)
	{
		MessageBox("获取IPC信息失败!");
		return FALSE;
	}

	m_ae_enable =ipc_feature.bae;
	m_cbo_shutter.SetCurSel(ipc_feature.eshutter);
	m_cbo_db.SetCurSel(ipc_feature.agcgain);

	int is_flip;
	if(HW_NET_SET_GetFlipStatus(g_server_id,slot,&is_flip) == FALSE)
	{
		MessageBox("获取翻转失败");
		return FALSE;
	}

	m_enable_flip = is_flip ? TRUE : FALSE;

	UpdateData(FALSE);
	
	refresh_state();

	return TRUE;
}

void CDlgIPCFeature::refresh_state()
{
		m_cbo_shutter.EnableWindow(!m_ae_enable);
		m_cbo_db.EnableWindow(!m_ae_enable);
}

void CDlgIPCFeature::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}

void CDlgIPCFeature::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	close();

	CDialog::OnClose();
}

void CDlgIPCFeature::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	refresh_state();
}

void CDlgIPCFeature::close_video()
{
	if(m_video)
	{
		delete m_video;
		m_video = NULL;
	}
}

BOOL CDlgIPCFeature::connect_video()
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
	m_video->start_preview(GetDlgItem(IDC_VIDEO_IPC)->GetSafeHwnd(),0);
	m_video->register_draw(draw_fun,(long)this);

	return TRUE;
}

void CDlgIPCFeature::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rt;
	GetDlgItem(IDC_VIDEO_IPC)->GetWindowRect(&rt);
	ScreenToClient((LPPOINT)&rt);
	ScreenToClient((LPPOINT)&rt + 1);
	if(PtInRect(&rt,point))
	{
		m_bmousedown = true;
		m_ori_pt = point;
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgIPCFeature::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bmousedown = false;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgIPCFeature::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	RECT rt;
 	GetDlgItem(IDC_VIDEO_IPC)->GetWindowRect(&rt);

	int pic_w = rt.right - rt.left;
	int pic_h = rt.bottom - rt.top;

	int w = pic_w / 8;
	int h = pic_h / 5;

	ScreenToClient((LPPOINT)&rt);
	ScreenToClient((LPPOINT)&rt + 1);
	if(PtInRect(&rt,point)
		&& m_bmousedown)
	{		
		m_auto_focus_rt.left = (m_ori_pt.x < point.x ? m_ori_pt.x : point.x) - rt.left;
		m_auto_focus_rt.top= (m_ori_pt.y  < point.y ? m_ori_pt.y : point.y) - rt.top;
		m_auto_focus_rt.right = m_auto_focus_rt.left + abs(point.x - m_ori_pt.x) ;
		m_auto_focus_rt.bottom = m_auto_focus_rt.top + abs(point.y - m_ori_pt.y) ;

		m_auto_focus_rt.left = (m_auto_focus_rt.left / w) * w;
		if(m_auto_focus_rt.right % w)
		{
			m_auto_focus_rt.right = (m_auto_focus_rt.right / w + 1) * w;
		}
		
		m_auto_focus_rt.top = (m_auto_focus_rt.top / h) * h;
		if(m_auto_focus_rt.bottom % h)
		{
			m_auto_focus_rt.bottom = (m_auto_focus_rt.bottom / h + 1) * h;
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void __stdcall CDlgIPCFeature:: draw_fun(PLAY_HANDLE handle, HDC hDc, LONG nUser)
{	
	CDC* dc = CDC::FromHandle(hDc);
	CDlgIPCFeature* test= (CDlgIPCFeature*)nUser;
	if(!test->m_auto_focus_rect_enable)
	{
		return;
	}

	RECT rt;
	test->GetDlgItem(IDC_VIDEO_IPC)->GetClientRect(&rt);
	int pic_w = rt.right;
	int pic_h = rt.bottom;
	int w =pic_w / 8;
	int h = pic_h / 5;

	int i = 0,j = 0;
	CPen pen_red(0,1,RGB(250,0,0));
	CPen *oldPen = dc->SelectObject(&pen_red);
	for(i = 1; i <5; i++)
	{
		dc->MoveTo(0,h * i);
		dc->LineTo(rt.right,h * i);
	}
	
	//draw v line
	for(i = 1; i < 8; i++)
	{
		dc->MoveTo(w * i,0);
		dc->LineTo(w * i,rt.bottom);
	}
	dc->SelectObject(oldPen);	


// 	CDC ImageDC,MaskDC;
// 	ImageDC.CreateCompatibleDC(dc);	
// 	ImageDC.SelectObject(&test->m_mask_bmp);
// 	MaskDC.CreateCompatibleDC(dc);		
// 	MaskDC.BitBlt(0,0,rt.right,rt.bottom,&ImageDC,0,0,SRCCOPY);	
// 
// 	ImageDC.BitBlt(0,0,rt.right,rt.bottom,&MaskDC,0,0,SRCAND);	
// 	
// 	dc->StretchBlt(test->m_auto_focus_rt.left,
// 		test->m_auto_focus_rt.top,
// 		test->m_auto_focus_rt.right - test->m_auto_focus_rt.left,
// 		test->m_auto_focus_rt.bottom - test->m_auto_focus_rt.top,
// 		&MaskDC,0,0,10,10,SRCAND);
// 	
// 	dc->StretchBlt(test->m_auto_focus_rt.left,
// 		test->m_auto_focus_rt.top,
// 		test->m_auto_focus_rt.right - test->m_auto_focus_rt.left,
// 		test->m_auto_focus_rt.bottom - test->m_auto_focus_rt.top,
// 		&ImageDC,0,0,10,10,SRCPAINT);
// 	
// 	ImageDC.DeleteDC();
// 	MaskDC.DeleteDC();
		
	
	// draw user rect
	CPen pen_blue(0,1,RGB(0,0,255));
	 dc->SelectObject(&pen_blue);
	
	dc->SelectStockObject(NULL_BRUSH);
	dc->Rectangle(&test->m_auto_focus_rt);
}

void CDlgIPCFeature::OnBtnAutoFocus() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return ;
	}	

	struct tAutoFocus auto_focus;
	memset(&auto_focus,0,sizeof(auto_focus));
	auto_focus.slot = slot;
	if(m_auto_focus_rect_enable)
	{
		RECT rt;
		GetDlgItem(IDC_VIDEO_IPC)->GetClientRect(&rt);
		int pic_w = rt.right;
		int pic_h = rt.bottom;
		int w = pic_w / 8;
		int h = pic_h / 5;
		auto_focus.custom_focus = 1;
 		auto_focus.x = m_auto_focus_rt.left / w;
 		auto_focus.y = m_auto_focus_rt.top /h;
 		auto_focus.w = (m_auto_focus_rt.right - m_auto_focus_rt.left) / w;
 		auto_focus.h = (m_auto_focus_rt.bottom - m_auto_focus_rt.top) / h;
		TRACE("auto focus x %d,y %d,w %d,h %d\n",auto_focus.x,auto_focus.y,auto_focus.w,auto_focus.h);
	}

	if(HW_NET_SET_IPCAM_AutoFocusStart(g_server_id,&auto_focus) == FALSE)
	{
		MessageBox("启动自动聚焦失败!");
	}
}

void CDlgIPCFeature::OnChkRectEnable() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CDlgIPCFeature::OnBtnEnableFlip() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(HW_NET_SET_SetFlipStatus(g_server_id,0,m_enable_flip ? TRUE : FALSE) == FALSE)
	{
		MessageBox("设置翻转失败");
	}	

}
