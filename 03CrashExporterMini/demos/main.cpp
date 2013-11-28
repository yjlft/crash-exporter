

#include "..\include\CrashExporterMini.h"

CCrashExporterMini g_CrashExporterMini;
DWORD WINAPI ThreadProc( LPVOID lpParam )
{
	char* pcTemp = NULL;
	pcTemp[ 0 ] = 'a';
	return 0;
}

void main(){	
	DWORD idThread;
	HANDLE hThread = NULL;
	hThread = ::CreateThread( NULL, 0, ThreadProc, NULL, 0, &idThread );

	::WaitForSingleObject(hThread, INFINITE);
	
}
