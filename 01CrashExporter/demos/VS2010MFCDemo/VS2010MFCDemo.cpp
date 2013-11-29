
// VS2010MFCDemo.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "VS2010MFCDemo.h"
#include "VS2010MFCDemoDlg.h"
#include "CrashRpt.h"
#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVS2010MFCDemoApp

BEGIN_MESSAGE_MAP(CVS2010MFCDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVS2010MFCDemoApp ����

CVS2010MFCDemoApp::CVS2010MFCDemoApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CVS2010MFCDemoApp ����

CVS2010MFCDemoApp theApp;


// CVS2010MFCDemoApp ��ʼ��

int CVS2010MFCDemoApp::Run() 
{
	// Call your crInstall code here ...

	CR_INSTALL_INFO info;
	// Install crash handlers
	int nInstResult = crInstall(&info);            
	assert(nInstResult==0);

	nInstResult = crAddScreenshot(CR_AS_MAIN_WINDOW);
	assert(nInstResult==0);

	// Check result
	if(nInstResult!=0)
	{
		TCHAR buff[256];
		crGetLastErrorMsg(buff, 256); // Get last error
		_tprintf(_T("%s\n"), buff); // and output it to the screen
		return FALSE;
	}

	BOOL bRun;
	BOOL bExit=FALSE;
	while(!bExit)
	{
		bRun= CWinApp::Run();
		bExit=TRUE;
	}

	// Uninstall crash reporting
	//crUninstall();
	return bRun;
}


BOOL CVS2010MFCDemoApp::InitInstance()
{

	CWinApp::InitInstance();
	Run();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CVS2010MFCDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
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

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

