// DlgMotion.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgMotion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


net_motion_test::net_motion_test(CWnd* pParent)
{
	m_video = NULL;
	m_show_motion = FALSE;
	m_bmouse_down = FALSE;

	m_mask_bmp.LoadBitmap(IDB_MOTION_MASK);
}


BEGIN_MESSAGE_MAP(net_motion_test, CWnd)
	//{{AFX_MSG_MAP(CDlgAlarm)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL net_motion_test::connect_video(USER_HANDLE server_id,int slot)
{
	if(is_connected())
	{
		return FALSE;
	}

	m_video = new net_video_test(server_id,slot);
	if(m_video->start_preview(GetSafeHwnd(),0))
	{
		tMotionRowCols rowcols;
		memset(&rowcols,0,sizeof(rowcols));
		rowcols.slot = slot;
		
		if(HW_NET_SET_GetMotionRowCols(server_id,&rowcols) == FALSE)
		{
			return FALSE;
		}
		m_motion_col = rowcols.cols;
		m_motion_row = rowcols.rows;
		m_int_num_in_one_row = 1 + (m_motion_col - 1) / 32;
		
		memset(&m_motion_cfg,0,sizeof(m_motion_cfg));
		m_motion_cfg.slot = slot;
		if(HW_NET_SET_GetMotionEx(server_id,&m_motion_cfg) == FALSE)
		{
			return FALSE;
		}		

		hwplay_auto_adjust_size(m_video->handle(),FALSE);
		return TRUE;
	}else{
		memset(&m_motion_cfg,0,sizeof(m_motion_cfg));

		delete m_video;
		m_video = NULL;

		return FALSE;
	}
}

void net_motion_test::close_video()
{
	if(is_connected())
	{
		show_motion(FALSE);

		m_video->stop_preview();
		delete m_video;
		m_video = NULL;
	}
}

BOOL net_motion_test::show_motion(BOOL bshow)
{
	if(!is_connected())
	{
		return FALSE;
	}

	if(bshow == m_show_motion)
	{
		return FALSE;
	}

	if(bshow)
	{		
		hwplay_register_draw_fun(m_video->handle(),motion_draw,(long)this);
	}else{
		hwplay_register_draw_fun(m_video->handle(),NULL,0);
	}

	m_show_motion = bshow;

	return TRUE;
}

BOOL net_motion_test::getxy(POINT point,int* pX,int* pY)
{
	if(pX == NULL || pY == NULL) return false;
	
	RECT rt;
	GetClientRect(&rt);
	int w = rt.right / m_motion_col;
	int h = rt.bottom / m_motion_row;
	
	*pX = point.x / w;
	*pY = point.y / h;
	return true;
}

BOOL net_motion_test::is_motion_set(int x,int y)
{
	int* data = (int*)m_motion_cfg.data;

	return data[y * m_int_num_in_one_row + x / 32] & (1 << (x % 32));
}

BOOL net_motion_test::is_motion_set(POINT pt)
{
	int x,y;
	getxy(pt,&x,&y);

	return is_motion_set(x,y);
}

void net_motion_test::clear_motion(int x, int y)
{
	int* data = (int*)m_motion_cfg.data;
	data[y * m_int_num_in_one_row + x / 32] &= ~(1 << (x % 32));
}

void net_motion_test::clear_all_motion()
{
	memset(m_motion_cfg.data,0,sizeof(m_motion_cfg.data));
}

void net_motion_test::clear_motion(POINT pt)
{
	int x,y;
	getxy(pt,&x,&y);

	clear_motion(x,y);
}

void net_motion_test::set_motion(int x,int y)
{
	int* data = (int*)m_motion_cfg.data;
	data[y * m_int_num_in_one_row + x / 32] |= (1 << (x % 32));
}

void net_motion_test::set_motion(POINT pt)
{
	int x,y;
	getxy(pt,&x,&y);

	set_motion(x,y);
}

void CALLBACK net_motion_test::motion_draw(PLAY_HANDLE handle,HDC hDc,LONG nUser)
{
	net_motion_test* motion_test = (net_motion_test*)nUser;
	if(!motion_test->is_show_motion())
	{
		return;
	}

	CDC* pDc = CDC::FromHandle(hDc);
	
	RECT rt;
	motion_test->GetClientRect(&rt);
	int w = 0,h = 0,left_w = 0,left_h = 0;
	w = (rt.right - rt.left) / motion_test->m_motion_col;
	h = (rt.bottom  - rt.top) / motion_test->m_motion_row;
	left_w = (rt.right - rt.left) % motion_test->m_motion_col;
	left_h = (rt.bottom - rt.top) % motion_test->m_motion_row;
	//TRACE("w %d,left w %d\n",w,left_w);
	//draw h line
	int i = 0,j = 0;
	CPen newPen(0,1,RGB(250,0,0));
	CPen *oldPen = pDc->SelectObject(&newPen);
	for(i = 1; i < motion_test->m_motion_row; i++)
	{
		pDc->MoveTo(0,h * i);
		pDc->LineTo(rt.right,h * i);
	}
	
	//draw v line
	for(i = 1; i < motion_test->m_motion_col; i++)
	{
		pDc->MoveTo(w * i,0);
		pDc->LineTo(w * i,rt.bottom);
	}
	pDc->SelectObject(oldPen);
	
	CDC ImageDC,MaskDC;
	ImageDC.CreateCompatibleDC(pDc);	
	ImageDC.SelectObject(&motion_test->m_mask_bmp);
	MaskDC.CreateCompatibleDC(pDc);		
	MaskDC.BitBlt(0,0,rt.right,rt.bottom,&ImageDC,0,0,SRCCOPY);
	
	ImageDC.SetBkColor(RGB(0,0,0));
	ImageDC.SetTextColor(RGB(255,255,255));
	ImageDC.BitBlt(0,0,rt.right,rt.bottom,&MaskDC,0,0,SRCAND);	
	
	int morex,morey;
	for(i = 0; i < motion_test->m_motion_row; i++)
	{
		for(j = 0; j < motion_test->m_motion_col; j++)
		{
			if(motion_test->is_motion_set(j,i))
			{			
				if(j == motion_test->m_motion_col - 1)
				{
					morex = left_w;
				}else{
					morex = 0;
				}	
				if(i == motion_test->m_motion_row -1)
				{
					morey = left_h;
				}else{
					morey = 0;
				}
				
				pDc->BitBlt(j * w,i * h,w + morex,h + morey,&MaskDC,0,0,SRCAND);
				pDc->BitBlt(j * w,i * h,w + morex,h + morey,&ImageDC,0,0,SRCPAINT);
				
			}
		}
	}
	ImageDC.DeleteDC();
	MaskDC.DeleteDC();
}

BOOL net_motion_test::is_connected()
{
	return m_video != NULL;
}

BOOL net_motion_test::is_show_motion()
{
	return m_show_motion;
}

void net_motion_test::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!is_show_motion())
	{
		return;
	}

	if(is_motion_set(point))
	{
		clear_motion(point);
		m_ori_is_set = FALSE;
	}else{
		set_motion(point);
		m_ori_is_set = TRUE;
	}

	m_ori = point;
	m_bmouse_down = TRUE;

	//CEdit::OnLButtonDown(nFlags,point);
}

void net_motion_test::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bmouse_down = FALSE;

	CEdit::OnLButtonUp(nFlags,point);
}


void net_motion_test::OnMouseMove(UINT nFlags, CPoint point)
{
	//TRACE("on mouse move\n");

	if(m_bmouse_down)
	{
		int orix,oriy;
		getxy(m_ori,&orix,&oriy);
		
		int dstx,dsty;
		getxy(point,&dstx,&dsty);
		
		int minx = min(orix,dstx);
		int maxx = max(orix,dstx);
		int miny = min(oriy,dsty);
		int maxy = max(oriy,dsty);
		
		for(int i = minx; i <= maxx; i++)
		{
			for(int j = miny; j <= maxy; j++)
			{
				if(m_ori_is_set)
				{
					set_motion(i,j);
				}else{
					clear_motion(i,j);
				}				
			}
		}
	}

	CEdit::OnMouseMove(nFlags,point);
}


/////////////////////////////////////////////////////////////////////////////
// CDlgMotion dialog

CDlgMotion::CDlgMotion(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMotion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMotion)
	
	//}}AFX_DATA_INIT
}


void CDlgMotion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMotion)
	DDX_Control(pDX, IDC_COMBO3, m_cbo_sensi);
	DDX_Control(pDX, IDC_COMBO1, m_cbo_slot);
	DDX_Control(pDX, IDC_COMBO4, m_cbo_delay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMotion, CDialog)
	//{{AFX_MSG_MAP(CDlgMotion)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMotion message handlers
extern USER_HANDLE g_server_id;
extern int g_window_count;
void CDlgMotion::close()
{
	close_video();
}

void CDlgMotion::close_video()
{
	m_motion_test.close_video();
}

BOOL CDlgMotion::connect_video()
{
	close_video();

	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	if(m_motion_test.connect_video(g_server_id,slot))
	{
		m_motion_test.show_motion(TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CDlgMotion::commit()
{
	UpdateData(TRUE);

	m_motion_test.m_motion_cfg.lev = m_cbo_sensi.GetCurSel();
	m_motion_test.m_motion_cfg.recDelay = m_cbo_delay.GetCurSel();

	if(HW_NET_SET_SetMotionEx(g_server_id,&m_motion_test.m_motion_cfg) == FALSE)
	{
		MessageBox("设置移动侦测失败!");
		return FALSE;
	}

	MessageBox("设置成功!");
	return TRUE;
}

BOOL CDlgMotion::refresh()
{
	if(!connect_video())
	{
		return FALSE;
	}	

	m_cbo_sensi.SetCurSel(m_motion_test.m_motion_cfg.lev);
	m_cbo_delay.SetCurSel(m_motion_test.m_motion_cfg.recDelay);

	return TRUE;
}

BOOL CDlgMotion::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_motion_test.SubclassDlgItem(IDC_VIDEO,this);

	int i;
	CString str;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_slot.AddString(str);
	}

	m_cbo_sensi.AddString("0 最高");
	for(i = 1; i < 6; i++)
	{
		str.Format("%d",i);
		m_cbo_sensi.AddString(str);
	}
	m_cbo_sensi.AddString("6 最低");

	//int	recDelay;	//0-6:10s,20s,30s,1m,2m,5m,10m	
	m_cbo_delay.AddString("10秒");
	m_cbo_delay.AddString("20秒");
	m_cbo_delay.AddString("30秒");
	m_cbo_delay.AddString("1分钟");
	m_cbo_delay.AddString("2分钟");
	m_cbo_delay.AddString("5分钟");
	m_cbo_delay.AddString("10分钟");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMotion::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	refresh();
}

void CDlgMotion::OnButton1() 
{
	// TODO: Add your control notification handler code here
	m_motion_test.clear_all_motion();
}
