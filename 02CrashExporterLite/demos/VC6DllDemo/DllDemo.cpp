
#include <WINDOWS.H>
#include <stdio.h>
#include <tchar.h>
#include "DllDemo.h"
#include <assert.h>

#include "CrashRpt.h"

void InitCrashRpt()
{
	crInstall("TestDllDemo", "TestDllDemo");
	//crInstall();
	//CrAutoInstallHelper AutoInstallHelper;
}


void UnInitCrashRpt()
{
	crUninstall();
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