
// VS2010MFCDemo.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CVS2010MFCDemoApp:
// 有关此类的实现，请参阅 VS2010MFCDemo.cpp
//

class CVS2010MFCDemoApp : public CWinApp
{
public:
	CVS2010MFCDemoApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CVS2010MFCDemoApp theApp;