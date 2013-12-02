/************************************************************************************* 
This file is a part of CrashRpt library.
Copyright (c) 2003-2013 The CrashRpt project authors. All Rights Reserved.

Use of this source code is governed by a BSD-style license
that can be found in the License.txt file in the root of the source
tree. All contributing project authors may
be found in the Authors.txt file in the root of the source tree.
***************************************************************************************/

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <signal.h>
#include <exception>
#include <float.h>
#include <assert.h>

#include "CrashRpt.h" // Include CrashRpt header

LPVOID lpvState = NULL; // Not used, deprecated


DWORD WINAPI ThreadProc( LPVOID /*lpParam*/ )
{
	int *p = 0;
	*p = 0; // Access violation
	return 0;
}


void sigfpe_test()
{ 
	// Code taken from http://www.devx.com/cplus/Article/34993/1954

	//Set the x86 floating-point control word according to what
	//exceptions you want to trap. 
	_clearfp(); //Always call _clearfp before setting the control
	//word
	//Because the second parameter in the following call is 0, it
	//only returns the floating-point control word
	unsigned int cw; 
	_controlfp_s(&cw, 0, 0); //Get the default control
	//word
	//Set the exception masks off for exceptions that you want to
	//trap.  When a mask bit is set, the corresponding floating-point
	//exception is //blocked from being generating.
	cw &=~(EM_OVERFLOW|EM_UNDERFLOW|EM_ZERODIVIDE|
		EM_DENORMAL|EM_INVALID);
	//For any bit in the second parameter (mask) that is 1, the 
	//corresponding bit in the first parameter is used to update
	//the control word.  
	unsigned int cwOriginal;
	_controlfp_s(&cwOriginal, cw, MCW_EM); //Set it.
	//MCW_EM is defined in float.h.
	//Restore the original value when done:
	//_controlfp(cwOriginal, MCW_EM);

	// Divide by zero

	float a = 1;
	float b = 0;
	float c = a/b;
	c; 
}

#define BIG_NUMBER 0x1fffffff
#pragma warning(disable: 4717) // avoid C4717 warning
int RecurseAlloc() 
{
	int *pi = new int[BIG_NUMBER];
	pi;
	RecurseAlloc();
	return 0;
}

class CDerived;
class CBase
{
public:
	CBase(CDerived *derived): m_pDerived(derived) {};
	~CBase();
	virtual void function(void) = 0;

	CDerived * m_pDerived;
};

#pragma warning(disable:4355)
class CDerived : public CBase
{
public:
	CDerived() : CBase(this) {};   // C4355
	virtual void function(void) {};
};

CBase::~CBase()
{
	m_pDerived -> function();
}


int main(int argc, char* argv[])
{
	argc; // this is to avoid C4100 unreferenced formal parameter warning
	argv; // this is to avoid C4100 unreferenced formal parameter warning

	// Install crash reporting
	CR_INSTALL_INFO info;
	// memset(&info, 0, sizeof(CR_INSTALL_INFO));
	// info.cb = sizeof(CR_INSTALL_INFO);             // Size of the structure
	// info.pszAppName = _T("CrashRpt Console Test"); // App name
	// info.pszAppVersion = _T("1.0.0");              // App version
	// info.pszRestartCmdLine = _T("/restart"); 
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


	printf("Choose an exception type:\n");
	printf("0 - SEH exception\n");
	printf("1 - terminate\n");
	printf("2 - unexpected\n");
	printf("3 - pure virtual method call\n");
	printf("4 - invalid parameter\n");
	printf("5 - new operator fault\n");	
	printf("6 - SIGABRT\n");
	printf("7 - SIGFPE\n");
	printf("8 - SIGILL\n");
	printf("9 - SIGINT\n");
	printf("10 - SIGSEGV\n");
	printf("11 - SIGTERM\n");
	printf("12 - RaiseException\n");
	printf("13 - throw C++ typed exception\n");
	printf("Your choice >  ");

	int ExceptionType = 0;
	scanf_s("%d", &ExceptionType);

	switch(ExceptionType)
	{
	case 0: // SEH
		{ 
						// Access violation
		#if 1
			int *p = 0;
			#pragma warning(disable : 6011)   // warning C6011: Dereferencing NULL pointer 'p'
			*p = 0;
			#pragma warning(default : 6011)   
		#else
			DWORD idThread;
			HANDLE hThread;
			hThread = ::CreateThread( NULL, 0, ThreadProc, NULL, 0, &idThread );
			::WaitForSingleObject(hThread, INFINITE);
		#endif

		}
		break;
	case 1: // terminate
		{
			// Call terminate
			terminate();
		}
		break;
	case 2: // unexpected
		{
			// Call unexpected
			unexpected();
		}
		break;
	case 3: // pure virtual method call
		{
			// pure virtual method call
			CDerived derived;
		}
		break;
	case 4: // invalid parameter
		{      
			char* formatString;
			// Call printf_s with invalid parameters.
			formatString = NULL;
#pragma warning(disable : 6387)   // warning C6387: 'argument 1' might be '0': this does not adhere to the specification for the function 'printf'
			printf(formatString);
#pragma warning(default : 6387)   

		}
		break;
	case 5: // new operator fault
		{
			// Cause memory allocation error
			RecurseAlloc();
		}
		break;
	case 6: // SIGABRT 
		{
			// Call abort
			abort();
		}
		break;
	case 7: // SIGFPE
		{
			// floating point exception ( /fp:except compiler option)
			sigfpe_test();            
		}    
		break;
	case 8: // SIGILL 
		{
			raise(SIGILL);              
		}    
		break;
	case 9: // SIGINT 
		{
			raise(SIGINT);              
		}    
		break;
	case 10: // SIGSEGV 
		{
			raise(SIGSEGV);              
		}    
		break;
	case 11: // SIGTERM
		{
			raise(SIGTERM);            
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
#ifdef TEST_DEPRECATED_FUNCS
	Uninstall(lpvState); // Uninstall exception handlers
#else
	int nUninstRes = crUninstall(); // Uninstall exception handlers
	assert(nUninstRes==0);
	nUninstRes;
#endif //TEST_DEPRECATED_FUNCS

	return 0;
}


