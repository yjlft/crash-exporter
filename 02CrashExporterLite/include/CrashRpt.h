
#ifndef _CRASHRPT_H_
#define _CRASHRPT_H_

#include <windows.h>
#include <dbghelp.h>

#ifdef __cplusplus // Use undecorated names
extern "C" {
#endif

// Define SAL macros to be empty if some old Visual Studio used
#ifndef __reserved
  #define __reserved
#endif
#ifndef __in
  #define __in
#endif
#ifndef __in_opt
  #define __in_opt
#endif
#ifndef __out_ecount_z
  #define __out_ecount_z(x)
#endif


#ifndef CRASHRPT_LIB // If CrashRpt is used as DLL
#define CRASHRPT_DECLSPEC_DLLEXPORT __declspec(dllexport) 
#define CRASHRPT_DECLSPEC_DLLIMPORT __declspec(dllimport) 
#else // If CrashRpt is used as static library
#define CRASHRPT_DECLSPEC_DLLEXPORT 
#define CRASHRPT_DECLSPEC_DLLIMPORT
#endif
	
	// This is needed for exporting/importing functions from/to CrashRpt.dll
#ifdef CRASHRPT_EXPORTS
#define CRASHRPTAPI CRASHRPT_DECLSPEC_DLLEXPORT WINAPI 
#else 
#define CRASHRPTAPI CRASHRPT_DECLSPEC_DLLIMPORT WINAPI
#endif


	int CRASHRPTAPI crInstall( 
		__in LPCSTR lpcszDumpFileName = NULL,
		__in LPCSTR lpcszStackWalkFileName = NULL );

	int CRASHRPTAPI crUninstall();

#ifdef __cplusplus
}
#endif

class CrAutoInstallHelper
{
public:

	//! Installs exception handlers to the caller process
	CrAutoInstallHelper(__in LPCSTR lpcszDumpFileName = NULL,
					__in LPCSTR lpcszStackWalkFileName = NULL )
	{
		m_nInstallStatus = crInstall(lpcszDumpFileName, lpcszStackWalkFileName);
	}

	//! Uninstalls exception handlers from the caller process
	~CrAutoInstallHelper()
	{
		if(m_nInstallStatus==0)
			crUninstall();
	}

	//! Install status
	int m_nInstallStatus;
};
#endif