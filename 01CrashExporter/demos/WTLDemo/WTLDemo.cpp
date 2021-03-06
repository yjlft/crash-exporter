/************************************************************************************* 
This file is a part of CrashRpt library.
Copyright (c) 2003-2013 The CrashRpt project authors. All Rights Reserved.

Use of this source code is governed by a BSD-style license
that can be found in the License.txt file in the root of the source
tree. All contributing project authors may
be found in the Authors.txt file in the root of the source tree.
***************************************************************************************/

// CrashRptTest.cpp : main source file for CrashRptTest.exe
//

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "CrashThread.h"
#include <shellapi.h>


CAppModule _Module;
HANDLE g_hWorkingThread = NULL;
CrashThreadInfo g_CrashThreadInfo;

// Helper function that returns path to application directory
CString GetAppDir()
{
    CString string;
    LPTSTR buf = string.GetBuffer(_MAX_PATH);
    GetModuleFileName(NULL, buf, _MAX_PATH);
    *(_tcsrchr(buf,'\\'))=0; // remove executable name
    string.ReleaseBuffer();
    return string;
}

// Helper function that returns path to module
CString GetModulePath(HMODULE hModule)
{
    CString string;
    LPTSTR buf = string.GetBuffer(_MAX_PATH);
    GetModuleFileName(hModule, buf, _MAX_PATH);
    TCHAR* ptr = _tcsrchr(buf,'\\');
    if(ptr!=NULL)
        *(ptr)=0; // remove executable name
    string.ReleaseBuffer();
    return string;
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    // Get command line params
    LPCWSTR szCommandLine = GetCommandLineW();  
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(szCommandLine, &argc);

    CMainDlg dlgMain;

    if(argc==2 && wcscmp(argv[1], L"/restart")==0)
        dlgMain.m_bRestarted = TRUE;
    else
        dlgMain.m_bRestarted = FALSE;

    if(dlgMain.Create(NULL) == NULL)
    {
        ATLTRACE(_T("Main dialog creation failed!\n"));
        return 0;
    }

    dlgMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

BOOL WINAPI CrashCallback(LPVOID lpvState)
{
    UNREFERENCED_PARAMETER(lpvState);

    // Crash happened!
	::MessageBox(NULL, _T("Hello EveryOne!!!"), _T("haha"),MB_OK);

    return TRUE;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    HRESULT hRes = ::CoInitialize(NULL);
    // If you are running on NT 4.0 or higher you can use the following call instead to 
    // make the EXE free threaded. This means that calls come in on a random RPC thread.
    //	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // Install crash reporting

    CR_INSTALL_INFO info;
    memset(&info, 0, sizeof(CR_INSTALL_INFO));
    info.cb = sizeof(CR_INSTALL_INFO);  
    info.pszAppName = _T("WTLDemo"); // Define application name.			
    info.pfnCrashCallback = CrashCallback; // Define crash callback function.   
    info.dwFlags = 0;                    
    info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS; // Install all available exception handlers.    
   
	info.dwFlags |= CR_INST_ALLOW_ATTACH_MORE_FILES; //!< Adds an ability for user to attach more files to crash report by clicking "Attach More File(s)" item from context menu of Error Report Details dialog.

    info.pszDebugHelpDLL = NULL;                    // Search for dbghelp.dll using default search sequence.
    info.uMiniDumpType = MiniDumpNormal;            // Define minidump size.

    info.pszErrorReportSaveDir = NULL;       // Save error reports to the default location.
    info.pszRestartCmdLine = _T("/restart"); // Command line for automatic app restart.

    // Install crash handlers.
    CrAutoInstallHelper cr_install_helper(&info);
    if(cr_install_helper.m_nInstallStatus!=0)
    {
        TCHAR buff[256];
        crGetLastErrorMsg(buff, 256);
        MessageBox(NULL, buff, _T("crInstall error"), MB_OK);
        return FALSE;
    }
    ATLASSERT(cr_install_helper.m_nInstallStatus==0); 

    CString sLogFile = GetAppDir() + _T("\\*.log");

    int nResult = crAddFile2(sLogFile, NULL, _T("Dummy Log File"), CR_AF_MAKE_FILE_COPY|CR_AF_ALLOW_DELETE);
    ATLASSERT(nResult==0);
    

    //nResult = crAddScreenshot2(CR_AS_PROCESS_WINDOWS|CR_AS_USE_JPEG_FORMAT|CR_AS_ALLOW_DELETE, 10);
    nResult = crAddScreenshot(CR_AS_MAIN_WINDOW);
    ATLASSERT(nResult==0);
	
	nResult = crAddProperty(_T("MACAddress"),_T("11.11.11.11"));
    ATLASSERT(nResult==0);

	nResult = crAddProperty(_T("UserName"),_T("who am I?"));
    ATLASSERT(nResult==0);


    /* Create another thread */
    g_CrashThreadInfo.m_bStop = false;
    g_CrashThreadInfo.m_hWakeUpEvent = CreateEvent(NULL, FALSE, FALSE, _T("WakeUpEvent"));
    ATLASSERT(g_CrashThreadInfo.m_hWakeUpEvent!=NULL);

    DWORD dwThreadId = 0;
    g_hWorkingThread = CreateThread(NULL, 0, CrashThread, (LPVOID)&g_CrashThreadInfo, 0, &dwThreadId);
    ATLASSERT(g_hWorkingThread!=NULL);

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();

    // Close another thread
    g_CrashThreadInfo.m_bStop = true;
    SetEvent(g_CrashThreadInfo.m_hWakeUpEvent);
    // Wait until thread terminates
    WaitForSingleObject(g_hWorkingThread, INFINITE);

    ::CoUninitialize();

    return nRet;
}
