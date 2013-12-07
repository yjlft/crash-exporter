// OspMFCDemoApp.h : main header file for the OSPMFCDEMOAPP application
//

#if !defined(AFX_OSPMFCDEMOAPP_H__9575CF4E_E808_487A_BD70_F3B015CD711E__INCLUDED_)
#define AFX_OSPMFCDEMOAPP_H__9575CF4E_E808_487A_BD70_F3B015CD711E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COspMFCDemoAppApp:
// See OspMFCDemoApp.cpp for the implementation of this class
//

class COspMFCDemoAppApp : public CWinApp
{
public:
	COspMFCDemoAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COspMFCDemoAppApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void start();
	void stop();
	void restart();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COspMFCDemoAppApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSPMFCDEMOAPP_H__9575CF4E_E808_487A_BD70_F3B015CD711E__INCLUDED_)
