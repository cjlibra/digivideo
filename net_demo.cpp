
// net_demo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "net_demo.h"
#include "net_demoDlg.h"
#include "Kaoqing.h"

#include "AMainDlg.h"

#include "DemoRfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cnet_demoApp

BEGIN_MESSAGE_MAP(Cnet_demoApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// Cnet_demoApp 构造

Cnet_demoApp::Cnet_demoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cnet_demoApp 对象

Cnet_demoApp theApp;


// Cnet_demoApp 初始化

BOOL Cnet_demoApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CDemoRfDlg  dlgdlg;
	dlgdlg.DoModal();
	return FALSE;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	HANDLE hUser;
    if(LogonUser ("test",".","test",LOGON32_LOGON_NEW_CREDENTIALS,LOGON32_PROVIDER_DEFAULT,&hUser)){
	 TRACE("授权1成功");
	}
	if ( 0 == ImpersonateLoggedOnUser(hUser)){
		TRACE("授权2不成功");
	}
	//CKaoqing dlg1;
	CAMainDlg dlg1;
	Cnet_demoDlg dlg2;
	INT_PTR nResponse;
	if (1 || IDOK  == AfxMessageBox(_T("选择"),MB_OKCANCEL )){
	 m_pMainWnd = &dlg1;
	 nResponse = dlg1.DoModal();
	}
	else{
	 m_pMainWnd = &dlg2;
	 nResponse = dlg2.DoModal();
	}
	
	
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
