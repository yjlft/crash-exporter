// OspMFCDemoApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OspMFCDemoApp.h"
#include "OspMFCDemoAppDlg.h"
#include "CrashRpt for vc6.0.h"
#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define POCESS_NAME  _T("OspMFCDemoApp")

/////////////////////////////////////////////////////////////////////////////
// COspMFCDemoAppApp

BEGIN_MESSAGE_MAP(COspMFCDemoAppApp, CWinApp)
	//{{AFX_MSG_MAP(COspMFCDemoAppApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COspMFCDemoAppApp construction

COspMFCDemoAppApp::COspMFCDemoAppApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COspMFCDemoAppApp object

COspMFCDemoAppApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COspMFCDemoAppApp initialization

void COspMFCDemoAppApp::start()
{
	COspMFCDemoAppDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
}

void InitCrashExporter()
{
	CR_INSTALL_INFO info;
	// Install crash handlers
	int nInstResult = crInstall(&info);            
	assert(nInstResult==0);
	
	nInstResult = crAddScreenshot(CR_AS_MAIN_WINDOW);
	assert(nInstResult==0);
	
	// Check result
	if(nInstResult!=0)
	{
		TCHAR szbuff[256];
		crGetLastErrorMsg(szbuff, 256); // Get last error
		_tprintf(_T("%s\n"), szbuff); // and output it to the screen
		return;
	}
}

void UnInitCrashExporter()
{
	// Uninstall crash reporting
	crUninstall();
}


BOOL COspMFCDemoAppApp::InitInstance()
{
	m_hMutex = ::CreateMutex(NULL, FALSE, POCESS_NAME);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::MessageBox(NULL, _T("程序已经在运行"), _T("提示"), MB_OK);
		return FALSE;
	}

	// Standard initialization
	InitCrashExporter();
	LPWSTR *szArglist;
	int nArgs;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if( szArglist == NULL ||nArgs != 2)
		goto cleanup;
	if ((CString)szArglist[1] == _T("/start"))
		start();
	else
		goto cleanup;	//invaid arguments

cleanup:
	LocalFree(szArglist);
	return FALSE;
}


int COspMFCDemoAppApp::ExitInstance()   
{ 
	UnInitCrashExporter();
	CloseHandle(m_hMutex);  
	return  CWinApp::ExitInstance(); 
} 