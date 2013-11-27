/************************************************************************************* 
This file is a part of CrashRpt library.
Copyright (c) 2003-2013 The CrashRpt project authors. All Rights Reserved.

Use of this source code is governed by a BSD-style license
that can be found in the License.txt file in the root of the source
tree. All contributing project authors may
be found in the Authors.txt file in the root of the source tree.
***************************************************************************************/

// File: CrashSender.cpp
// Description: Entry point to the application. 
// Authors: zexspectrum
// Date: 2010

#include "stdafx.h"
#include "resource.h"
#include "ErrorReportSender.h"
#include "CrashInfoReader.h"
#include "strconv.h"
#include "Utility.h"
#include "ExportDlg.h"

CAppModule _Module;             // WTL's application module.

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int /*nCmdShow*/ = SW_SHOWDEFAULT)
{ 
	int nRet = 0; // Return code

	// Get command line parameters.
	LPCWSTR szCommandLine = GetCommandLineW();

	// Split command line.
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(szCommandLine, &argc);

	// Check parameter count.
	if(argc!=2)
		return 1; // No arguments passed, exit.

	if(_tcscmp(argv[1], _T("/terminate"))==0)
	{
		// User wants us to find and terminate all instances of CrashSender.exe
		return CErrorReportSender::TerminateAllCrashSenderProcesses();
	}

	// Extract file mapping name from command line arg.    
	CString sFileMappingName = CString(argv[1]);
		
	// Create the sender model that will collect crash report data 
	// and send error report(s).
	CErrorReportSender* pSender = CErrorReportSender::GetInstance();

	// Init the sender object
	BOOL bInit = pSender->Init(sFileMappingName.GetBuffer(0));
	if(!bInit)
	{
		// Failed to init 
		delete pSender;
		return 0;
	}      

	// Determine what to do next 
	// (either run in GUI more or run in silent mode).
	if(pSender->GetCrashInfo()->m_bGUIMode)
	{
		CExportDlg dlgDetail;
		dlgDetail.DoModal();
	}
	else
	{
		//::MessageBox(NULL, _T("slient mode"), _T("CrashSender"), MB_OK);
		time_t now;
		char chfilename[64] = {0};

		time(&now);
		strftime(chfilename, sizeof(chfilename), "crashrpt_%Y-%m-%d_%H-%M-%S.zip", localtime(&now));
		CString sfilename(chfilename);
		pSender->ExportReport(sfilename);
	}
	
	// Delete sender object.
	delete pSender;

	// Exit.
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{  
	//::MessageBox(NULL, _T("_tWinMain"), _T("info"), MB_OK);
	HRESULT hRes = ::CoInitialize(NULL);
	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

