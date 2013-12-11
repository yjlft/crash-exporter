// OspMFCDemoAppDlg.h : header file
//

#if !defined(AFX_OSPMFCDEMOAPPDLG_H__CAAF9DB1_0E7F_4FE6_B1BF_F1F8618A13F3__INCLUDED_)
#define AFX_OSPMFCDEMOAPPDLG_H__CAAF9DB1_0E7F_4FE6_B1BF_F1F8618A13F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COspMFCDemoAppDlg dialog

class COspMFCDemoAppDlg : public CDialog
{
// Construction
public:
	COspMFCDemoAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COspMFCDemoAppDlg)
	enum { IDD = IDD_OSPMFCDEMOAPP_DIALOG };
	CComboBox	m_cboExcType;
	CComboBox	m_cboThread;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COspMFCDemoAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COspMFCDemoAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnbtnCrash();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSPMFCDEMOAPPDLG_H__CAAF9DB1_0E7F_4FE6_B1BF_F1F8618A13F3__INCLUDED_)
