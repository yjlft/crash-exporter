/************************************************************************************* 
This file is a part of CrashRpt library.
Copyright (c) 2003-2013 The CrashRpt project authors. All Rights Reserved.

Use of this source code is governed by a BSD-style license
that can be found in the License.txt file in the root of the source
tree. All contributing project authors may
be found in the Authors.txt file in the root of the source tree.
***************************************************************************************/

/*! \file  ErrorReportExporter.h
*  \brief  Export Crash Error information, eg. minidump, stack walk, Screenshot...
*/

#pragma once
#include "AssyncNotification.h"
#include "tinyxml.h"
#include "CrashInfoReader.h"

// Action type
enum ActionType  
{
	COLLECT_CRASH_INFO  = 0x01, // Crash info should be collected.
	COMPRESS_REPORT     = 0x02, // Error report files should be packed into ZIP archive.
	RESTART_APP         = 0x04, // Crashed app should be restarted.
	SEND_REPORT         = 0x08, // Report should be sent.
	SEND_RECENT_REPORTS = 0x10  // Recent crash reports should be sent.
};

// Messages sent to GUI buy the sender
#define WM_NEXT_ITEM_HINT      (WM_USER+1023)
#define WM_ITEM_STATUS_CHANGED (WM_USER+1024)
#define WM_DELIVERY_COMPLETE   (WM_USER+1025)

// The main class that collects crash report files, packs them 
// into a ZIP archive and sends the error report.
class CErrorReportExporter
{
public:

	// Constructor.
	CErrorReportExporter();

	// Destructor.
	virtual ~CErrorReportExporter();

	// Returns singleton of this class.
	static CErrorReportExporter* GetInstance();

	// Performs initialization.	
	BOOL Init(LPCTSTR szFileMappingName);
		
	// Cleans up all temp files and does other finalizing work.
	BOOL Finalize();

	// Returns pointer to object containing crash information.
	CCrashInfoReader* GetCrashInfo();
	
	// Returns last error message.
	CString GetErrorMsg();

	// Set the window that will receive notifications from this object.
	void SetNotificationWindow(HWND hWnd);

	// Blocks until an assync operation finishes.
	void WaitForCompletion();

	// Cancels the assync operation.
	void Cancel();

	// Gets current operation status.
	void GetCurOpStatus(int& nProgressPct, std::vector<CString>& msg_log);
		
	// Unblocks waiting worker thread.
	void FeedbackReady(int code);
		
	// Returns current error report's index.
	int GetCurReport();
		
	// Returns path to log file.
	CString GetLogFilePath();

	// Allows to specify file name for exporting error report.
	void SetExportFlag(BOOL bExport, CString sExportFile);

	// Exports crash report to disc as a ZIP archive.
	void ExportReport(LPCTSTR szOutFileName);
	
	// This method finds and terminates all instances of crashExporter.exe process.
	static int TerminateAllcrashExporterProcesses();
		
private:

	// This method performs an action or several actions.
	BOOL DoWork(int Action);
		
	// Worker thread proc.
	static DWORD WINAPI WorkerThread(LPVOID lpParam);  

	// Runs an action or several actions in assync mode.
	BOOL DoWorkAssync(int Action);

	// Collects crash report files.
	BOOL CollectCrashFiles();  

	// Includes a single file to crash report
	BOOL CollectSingleFile(ERIFileItem* pfi);

	// Includes all files matching search pattern to crash report
	BOOL CollectFilesBySearchTemplate(ERIFileItem* pfi, std::vector<ERIFileItem>& file_list);
		
	// Takes desktop screenshot.
	BOOL TakeDesktopScreenshot();

	// Creates crash dump file.
	BOOL CreateMiniDump();  

	//+ Create StackWalker Info.
	BOOL CreateStackWalkerInfo();

	// This method is used to have the current process be able to call MiniDumpWriteDump.
	BOOL SetDumpPrivileges();

	// Creates crash description XML file.
	BOOL CreateCrashDescriptionXML(CErrorReportInfo& eri);
	
	// Adds an element to XML file.
	void AddElemToXML(CString sName, CString sValue, TiXmlNode* root);

	// Minidump callback.
	static BOOL CALLBACK MiniDumpCallback(PVOID CallbackParam, PMINIDUMP_CALLBACK_INPUT CallbackInput,
		PMINIDUMP_CALLBACK_OUTPUT CallbackOutput); 

	// Minidump callback.
	BOOL OnMinidumpProgress(const PMINIDUMP_CALLBACK_INPUT CallbackInput,
		PMINIDUMP_CALLBACK_OUTPUT CallbackOutput);

	// Restarts the application.
	BOOL RestartApp();

	// Packs error report files to ZIP archive.
	BOOL CompressReportFiles(CErrorReportInfo* eri);

	// Unblocks parent process.
	void UnblockParentProcess();
	
	// Internal variables
	static CErrorReportExporter* m_pInstance; // Singleton
	CCrashInfoReader m_CrashInfo;       // Contains crash information.
	CString m_sErrorMsg;                // Last error message.
	HWND m_hWndNotify;                  // Notification window.
	int m_nCurReport;                   // Index of current error report.
	HANDLE m_hThread;                   // Handle to the worker thread.
	AssyncNotification m_Assync;        // Used for communication with the main thread.
	CString m_sZipName;                 // Name of the ZIP archive to send.
	int m_Action;                       // Current assynchronous action.
	BOOL m_bExport;                     // If TRUE than export should be performed.
	CString m_sExportFileName;          // File name for exporting.
};


