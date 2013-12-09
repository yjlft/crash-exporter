// WinServer.cpp: implementation of the WinServer class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <WINSVC.H>
#include <crtdbg.h>
#include <TLHELP32.H>
#include "WinServer.h"
#include "KillProcess.h"


WinServer* WinServer::m_pWinServer = NULL;
//
WinServer* WinServer::Instance()
{
	if (NULL == m_pWinServer)
	{
		m_pWinServer = new WinServer();
	}
	return m_pWinServer;
}
//
WinServer::WinServer()	 
{	
	m_pszLauchAppName = NULL;
	m_pFindKillProcess = NULL;
}

WinServer::~WinServer()
{
	if (m_pFindKillProcess != NULL)
	{
		delete m_pFindKillProcess;
		m_pFindKillProcess = NULL;
	}
}
//
LPTSTR WinServer::GetLastErrorText(LPTSTR pszBuf, DWORD dwSize) 
{
	LPTSTR pszTemp = 0;
	
	DWORD dwRet =	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
		0,
		GetLastError(),
		LANG_NEUTRAL,
		pszTemp,
		0,
		0
		);
	
	if( !dwRet || (dwSize < dwRet + 14) )
		pszBuf[0] = _T('\0');
	else 
	{
		pszTemp[_tcsclen(pszTemp) - 2] = _T('\0');  //remove cr/nl characters
		_tcscpy(pszBuf, pszTemp);
	}
	
	if( pszTemp )
		LocalFree(HLOCAL(pszTemp));
	
	return pszBuf;
}

int WinServer::Init(WINSERVER_INFO* pInfo)
{
	int nStatus = -1;
	
    // Validate input parameters.
    if(pInfo == NULL ||  pInfo->cb != sizeof(WINSERVER_INFO))     
		return 1;
	if (pInfo->pszWinServerName == NULL)
		return 2;
	if (pInfo->pszLauchAppName == NULL)
		return 3;
	m_pszWinServerName = pInfo->pszWinServerName;
	m_pszLauchAppName = pInfo->pszLauchAppName;
	m_pszLauchAppCmdLine = pInfo->pszLauchAppCmdLine;
	m_bLauchApp = (pInfo->dwFlags&NOT_LAUCH_APP) == 0;
	m_bkillAppBeforeLauch = (pInfo->dwFlags&NOT_KILL_PRELAUCHED_APP) == 0;
	m_bKillAppAfterStopService = (pInfo->dwFlags&NOT_KILL_LAUCHED_APP) == 0;
	
	return 0;
}

void WinServer::Run(DWORD dwCmdCode)
{
	switch (dwCmdCode)
	{
	case CODE_INSTANCE:
		InstanceServer();
		break;
	case CODE_UNINSTANCE:
		UninstanceServer();
		break;
	case CODE_START:
		RunService();
		break;
	case CODE_STOP:
		StopService();
		break;	
	case CODE_NONE:
		{
			SERVICE_TABLE_ENTRY dispatchTable[] =
			{
				{ LPTSTR(m_pszWinServerName), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
				{ 0, 0 }
			};
			//
			if (!StartServiceCtrlDispatcher(dispatchTable))
			{
				TCHAR szBuf[MAX_LEN];
				GetLastErrorText(szBuf, MAX_LEN);
				_tprintf(_T("CODE_NONE GetLastErrorText is %s.\n"), szBuf);
			}
		}
		break;
	default:
		break;
	}
}
void WINAPI WinServer::ServiceMain(DWORD argc, LPTSTR* argv)
{
	_ASSERTE(m_pWinServer != NULL);
	if (m_pWinServer == NULL)
		return;
	
	m_pWinServer->m_hServiceStatus =	RegisterServiceCtrlHandler(
		m_pWinServer->m_pszWinServerName, WinServer::ServiceCtrl);
	
	if( m_pWinServer->m_hServiceStatus )
	{
		m_pWinServer->ReportStatus(SERVICE_START_PENDING);
		
		m_pWinServer->ReportStatus(SERVICE_STOPPED);
	}			
}
//
void WINAPI WinServer::ServiceCtrl(DWORD dwCtrlCode)
{
	_ASSERT(m_pWinServer);
	if (m_pWinServer == NULL)
		return;
	
	switch( dwCtrlCode ) {
	case SERVICE_CONTROL_STOP:				
		{
			m_pWinServer->m_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		}
		break;		
	case SERVICE_CONTROL_PAUSE:	
		{
			m_pWinServer->m_ServiceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
			m_pWinServer->ReportStatus(m_pWinServer->m_ServiceStatus.dwCurrentState);
		}
		break;		
	case SERVICE_CONTROL_CONTINUE:
		{
			m_pWinServer->m_ServiceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
			m_pWinServer->ReportStatus(m_pWinServer->m_ServiceStatus.dwCurrentState);
		}		
		break;		
	case SERVICE_CONTROL_SHUTDOWN:
		{
			m_pWinServer->m_ServiceStatus.dwCurrentState = SERVICE_CONTROL_SHUTDOWN;					
		}		
		break;		
	case SERVICE_CONTROL_INTERROGATE:
		{
			m_pWinServer->ReportStatus(m_pWinServer->m_ServiceStatus.dwCurrentState);
		}		
		break;		
	default:
		break;
	}
}
//
void WinServer::InstanceServer()
{
	TCHAR szPath[MAX_LEN] = {0};
	
	if(GetModuleFileName(0, szPath, MAX_LEN - 1) != 0)
	{
		SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if(schSCManager == NULL) 
			return;
		
		SC_HANDLE schService = CreateService(
			schSCManager,
			m_pszWinServerName,
			m_pszWinServerName,
			SERVICE_ALL_ACCESS,
			SERVICE_INTERACTIVE_PROCESS|SERVICE_WIN32_SHARE_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			szPath,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);
		if( schService ) 
		{
			_tprintf(_T("Create %s service success.\n"), m_pszWinServerName );
			CloseServiceHandle(schService);
		} 
		else 
		{
			_tprintf(_T("Create %s service failed.\n"), m_pszWinServerName );
		}
		
		CloseServiceHandle(schSCManager);
	}
}
//
void WinServer::UninstanceServer()
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if(schSCManager == NULL) 
	{
		_tprintf(_T("OpenSCManager failed\n"));
		return;
	}
	
	schService = OpenService(schSCManager, m_pszWinServerName, SERVICE_ALL_ACCESS);		
	if( schService == NULL) 
	{
		_tprintf(_T("OpenService failed \n"));
		goto cleanup;
	}
	
	if( ControlService(schService, SERVICE_CONTROL_STOP, &m_ServiceStatus) ) 
	{
		_tprintf(_T("Stopping %s."), m_pszWinServerName);
		Sleep(1000);
		
		while(QueryServiceStatus(schService, &m_ServiceStatus)) 
		{
			if(m_ServiceStatus.dwCurrentState == SERVICE_STOP_PENDING) 
			{
				_tprintf(_T("."));
				Sleep(1000);
			} 
			else
				break;
		}
		
		if( m_ServiceStatus.dwCurrentState == SERVICE_STOPPED )
			_tprintf(_T("\n%s stop success.\n"), m_pszWinServerName);
		
		else
			_tprintf(_T("\n%s stop failed.\n"), m_pszWinServerName);
	}
	if (m_bKillAppAfterStopService)
	{
		int bRet = KillLauchedApp();	
		if (0 == bRet)
			_tprintf(_T("\nKill Lauched App process success.\n"));
		else if (1 == bRet)
			_tprintf(_T("\nFind Lauched App failed.\n"));
		else if (2 == bRet)
			_tprintf(_T("\nKill Lauched App process failed.\n"));
	}	
	// now remove the service
	if( DeleteService(schService) ) 
		_tprintf(_T("Remove %s success.\n"), m_pszWinServerName); 
	else 
		_tprintf(_T("Remove %s failed.\n"));			
	
cleanup:
	if (schService != NULL)
		CloseHandle(schService);
	if (schSCManager != NULL)
		CloseHandle(schSCManager);
}

//
void WinServer::RunService()
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	
	schSCManager = ::OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if(schSCManager == NULL) 
	{
		_tprintf(_T("OpenSCManager failed\n"));
		return;
	}
	
	schService = OpenService(schSCManager, m_pszWinServerName, SERVICE_ALL_ACCESS);		
	if( schService == NULL) 
	{
		_tprintf(_T("OpenService failed \n"));
		goto cleanup;
	}
	
	// try to start the service
	_tprintf(_T("Starting up %s."), m_pszWinServerName);
	if (m_pszLauchAppName == NULL)
	{
		_tprintf(_T("\nLauchAppName is NULL"));
		goto cleanup;
	}
	
	if (m_bkillAppBeforeLauch)
		KillLauchedApp();
	
	if (m_bLauchApp)
	{
		if (LaunchApp(m_pszLauchAppName, (LPTSTR)m_pszLauchAppCmdLine))
			_tprintf(_T("\nRun new App process success.\n"));
		else
			_tprintf(_T("\nRun new App process failed.\n"));
	}
	
cleanup:
	if (schService != NULL)
		CloseHandle(schService);
	if (schSCManager != NULL)
		CloseHandle(schSCManager);
}

//
void WinServer::StopService()
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	
	schSCManager = ::OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if(schSCManager == NULL) 
	{
		_tprintf(_T("OpenSCManager failed\n"));
		return;
	}
	
	schService = OpenService(schSCManager, m_pszWinServerName, SERVICE_ALL_ACCESS);		
	if( schService == NULL) 
	{
		_tprintf(_T("OpenService failed \n"));
		goto cleanup;
	}
	if (m_pszLauchAppName == NULL)
		goto cleanup;
	
	if (m_bKillAppAfterStopService)
	{
		int bRet = KillLauchedApp();	
		if (0 == bRet)
			_tprintf(_T("\nKill Lauched App process success.\n"));
		else if (1 == bRet)
			_tprintf(_T("\nFind Lauched App failed.\n"));
		else if (2 == bRet)
			_tprintf(_T("\nKill Lauched App process failed.\n"));
	}
	
cleanup:
	if (schService != NULL)
		CloseHandle(schService);
	if (schSCManager != NULL)
		CloseHandle(schSCManager);
}
//
BOOL WinServer::ReportStatus(DWORD dwCurrentState, DWORD dwWaitHint,DWORD dwErrExit)
{
	m_ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    m_ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_SHUTDOWN|SERVICE_ACCEPT_STOP; 
    m_ServiceStatus.dwWin32ExitCode      = 0; 
    m_ServiceStatus.dwServiceSpecificExitCode = 0; 
    m_ServiceStatus.dwCheckPoint         = 0; 
    m_ServiceStatus.dwWaitHint           = 0; 
	m_ServiceStatus.dwCurrentState       = dwCurrentState; 
    m_ServiceStatus.dwCheckPoint         = 0; 
    m_ServiceStatus.dwWaitHint           = 0;
	SetServiceStatus (m_hServiceStatus, &m_ServiceStatus);
	return TRUE;
	
}

// Launches App process
BOOL WinServer::LaunchApp(LPCTSTR pszAppName, LPTSTR pszCmdLineParams)
{
    /* Create App process */
	
    STARTUPINFO si;
    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
	
    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(PROCESS_INFORMATION));    
	
    BOOL bCreateProcess = CreateProcess(
        pszAppName, pszCmdLineParams, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if(pi.hThread)
    {
        CloseHandle(pi.hThread);
        pi.hThread = NULL;
    }
    if(!bCreateProcess)
    {
        _ASSERTE(bCreateProcess);
        return FALSE;
    }
	
    CloseHandle( pi.hProcess );
    pi.hProcess = NULL;	
	return TRUE;
}

int WinServer::KillLauchedApp()
{
	DWORD pid = GetFindKillProcessInstance()->FindProcess(m_pszLauchAppName);
	if (pid == 0)
		return 1;
	
	if (!GetFindKillProcessInstance()->KillProcess(pid, TRUE))
		return 2;
	return 0;
}

CFindKillProcess* WinServer::GetFindKillProcessInstance()
{
	if (m_pFindKillProcess == NULL)
	{
		m_pFindKillProcess = new CFindKillProcess();
	}
	return m_pFindKillProcess;
}


/*
* this func has Packaged as a class, see CFindKillProcess
*
DWORD WINAPI KillProcessProc(LPVOID lpParam)
{	
LPCTSTR szExeName = (LPCTSTR)lpParam;
HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
PROCESSENTRY32 pe;
pe.dwSize = sizeof(PROCESSENTRY32);
HANDLE hProcess;  
Process32First(hSnapshot,&pe);  
do{  
if(strcmp(pe.szExeFile, szExeName)==0){ 
hProcess=OpenProcess(PROCESS_TERMINATE,FALSE, pe.th32ProcessID);  
if(hProcess){  
TerminateProcess(hProcess,0);  
CloseHandle(hProcess); 
}  
} 
}while(Process32Next(hSnapshot,&pe));  
CloseHandle(hSnapshot);  
return 0;  

  }
  
	BOOL WinServer::KillProcess(LPCTSTR pszExeName)
	{
	HANDLE hThread;  	
    hThread=CreateThread(NULL, 0, KillProcessProc, (LPVOID)pszExeName, 0, NULL);  
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);  
	return TRUE;
	}
*/

