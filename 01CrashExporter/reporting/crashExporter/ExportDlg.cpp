#include "StdAfx.h"
#include "ExportDlg.h"
#include "Utility.h"
#include "CrashInfoReader.h"
#include "ErrorReportExporter.h"
#include "strconv.h"



LRESULT CExportDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CExportDlg::OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CExportDlg::OnExport(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// This method is called when user clicks the "Export" button. 
	// We should export crash report contents as a ZIP archive to
	// user-specified folder.

	CErrorReportExporter* pSender = CErrorReportExporter::GetInstance();
	// Format file name for the output ZIP archive.
	CString sFileName = _T("");//pCrashInfo->GetReport(m_nCurReport)->GetCrashGUID() + _T(".zip");

	// Display "Save File" dialog.
	CFileDialog dlg(FALSE, _T("*.zip"), sFileName,
		OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT,
		_T("ZIP Files (*.zip)\0*.zip\0All Files (*.*)\0*.*\0\0"), m_hWnd);

	INT_PTR result = dlg.DoModal();
	if(result==IDOK)
	{
		// Determine what destination user chosen
		CString sExportFileName = dlg.m_szFileName;

		// Export crash report assynchronously
		pSender->ExportReport(sExportFileName);
	}
	EndDialog(wID);
	return 0;
}
