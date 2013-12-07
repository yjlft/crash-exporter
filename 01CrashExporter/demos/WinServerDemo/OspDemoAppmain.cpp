
#include "DemoInstance.h"
#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <tchar.h>
#include "CrashRpt for vc6.0.h"

#define  DEMO_APP_NO  10


void InitCrashExporter()
{
	CR_INSTALL_INFO info;
	// memset(&info, 0, sizeof(CR_INSTALL_INFO));
	// info.cb = sizeof(CR_INSTALL_INFO);             // Size of the structure
	// info.pszAppName = _T("CrashRpt Console Test"); // App name
	// info.pszAppVersion = _T("1.0.0");              // App version
	//info.pszRestartCmdLine = _T("/restart"); 
	info.dwFlags = info.dwFlags|CR_INST_AUTO_THREAD_HANDLERS;
	
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
}

void Init()
{
	InitCrashExporter();
	if (!::IsOspInitd())
	{
		::OspInit(TRUE, 2500);         								/* 初始化Osp, 在端口2500启动Telnet */
		u32 dwNode = ::OspCreateTcpNode(0, 6682);      				/* 在6682端口上创建本地监听结点 */
		if (dwNode == INVALID_SOCKET)
		{
			return;
		}
		CDemoInstance::GetDemoInstance()->CreateApp("DemoApp", DEMO_APP_NO, 100);
	}
}
void start()
{
	Init();
}

void stop()
{
	crUninstall();
	if (CDemoInstance::GetDemoInstance() != NULL)
	{
		CDemoInstance::GetDemoInstance()->QuitApp();
	}
	exit(1);
}

void restart()
{

}
int main(int argc, char* argv[])
{
	// Check parameter count.
	if(argc!=2)
		return 1; // No arguments passed, exit.

	if (_tcscmp(argv[1], _T("/start"))==0)
		start();
	else if (_tcscmp(argv[1], _T("/stop"))==0)
		stop();
	else if (_tcscmp(argv[1], _T("/restart"))==0)
		restart();
	else
		return 2;	//invaid arguments
	
	printf("Choose an exception type:\n");
	printf("0 - SEH exception\n");
	printf("12 - RaiseException\n");
	printf("13 - throw C++ typed exception\n");
	printf("Your choice >  ");
	
	int ExceptionType = 0;
	scanf("%d", &ExceptionType);
	
	u32 dstIID = MAKEIID(DEMO_APP_NO, CInstance::DAEMON);

	switch(ExceptionType)
	{
	case 0: // SEH
		{
			OspPost(dstIID, eEventSEH);
		}
		break;
	case 12: // RaiseException 
		{
			OspPost(dstIID, eEventRaiseException);    
		}
		break;
	case 13: // throw 
		{
			OspPost(dstIID, eEventThrow);
		}
		break;
	default:
		{
			printf("Unknown exception type specified.");       
			_getch();
		}
		break;
	}
	_getch();
	return 0;
}