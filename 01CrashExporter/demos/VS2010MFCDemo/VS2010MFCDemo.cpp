
// VS2010MFCDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "VS2010MFCDemo.h"
#include "VS2010MFCDemoDlg.h"
#include "CrashRpt.h"
#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVS2010MFCDemoApp

BEGIN_MESSAGE_MAP(CVS2010MFCDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVS2010MFCDemoApp 构造

CVS2010MFCDemoApp::CVS2010MFCDemoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CVS2010MFCDemoApp 对象

CVS2010MFCDemoApp theApp;


// CVS2010MFCDemoApp 初始化

int CVS2010MFCDemoApp::Run() 
{
	// Call your crInstall code here ...

	CR_INSTALL_INFO info;
	// Install crash handlers
	int nInstResult = crInstall(&info);            
	assert(nInstResult==0);

	nInstResult = crAddScreenshot(CR_AS_MAIN_WINDOW);
	assert(nInstResult==0);

	// Check result
	if(nInstResult!=0)
	{
		TCHAR buff[256];
		crGetLastErrorMsg(buff, 256); // Get last error
		_tprintf(_T("%s\n"), buff); // and output it to the screen
		return FALSE;
	}

	BOOL bRun;
	BOOL bExit=FALSE;
	while(!bExit)
	{
		bRun= CWinApp::Run();
		bExit=TRUE;
	}

	// Uninstall crash reporting
	//crUninstall();
	return bRun;
}


BOOL CVS2010MFCDemoApp::InitInstance()
{

	CWinApp::InitInstance();
	Run();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CVS2010MFCDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

