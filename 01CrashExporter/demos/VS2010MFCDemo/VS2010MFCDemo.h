
// VS2010MFCDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVS2010MFCDemoApp:
// �йش����ʵ�֣������ VS2010MFCDemo.cpp
//

class CVS2010MFCDemoApp : public CWinApp
{
public:
	CVS2010MFCDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVS2010MFCDemoApp theApp;