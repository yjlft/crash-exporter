#include "CrashRpt.h"
#include "CrashHandler.h"


int CRASHRPTAPI crInstall( __in LPCSTR lpcszDumpFileName /*= NULL*/, __in LPCSTR lpcszStackWalkFileName /*= NULL*/ )
{
	int nStatus = -1;
	CCrashHandler *pCrashHandler = NULL;

	// Check if crInstall() already was called for current process.
	pCrashHandler = CCrashHandler::GetCurrentProcessCrashHandler();

	if(pCrashHandler!=NULL && pCrashHandler->IsInitialized())
	{    
		OutputDebugString(_T("Can't install crash handler to the same process twice."));
		nStatus = 2; 
		goto cleanup;
	}

	if(pCrashHandler==NULL)
	{
		pCrashHandler = new CCrashHandler();
		if(pCrashHandler==NULL)
		{    
			OutputDebugString(_T("Error allocating memory for crash handler."));
			nStatus = 3; 
			goto cleanup;
		}
	}

	pCrashHandler->Init(lpcszDumpFileName, lpcszStackWalkFileName);

	// OK.
	nStatus = 0;
cleanup:

	if(nStatus!=0) // If failed
	{
		if(pCrashHandler!=NULL && !pCrashHandler->IsInitialized())
		{
			// Release crash handler object
			CCrashHandler::ReleaseCurrentProcessCrashHandler();
		}
	}

	return nStatus;
}

int CRASHRPTAPI crUninstall()
{
	// Get crash handler singleton
	CCrashHandler *pCrashHandler = CCrashHandler::GetCurrentProcessCrashHandler();

	// Check if found
	if(pCrashHandler==NULL || !pCrashHandler->IsInitialized())
	{     
		OutputDebugString(_T("Crash handler wasn't preiviously installed for this process."));
		return 1; 
	}

	// Destroy the crash handler.
	int nDestroy = pCrashHandler->Destroy();
	if(nDestroy!=0)
	{
		OutputDebugString(_T("Destroy the crash handler failed."));
		return 2;
	}

	// Free the crash handler object.
	delete pCrashHandler;
	pCrashHandler = NULL;

	return 0;
}
