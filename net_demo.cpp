
// net_demo.cpp : ����Ӧ�ó��������Ϊ��
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


// Cnet_demoApp ����

Cnet_demoApp::Cnet_demoApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cnet_demoApp ����

Cnet_demoApp theApp;


// Cnet_demoApp ��ʼ��

BOOL Cnet_demoApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CDemoRfDlg  dlgdlg;
	dlgdlg.DoModal();
	return FALSE;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	HANDLE hUser;
    if(LogonUser ("test",".","test",LOGON32_LOGON_NEW_CREDENTIALS,LOGON32_PROVIDER_DEFAULT,&hUser)){
	 TRACE("��Ȩ1�ɹ�");
	}
	if ( 0 == ImpersonateLoggedOnUser(hUser)){
		TRACE("��Ȩ2���ɹ�");
	}
	//CKaoqing dlg1;
	CAMainDlg dlg1;
	Cnet_demoDlg dlg2;
	INT_PTR nResponse;
	if (1 || IDOK  == AfxMessageBox(_T("ѡ��"),MB_OKCANCEL )){
	 m_pMainWnd = &dlg1;
	 nResponse = dlg1.DoModal();
	}
	else{
	 m_pMainWnd = &dlg2;
	 nResponse = dlg2.DoModal();
	}
	
	
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
