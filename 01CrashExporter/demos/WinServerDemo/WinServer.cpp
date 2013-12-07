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


WinServer* WinServer::m_ptrServer = NULL;
LPCTSTR WinServer::m_szWinServerName = NULL;
//
WinServer* WinServer::Instance()
{
	if (NULL == m_ptrServer)
	{
		m_ptrServer = new WinServer(m_szWinServerName);
	}
	return m_ptrServer;
}
//
WinServer::WinServer(LPCTSTR strServerName):_dwCheckPoint(0)		 
{	
	m_szLauchAppName = NULL;
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
LPTSTR WinServer :: GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize ) 
{
	LPTSTR lpszTemp = 0;
	
	DWORD dwRet =	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
		0,
		GetLastError(),
		LANG_NEUTRAL,
		(LPTSTR)&lpszTemp,
		0,
		0
		);
	
	if( !dwRet || (dwSize < dwRet+14) )
		lpszBuf[0] = _T('\0');
	else {
		lpszTemp[_tcsclen(lpszTemp)-2] = _T('\0');  //remove cr/nl characters
		_tcscpy(lpszBuf, lpszTemp);
	}
	
	if( lpszTemp )
		LocalFree(HLOCAL(lpszTemp));
	
	return lpszBuf;
}
void WinServer::SetLauchAppName(LPCTSTR szAppName)
{
	m_szLauchAppName = szAppName;
}

void WinServer::SetWinServerName(LPCTSTR szWinServerName)
{
	 m_szWinServerName = szWinServerName;
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
				{ LPTSTR(m_szWinServerName), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
				{ 0, 0 }
			};
			//
			BOOL bRet = StartServiceCtrlDispatcher(dispatchTable);
			if (FALSE == bRet)
			{
				TCHAR szBuf[256];
				GetLastErrorText(szBuf,255);
			}
		}
		break;
	default:
		break;
	}
}
void WINAPI WinServer::ServiceMain(DWORD argc, LPTSTR * argv)
{
	_ASSERTE(m_ptrServer != NULL);
	
	m_ptrServer->_hServiceStatus =	RegisterServiceCtrlHandler(m_ptrServer->m_szWinServerName,WinServer::ServiceCtrl);
	
	if( m_ptrServer->_hServiceStatus )
	{
		m_ptrServer->ReportStatus(SERVICE_START_PENDING);

		m_ptrServer->ReportStatus(SERVICE_STOPPED);
	}			
}
//
void WINAPI WinServer::ServiceCtrl(DWORD dwCtrlCode)
{
	_ASSERT(m_ptrServer);

	switch( dwCtrlCode ) {
	case SERVICE_CONTROL_STOP:				
		{
			Instance()->_tServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		}
		break;		
	case SERVICE_CONTROL_PAUSE:	
		{
			Instance()->_tServiceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
			Instance()->ReportStatus(Instance()->_tServiceStatus.dwCurrentState);
		}
		break;		
	case SERVICE_CONTROL_CONTINUE:
		{
			Instance()->_tServiceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
			Instance()->ReportStatus(Instance()->_tServiceStatus.dwCurrentState);
		}		
		break;		
	case SERVICE_CONTROL_SHUTDOWN:
		{
			Instance()->_tServiceStatus.dwCurrentState = SERVICE_CONTROL_SHUTDOWN;					
		}		
		break;		
	case SERVICE_CONTROL_INTERROGATE:
		{
			Instance()->ReportStatus(Instance()->_tServiceStatus.dwCurrentState);
		}		
		break;		
	default:
		break;
	}
}
//
void WinServer::InstanceServer()
{
	char achPath[MAX_LEN] = {0};
	if(GetModuleFileName( 0, achPath, MAX_LEN - 1 ) != 0)
	{
		SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if(schSCManager) 
		{
			SC_HANDLE schService =	CreateService(
							schSCManager,
							m_szWinServerName,
							m_szWinServerName,
							SERVICE_ALL_ACCESS,
							SERVICE_INTERACTIVE_PROCESS|SERVICE_WIN32_SHARE_PROCESS,
							SERVICE_AUTO_START,
							SERVICE_ERROR_NORMAL,
							achPath,
							NULL,
							NULL,
							NULL,
							NULL,
							NULL);
			if( schService ) 
			{
				_tprintf(_T("%s installed.\n"), (LPCTSTR)m_szWinServerName );
				CloseServiceHandle(schService);
			} 
			else 
			{
				_tprintf(_T("CreateService failed\n"));
			}
			
			CloseServiceHandle(schSCManager);
		}
	}
}
//
void WinServer::UninstanceServer()
{
	SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if(schSCManager) 
	{
		SC_HANDLE schService =	OpenService(schSCManager, m_szWinServerName, SERVICE_ALL_ACCESS);		
		if( schService ) 
		{
			if( ControlService(schService, SERVICE_CONTROL_STOP, &_tServiceStatus) ) 
			{
				_tprintf(_T("Stopping %s."), (LPCTSTR)m_szWinServerName);
				Sleep(1000);
				
				while(QueryServiceStatus(schService, &_tServiceStatus)) 
				{
					if(_tServiceStatus.dwCurrentState == SERVICE_STOP_PENDING) 
					{
						_tprintf(_T("."));
						Sleep(1000);
					} 
					else
					{
						break;
					}
				}
				
				if( _tServiceStatus.dwCurrentState == SERVICE_STOPPED )
				{
					_tprintf(_T("\n%s stopped.\n"), m_szWinServerName);
				}
				else
				{
					_tprintf(_T("\n%s failed to stop.\n"), m_szWinServerName);
				}
			}
			
			// now remove the service
			if( DeleteService(schService) ) 
				_tprintf(_T("%s removed.\n"), m_szWinServerName); 
			else 
				_tprintf(_T("DeleteService failed\n"));			
			CloseServiceHandle(schService);
		} 
		else 
		{
			_tprintf(_T("OpenService failed \n"));
		}
		//CloseServiceHandle(schSCManager);
	} 
	else 
	{
		_tprintf(_T("OpenSCManager failed\n"));
	}
}

//
void WinServer::RunService()
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	schSCManager = ::OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if( schSCManager ) 
	{
		schService = ::OpenService(schSCManager,m_szWinServerName,SERVICE_ALL_ACCESS);
		
		if( schService ) 
		{
			// try to start the service
			_tprintf(_T("Starting up %s."), m_szWinServerName);
			if (m_szLauchAppName != NULL)
			{
				if (GetFindKillProcessInstance()->FindProcess(m_szLauchAppName))
				{
					_tprintf(_T("\nFind prev App process.\n"));
					if (GetFindKillProcessInstance()->KillProcess(m_szLauchAppName, TRUE))
					{
						::Sleep(500);
						_tprintf(_T("\nKill prev App process success.\n"));
					}
					else
						_tprintf(_T("\nKill prev App process failed.\n"));
				}
				
				if (LaunchApp(m_szLauchAppName, _T(" /start")))
					_tprintf(_T("\nRun new App process success.\n"));
				else
				_tprintf(_T("\nRun new App process failed.\n"));
			}

			::CloseServiceHandle(schService);
		} 
		else 
		{
			_tprintf(_T("OpenService failed\n"));
		}

		::CloseServiceHandle(schSCManager);
	} 
	else 
	{
		_tprintf(_T("OpenSCManager failed\n"));
	}	
}

//
void WinServer::StopService()
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	schSCManager = ::OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if( schSCManager ) 
	{
		schService = ::OpenService(schSCManager,m_szWinServerName, SERVICE_ALL_ACCESS);
		if( schService ) 
		{
			if (m_szLauchAppName != NULL)
			{
				if (GetFindKillProcessInstance()->FindProcess(m_szLauchAppName))
				{
					if (GetFindKillProcessInstance()->KillProcess(m_szLauchAppName, TRUE))
						_tprintf(_T("\nKill App process success.\n"));
					else
						_tprintf(_T("\nKill App process failed.\n"));
				}
			}
			::CloseServiceHandle(schService);
		} 
		else 
		{			
			_tprintf(_T("OpenService failed \n"));
		}
		::CloseServiceHandle(schSCManager);
	} 
	else 
	{
		_tprintf(_T("OpenSCManager failed \n"));
	}
}
//
BOOL WinServer::ReportStatus(DWORD dwCurrentState, DWORD dwWaitHint,DWORD dwErrExit)
{
	_tServiceStatus.dwServiceType        = SERVICE_WIN32; 
    _tServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_SHUTDOWN|SERVICE_ACCEPT_STOP; 
    _tServiceStatus.dwWin32ExitCode      = 0; 
    _tServiceStatus.dwServiceSpecificExitCode = 0; 
    _tServiceStatus.dwCheckPoint         = 0; 
    _tServiceStatus.dwWaitHint           = 0; 
	_tServiceStatus.dwCurrentState       = dwCurrentState; 
    _tServiceStatus.dwCheckPoint         = 0; 
    _tServiceStatus.dwWaitHint           = 0;
	SetServiceStatus (_hServiceStatus, &_tServiceStatus);
	return TRUE;

}

// Launches App process
BOOL WinServer::LaunchApp(LPCTSTR szAppName, LPTSTR szCmdLineParams)
{
    /* Create App process */
	
    STARTUPINFO si;
    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
	
    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(PROCESS_INFORMATION));    
	
    BOOL bCreateProcess = CreateProcess(
        szAppName, szCmdLineParams, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
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

BOOL WinServer::KillProcess(LPCTSTR szExeName)
{
	HANDLE hThread;  	
    hThread=CreateThread(NULL, 0, KillProcessProc, (LPVOID)szExeName, 0, NULL);  
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);  
	return TRUE;
}
*/

CFindKillProcess* WinServer::GetFindKillProcessInstance()
{
	if (m_pFindKillProcess == NULL)
	{
		m_pFindKillProcess = new CFindKillProcess();
	}
	return m_pFindKillProcess;
}