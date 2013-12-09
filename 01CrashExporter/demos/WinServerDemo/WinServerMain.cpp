
#include <conio.h>
#include <tchar.h>
#include <WINDOWS.H>
#include "WinServer.h"

int main(int argc, char* argv[])
{
	if(argc < 2)
		return 1; // No arguments passed, exit.

	WINSERVER_INFO info;
	info.cb = sizeof(WINSERVER_INFO);
	info.pszWinServerName = _T("TestWinServer");
	info.pszLauchAppName =  _T("OspMFCDemoApp.exe");
	info.pszLauchAppCmdLine = _T("/start");
	info.pszLauchAppCmdLine = _T("/stop");

	WinServer::Instance()->Init(&info);
	DWORD dwCode = CODE_NONE;

	if (_tcscmp(argv[1], _T("-i"))==0)
		dwCode = CODE_INSTANCE;
	else if (_tcscmp(argv[1], _T("-u"))==0)
		dwCode = CODE_UNINSTANCE;
	else if (_tcscmp(argv[1], _T("-r"))==0)
		dwCode = CODE_START;
	else if (_tcscmp(argv[1], _T("-s"))==0)
		dwCode = CODE_STOP;
	else
		return 2;	//invaid arguments

	WinServer::Instance()->Run(dwCode);
	return 0;
}