// OspMFCDemoAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OspMFCDemoApp.h"
#include "OspMFCDemoAppDlg.h"
#include "DemoInstance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  DEMO_APP_NO  10

/////////////////////////////////////////////////////////////////////////////
// COspMFCDemoAppDlg dialog

COspMFCDemoAppDlg::COspMFCDemoAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COspMFCDemoAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COspMFCDemoAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COspMFCDemoAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COspMFCDemoAppDlg)
	DDX_Control(pDX, IDC_EXCTYPE, m_cboExcType);
	DDX_Control(pDX, IDC_THREAD, m_cboThread);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COspMFCDemoAppDlg, CDialog)
	//{{AFX_MSG_MAP(COspMFCDemoAppDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCrash, OnbtnCrash) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COspMFCDemoAppDlg message handlers

void InitOsp()
{
	if (!::IsOspInitd())
	{
		::OspInit(TRUE, 2500);         								/* 初始化Osp, 在端口2500启动Telnet */
		u32 dwNode = ::OspCreateTcpNode(0, 6682);      				/* 在6682端口上创建本地监听结点 */
		if (dwNode == INVALID_SOCKET)
		{
			return;
		}
		CDemoInstance::GetDemoInstance()->CreateApp("OspMFCDemoApp", DEMO_APP_NO, 100);
	}
}
void UnInitOsp()
{
	if (CDemoInstance::GetDemoInstance() != NULL)
	{
		CDemoInstance::GetDemoInstance()->QuitApp();
	}
}

BOOL COspMFCDemoAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here.
	
	InitOsp();
	
	m_cboThread.InsertString(0, _T("Main thread"));
	m_cboThread.InsertString(1, _T("Worker thread"));
	m_cboThread.SetCurSel(0);

	m_cboExcType.InsertString(0, _T("SEH"));
	m_cboExcType.InsertString(1, _T("RaiseException"));
	m_cboExcType.InsertString(2, _T("ThrowException"));
	m_cboExcType.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COspMFCDemoAppDlg::OnPaint() 
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

HCURSOR COspMFCDemoAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void OspPostException(int nExceptionType)
{
	u32 dstIID = MAKEIID(DEMO_APP_NO, CInstance::DAEMON);
	
	switch(nExceptionType)
	{
	case 0: // SEH
		{
			OspPost(dstIID, eEventSEH);
		}
		break;
	case 1: // RaiseException 
		{
			OspPost(dstIID, eEventRaiseException);    
		}
		break;
	case 2: // throw 
		{
			OspPost(dstIID, eEventThrow);
		}
		break;
	default:
		{
		//	printf("Unknown exception type specified.");       
		}
		break;
	}
}

DWORD WINAPI ThreadProc( LPVOID lpParam )
{
	OspPostException(*(int*)lpParam);
	return 0;
}

void COspMFCDemoAppDlg::OnbtnCrash() 
{
	int nThreadType = m_cboThread.GetCurSel();	
    int nExceptionType = m_cboExcType.GetCurSel();
	
    if(nThreadType==0) // The main thread
    {    
		OspPostException(nExceptionType);
    }
    else // Worker thread
    {
		DWORD idThread;
 		HANDLE hThread;
		hThread = ::CreateThread( NULL, 0, ThreadProc, &nExceptionType, 0, &idThread );
		::WaitForSingleObject(hThread, INFINITE);
    }
	
}

void COspMFCDemoAppDlg::OnClose()
{
	UnInitOsp();
	
	CDialog::OnClose();
}
