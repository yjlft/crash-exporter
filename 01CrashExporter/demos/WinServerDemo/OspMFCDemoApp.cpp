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
void COspMFCDemoAppApp::stop()
{

}

void COspMFCDemoAppApp::restart()
{
	
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
		TCHAR buff[256];
		crGetLastErrorMsg(buff, 256); // Get last error
		_tprintf(_T("%s\n"), buff); // and output it to the screen
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
	// Standard initialization
	InitCrashExporter();
	LPWSTR *szArglist;
	int nArgs;
	CString strArg;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if( szArglist == NULL ||nArgs != 2)
		goto cleanup;
	strArg = szArglist[1];
	if (strArg == _T("/start"))
		start();
	else if (strArg == _T("/stop"))
		stop();
	else if (strArg == _T("/restart"))
		restart();
	else
		goto cleanup;	//invaid arguments

cleanup:
	LocalFree(szArglist);
	return FALSE;
}


int COspMFCDemoAppApp::ExitInstance()   
{ 
	UnInitCrashExporter();
	return   CWinApp::ExitInstance(); 
} 