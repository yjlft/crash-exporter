// VC6MFCDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VC6MFCDemo.h"
#include "VC6MFCDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVC6MFCDemoDlg dialog

CVC6MFCDemoDlg::CVC6MFCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVC6MFCDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVC6MFCDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVC6MFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVC6MFCDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVC6MFCDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CVC6MFCDemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnCrash, OnbtnCrash)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVC6MFCDemoDlg message handlers

BOOL CVC6MFCDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVC6MFCDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CVC6MFCDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD WINAPI ThreadProc( LPVOID /*lpParam*/ )
{
	int *p = 0;
	*p = 0; // Access violation
	return 0;
}

void CVC6MFCDemoDlg::OnbtnCrash() 
{
	
#if 1
	int *p = 0;
	*p = 0; // Access violation
#else
	DWORD idThread;
	HANDLE hThread;
	hThread = ::CreateThread( NULL, 0, ThreadProc, NULL, 0, &idThread );
	::WaitForSingleObject(hThread, INFINITE);
#endif
}

