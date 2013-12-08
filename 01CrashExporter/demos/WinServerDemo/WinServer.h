// WinServer.h: interface for the WinServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_WINSERVER_INCLUDED_)
#define _WINSERVER_INCLUDED_

#define MAX_LEN 1024
typedef enum
{
	CODE_NONE = 0,
	CODE_INSTANCE,
	CODE_UNINSTANCE,
	CODE_START,
	CODE_STOP,
}WINSERVER_CMDCODE;


typedef enum
{
	NOT_LAUCH_APP = 0x1000,			// Lauch APP while run service
	NOT_KILL_PRELAUCHED_APP,		// Kill APP process if exist while run service
	NOT_KILL_LAUCHED_APP,			// Kill APP process if exist while stop service
}WINSERVER_INFO_Flags;

typedef struct tagWINSERVER_INFO
{
	WORD	cb;							// Size of this structure in bytes; must be initialized before using!
	LPCTSTR pszWinServerName;			// Name of winserver application.
	LPCTSTR pszLauchAppName;			// Name of Lauch application.
	LPCTSTR pszLauchAppCmdLine;			// CmdLine of Lauch application.
	DWORD	dwFlags;					// Flags.


	tagWINSERVER_INFO()
	{
		cb = 0;
		pszWinServerName = NULL;
		pszLauchAppName = NULL;
		pszLauchAppCmdLine = NULL;
		dwFlags = 0;
	}
}
WINSERVER_INFO;

typedef WINSERVER_INFO* PWINSERVER_INFO;


class CFindKillProcess;
class WinServer  
{
public:
	static					WinServer* Instance();
	virtual					~WinServer();
public:	
	static void WINAPI		ServiceMain(DWORD argc, LPTSTR * argv);
	static void WINAPI		ServiceCtrl(DWORD dwCtrlCode);
	void					Run(DWORD dwCmdCode = CODE_NONE);
	int						Init(PWINSERVER_INFO pInfo);
	BOOL					ReportStatus(DWORD dwCurrentState, DWORD dwWaitHint = 3000,DWORD dwErrExit = 0);
public:

	//
	SERVICE_STATUS			m_ServiceStatus;
	SERVICE_STATUS_HANDLE	m_hServiceStatus;
private:
	WinServer();
private:
	void					InstanceServer();
	void					UninstanceServer();
	void					RunService();

	void					StopService();
	LPTSTR					GetLastErrorText(LPTSTR pszBuf, DWORD dwSize) ;
	BOOL					LaunchApp(LPCTSTR pszAppName, LPTSTR pszCmdLineParams);
	int						KillLauchedApp();
	//BOOL					KillProcess(LPCTSTR pszExeName);
	CFindKillProcess*		GetFindKillProcessInstance(); 

	CFindKillProcess*		m_pFindKillProcess;

	static WinServer*		m_pWinServer;
	LPCTSTR					m_pszLauchAppName;
	LPCTSTR					m_pszWinServerName;
	LPCTSTR					m_pszLauchAppCmdLine;
	BOOL					m_bLauchApp;
	BOOL					m_bkillAppBeforeLauch;
	BOOL					m_bKillAppAfterStopService;
};

#endif // !defined(_WINSERVER_INCLUDED_)
