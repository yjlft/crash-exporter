/************************************************************************************* 
This file is a part of CrashRpt library.
Copyright (c) 2003-2013 The CrashRpt project authors. All Rights Reserved.

Use of this source code is governed by a BSD-style license
that can be found in the License.txt file in the root of the source
tree. All contributing project authors may
be found in the Authors.txt file in the root of the source tree.
***************************************************************************************/

// crashcon.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <assert.h>
#include <process.h>
#include "CrashRpt.h" // Include CrashRpt header

LPVOID lpvState = NULL; // Not used, deprecated


DWORD WINAPI ThreadProc( LPVOID /*lpParam*/ )
{
	printf("Press Enter to simulate a null pointer exception or any other key to exit...\n");
	int n = _getch();
	if(n==13)
	{
		int *p = 0;
		*p = 0; // Access violation
	}
	return 0;
}

int main(int argc, char* argv[])
{
    argc; // this is to avoid C4100 unreferenced formal parameter warning
    argv; // this is to avoid C4100 unreferenced formal parameter warning

    // Install crash reporting
    CR_INSTALL_INFO info;
//    memset(&info, 0, sizeof(CR_INSTALL_INFO));
//    info.cb = sizeof(CR_INSTALL_INFO);             // Size of the structure
   // info.pszAppName = _T("CrashRpt Console Test"); // App name
  //  info.pszAppVersion = _T("1.0.0");              // App version
	//info.pszRestartCmdLine = _T("/restart"); 
	info.dwFlags = info.dwFlags|CR_INST_NO_STACKWALK|CR_INST_NO_MINIDUMP;

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


#if 1
	printf("Press Enter to simulate a null pointer exception or any other key to exit...\n");
	int n = _getch();
	if(n==13)
	{
		int *p = 0;
		*p = 0; // Access violation
	}
#else
	DWORD idThread;
	HANDLE hThread;
	hThread = ::CreateThread( NULL, 0, ThreadProc, NULL, 0, &idThread );
	::WaitForSingleObject(hThread, INFINITE);
#endif


#ifdef TEST_DEPRECATED_FUNCS
    Uninstall(lpvState); // Uninstall exception handlers
#else
    int nUninstRes = crUninstall(); // Uninstall exception handlers
    assert(nUninstRes==0);
    nUninstRes;
#endif //TEST_DEPRECATED_FUNCS

    // Exit
    return 0;
}

