// WinServer.h: interface for the WinServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_WINSERVER_INCLUDED_)
#define _WINSERVER_INCLUDED_

#define MAX_LEN 1024
typedef enum _eCmdCod
{
	CODE_NONE = 0,
	CODE_INSTANCE,
	CODE_UNINSTANCE,
	CODE_START,
	CODE_STOP,
}eCmdCode;

class CFindKillProcess;
class WinServer  
{
public:
	static WinServer* Instance();
	virtual ~WinServer();
public:	
	static void WINAPI ServiceMain(DWORD argc, LPTSTR * argv);
	static void WINAPI ServiceCtrl(DWORD dwCtrlCode);
	void Run(DWORD dwCmdCode = CODE_NONE);
	BOOL ReportStatus(DWORD dwCurrentState, DWORD dwWaitHint = 3000,DWORD dwErrExit = 0);
	void SetLauchAppName(LPCTSTR szAppName);
	void SetWinServerName(LPCTSTR szWinServerName);
public:

	//
	SERVICE_STATUS		  _tServiceStatus;
	SERVICE_STATUS_HANDLE _hServiceStatus;
	DWORD				  _dwCheckPoint;
private:
	WinServer(LPCTSTR strServerName);
private:
	void	InstanceServer();
	void	UninstanceServer();
	void	RunService();

	void	StopService();
	LPTSTR  GetLastErrorText(LPTSTR lpszBuf, DWORD dwSize) ;
	BOOL	LaunchApp(LPCTSTR szAppName, LPTSTR szCmdLineParams);
	//BOOL	KillProcess(LPCTSTR szExeName);
	CFindKillProcess* GetFindKillProcessInstance(); 

	CFindKillProcess* m_pFindKillProcess;

	static WinServer* m_ptrServer;
	LPCTSTR m_szLauchAppName;
	static LPCTSTR m_szWinServerName;
};

#endif // !defined(_WINSERVER_INCLUDED_)
