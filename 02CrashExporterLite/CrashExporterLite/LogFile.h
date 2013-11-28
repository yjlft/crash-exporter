#ifndef _LOGFILE_H
#define _LOGFILE_H

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>

class LogFile
{
protected:
	
	CRITICAL_SECTION _csLock;
	char * _szFileName;
	HANDLE _hFile;
	
	bool OpenFile()		//Open the file,pointer to the end of the file
	{
		if(IsOpen())
			return true;
		
		if(!_szFileName)
			return false;
		
		WCHAR wszFileName[64];
		memset(wszFileName, 0, sizeof(wszFileName));
		MultiByteToWideChar(CP_ACP, 0, _szFileName, strlen(_szFileName)+1, wszFileName,
			sizeof(wszFileName)/sizeof(wszFileName[0]));

		_hFile =  CreateFile(
			wszFileName, 
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL 
			);
		
		//Open failed.if file not exist, create new
		if(!IsOpen() && GetLastError() == 2)
			_hFile =  CreateFile(
			wszFileName, 
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL 
			); 
		
		if(IsOpen())
			SetFilePointer(_hFile, 0, NULL, FILE_END);
		
		return IsOpen();
	}
	
	DWORD Write(LPCVOID lpBuffer, DWORD dwLength)
	{
		DWORD dwWriteLength = 0;
		
		if(IsOpen())
			WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);
		
		return dwWriteLength;
	}
	
	//you can modify yourself
	virtual void WriteLog( LPCVOID lpBuffer, DWORD dwLength)
	{
		DWORD dwWriteLength = 0;
		
		if(IsOpen())
		{
			WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);
			FlushFileBuffers(_hFile);
		}		
	}
	
	void Lock()  { ::EnterCriticalSection(&_csLock); }
	void Unlock() { ::LeaveCriticalSection(&_csLock); }
	
public:
	
	//set your LogFile name
	LogFile(const char *szFileName = "Log.log")
	{
		_szFileName = NULL;
		_hFile = INVALID_HANDLE_VALUE;
		::InitializeCriticalSection(&_csLock);
		
		SetFileName(szFileName);
	}
	
	virtual ~LogFile()
	{
		::DeleteCriticalSection(&_csLock);
		Close();
		
		if(_szFileName)
			delete []_szFileName;
	}
	
	const char * GetFileName()
	{
		return _szFileName;
	}
	
	//modify FileName,this will close the before one
	void SetFileName(const char *szName)
	{
		assert(szName);
		
		if(_szFileName)
			delete []_szFileName;
		
		Close();
		
		_szFileName = new char[strlen(szName) + 1];
		assert(_szFileName);
		strcpy(_szFileName, szName);
	}
	
	bool IsOpen()
	{
		return _hFile != INVALID_HANDLE_VALUE;
	}
	
	void Close()
	{
		if(IsOpen())
		{
			CloseHandle(_hFile);
			_hFile = INVALID_HANDLE_VALUE;
		}
	}
	
	//this will add to the end of the file
	void Log(LPCVOID lpBuffer, DWORD dwLength)
	{
		assert(lpBuffer);
		__try{
			Lock();
			
			if(!OpenFile())  return;
			
			WriteLog(lpBuffer, dwLength);
		}
		__finally{
			Unlock();
		} 
	}
	
	void Log(const char *szText)
	{
		Log(szText, strlen(szText));
	}
	
private:
	
	LogFile(const LogFile&);
	LogFile&operator = (const LogFile&);
};


#endif
