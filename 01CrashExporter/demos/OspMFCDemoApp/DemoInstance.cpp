// ServerInstance.cpp: implementation of the CServerInstance class.
//
//////////////////////////////////////////////////////////////////////

#include "DemoInstance.h"
#include "CrashRpt for vc6.0.h"



CDemoInstance::CDemoInstance(){}

CDemoInstance::
~CDemoInstance(){}

void CDemoInstance::InstanceEntry( CMessage *const pMsg) {}



CDemoApp* CDemoInstance::m_pDemoInstance = NULL;

CDemoApp* CDemoInstance::GetDemoInstance()
{
	if (!m_pDemoInstance)
	{
		m_pDemoInstance = new CDemoApp();
	}
	return m_pDemoInstance;
}



void CDemoInstance::DaemonInstanceEntry( CMessage *const pMsg, CApp* pApp) 
{
	u16 wCurEvent = pMsg->event;
	switch (wCurEvent)
	{		
	case eEventSEH:
		{
			// Access violation
			int *p = 0;
			*p = 0;
		}
		break;
	case eEventRaiseException:
		{
			// Raise noncontinuable software exception
			RaiseException(123, EXCEPTION_NONCONTINUABLE, 0, NULL);    
		}
		break;
	case eEventThrow:
		{
			// Throw typed C++ exception.
			throw 13;
		}
		break;
	case OSP_DISCONNECT:												
		break;
		
	default:
		::OspPrintf(TRUE, FALSE, "DaemonInstanceEntry pMsg->event not exist\n");
		break;
	}
}