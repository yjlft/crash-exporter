
// VS2010MFCDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VS2010MFCDemo.h"
#include "VS2010MFCDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVS2010MFCDemoDlg �Ի���




CVS2010MFCDemoDlg::CVS2010MFCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVS2010MFCDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVS2010MFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVS2010MFCDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnCrash, &CVS2010MFCDemoDlg::OnBnClickedbtncrash)
END_MESSAGE_MAP()


// CVS2010MFCDemoDlg ��Ϣ�������

BOOL CVS2010MFCDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVS2010MFCDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVS2010MFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD WINAPI ThreadProc( LPVOID /*lpParam*/ )
{
	int *p = 0;
	*p = 0; // Access violation
	return 0;
}

void CVS2010MFCDemoDlg::OnBnClickedbtncrash()
{

#if 0
	int *p = 0;
	*p = 0; // Access violation
#else
	DWORD idThread;
	HANDLE hThread;
	hThread = ::CreateThread( NULL, 0, ThreadProc, NULL, 0, &idThread );
	::WaitForSingleObject(hThread, INFINITE);
#endif
}
