
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "CrashRpt.h"

class CDerived;
class CBase
{
public:
	CBase(CDerived *derived): m_pDerived(derived) {};
	~CBase();
	virtual void function(void) = 0;

	CDerived * m_pDerived;
};

#pragma warning(disable:4355)
class CDerived : public CBase
{
public:
	CDerived() : CBase(this) {};   // C4355
	virtual void function(void) {};
};

CBase::~CBase()
{
	m_pDerived -> function();
}


void main()
{
	crInstall("VC6Demo", "VC6Demo");
	//crInstall();
	//CrAutoInstallHelper AutoInstallHelper;

	printf("Choose an exception type:\n");
	printf("0 - SEH exception\n");
	printf("12 - RaiseException\n");
	printf("13 - throw C++ typed exception\n");
	printf("Your choice >  ");
	
	int ExceptionType = 0;
	scanf("%d", &ExceptionType);
	
	switch(ExceptionType)
	{
	case 0: // SEH
		{
			// Access violation
			int *p = 0;
			*p = 0;
		}
		break;
	case 12: // RaiseException 
		{
			// Raise noncontinuable software exception
			RaiseException(123, EXCEPTION_NONCONTINUABLE, 0, NULL);        
		}
		break;
	case 13: // throw 
		{
			// Throw typed C++ exception.
			throw 13;
		}
		break;
	default:
		{
			printf("Unknown exception type specified.");       
			_getch();
		}
		break;
	}
	crUninstall();
}


