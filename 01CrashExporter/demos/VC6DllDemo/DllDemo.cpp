
#include <WINDOWS.H>
#include <stdio.h>
#include <tchar.h>
#include "DllDemo.h"
#include <assert.h>

#include "CrashRpt for vc6.0.h"

void InitCrashRpt()
{
	CR_INSTALL_INFO info;
	// memset(&info, 0, sizeof(CR_INSTALL_INFO));
	// info.cb = sizeof(CR_INSTALL_INFO);             // Size of the structure
	// info.pszAppName = _T("CrashRpt Console Test"); // App name
	// info.pszAppVersion = _T("1.0.0");              // App version
	//info.pszRestartCmdLine = _T("/restart"); 
	//info.dwFlags = info.dwFlags|CR_INST_SHOW_GUI;
	
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


void UnInitCrashRpt()
{
	int nUninstRes = crUninstall(); // Uninstall exception handlers
	assert(nUninstRes==0);
}

BOOL APIENTRY DllMain( HANDLE /*hModule*/, 
                       DWORD  ul_reason_for_call, 
                       LPVOID /*lpReserved*/
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitCrashRpt();
		break;
	case DLL_PROCESS_DETACH:
		UnInitCrashRpt();
		break;
	}
	return TRUE;
}


void Unknown()
{

	::MessageBox(NULL, "Unknown exception type specified", "Info", MB_OK);
}


void SEH()
{
	// Access violation
	int *p = 0;
	*p = 0;
}

void RaiseException()
{
	// Raise noncontinuable software exception
	RaiseException(123, EXCEPTION_NONCONTINUABLE, 0, NULL);    
}
void ThrowException()
{
	// Throw typed C++ exception.
	throw 13;	
}