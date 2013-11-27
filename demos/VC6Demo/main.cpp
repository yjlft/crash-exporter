
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <new.h>
#include <signal.h>
#include <exception>
#include <sys/stat.h>
#include <psapi.h>
#include <rtcapi.h>
#include <Shellapi.h>
#include <dbghelp.h>
#include <assert.h>

#include "..\..\include\CrashRpt for vc6.0.h"

#include <float.h>

#define BIG_NUMBER 0x1fffffff
#pragma warning(disable: 4717) // avoid C4717 warning

void main()
{
    CR_INSTALL_INFO info;
	// memset(&info, 0, sizeof(CR_INSTALL_INFO));
	// info.cb = sizeof(CR_INSTALL_INFO);             // Size of the structure
	// info.pszAppName = _T("CrashRpt Console Test"); // App name
	// info.pszAppVersion = _T("1.0.0");              // App version
	//info.pszRestartCmdLine = _T("/restart"); 
	info.dwFlags = info.dwFlags|CR_INST_SHOW_GUI;
	
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

	printf("Choose an exception type:\n");
	printf("0 - SEH exception\n");
	printf("4 - invalid parameter\n");
	printf("12 - RaiseException\n");
	printf("13 - throw C++ typed exception\n");
	printf("Your choice >  ");

	int ExceptionType = 0;
	scanf("%d", &ExceptionType);

	switch(ExceptionType)
	{
	case 0: // SEH
		{
			// Access violation
			int *p = 0;
			*p = 0;
		}
		break;
	case 4: // invalid parameter
		{      
			char* formatString;
			// Call printf_s with invalid parameters.
			formatString = NULL;
			printf(formatString);

		}
		break;
	case 12: // RaiseException 
		{
			// Raise noncontinuable software exception
			RaiseException(123, EXCEPTION_NONCONTINUABLE, 0, NULL);        
		}
		break;
	case 13: // throw 
		{
			// Throw typed C++ exception.
			throw 13;
		}
		break;
	default:
		{
			printf("Unknown exception type specified.");       
			_getch();
		}
		break;
	}
    int nUninstRes = crUninstall(); // Uninstall exception handlers
    assert(nUninstRes==0);
    nUninstRes;
}


