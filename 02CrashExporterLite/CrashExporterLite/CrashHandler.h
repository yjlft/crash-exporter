#pragma once
#include "stdafx.h"
#include "StackWalker.h"

#if !defined _CrashHandler_H
#define _CrashHandler_H

// #define _STACKWALKER_EXPORT_H_
// #ifdef _STACKWALKER_EXPORT_H_
// class _declspec(dllexport) CCrashHandler
// #else 
// class _declspec(dllimport) CCrashHandler
// #endif
class CCrashHandler  
{
public:
	// Constructor
	CCrashHandler();

	// Destructor
	virtual ~CCrashHandler();

	// Initializes the crash handler object.
	void Init(
		__in_opt LPCSTR lpcszDumpFileName = NULL,
		__in_opt LPCSTR lpcszStackWalkFileName = NULL);

	// Returns TRUE if object was initialized.
	BOOL IsInitialized();

	// Frees all used resources.
	int Destroy();

	// Sets exception handlers that work on per-process basis
	void SetProcessExceptionHandlers();

	// Installs C++ exception handlers that function on per-thread basis
	void SetThreadExceptionHandlers();

	//+
	// Returns the crash handler object (singleton).
	static CCrashHandler* GetCurrentProcessCrashHandler();
	// Releases the singleton of this crash handler object.
	static void ReleaseCurrentProcessCrashHandler();
private:
	// Singleton of the CCrashHandler class.
	static CCrashHandler* m_pProcessCrashHandler; 

	static LPCSTR m_szDumpFileName;				
	static LPCSTR m_szStackWalkFileName;

	BOOL m_bInitialized;           // Flag telling if this object was initialized.  

	static OutStackWalker* m_psw;	
	static OutStackWalker* GetStackWalkerInstance(){
		if (!m_psw)
			m_psw = new OutStackWalker();
		return m_psw;
	}


	//////////////////////////////////////////////////////////////////////////

	// Collects current process state.
	static void GetExceptionPointers(
		DWORD dwExceptionCode, 
		EXCEPTION_POINTERS** pExceptionPointers);

	// This method creates minidump of the process
	static void CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs);

	/* Exception handler functions. */

	static LONG WINAPI SehHandler(PEXCEPTION_POINTERS pExceptionPtrs);
	static void __cdecl TerminateHandler();
	static void __cdecl UnexpectedHandler();

	static void __cdecl PureCallHandler();

	static void __cdecl InvalidParameterHandler(const wchar_t* expression, 
		const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);

	static int __cdecl NewHandler(size_t);

	static void SigabrtHandler(int);
	static void SigfpeHandler(int /*code*/, int subcode);
	static void SigintHandler(int);
	static void SigillHandler(int);
	static void SigsegvHandler(int);
	static void SigtermHandler(int);
	static void ShowCallstack(HANDLE hThread, const CONTEXT *context);
};

#endif
