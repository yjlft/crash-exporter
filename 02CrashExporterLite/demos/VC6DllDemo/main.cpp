#include "DllDemo.h"
#include <STDIO.H>
#include <windows.h>
#include <conio.h>

int main(void)
{
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
			SEH();
		}
		break;
	case 12: // RaiseException 
		{
			RaiseException();        
		}
		break;
	case 13: // throw 
		{
			ThrowException();
		}
		break;
	default:
		{
			Unknown();
			_getch();
		}
		break;
	}

	return 0;
}