

#if !defined(_CDEMOINSTANCE_INCLUDED_)
#define _CDEMOINSTANCE_INCLUDED_

#include "osp.h"

#define EVENT_BASE OSP_USEREVENT_BASE + 100

typedef enum _eEventType
{
	eEventSEH = EVENT_BASE + 1,
	eEventRaiseException = EVENT_BASE + 2,
	eEventThrow = EVENT_BASE + 3
};

class CDemoInstance;
typedef zTemplate<CDemoInstance> CDemoApp;

class CDemoInstance : public CInstance  
{
public:
	CDemoInstance();
	virtual ~CDemoInstance();

	virtual void InstanceEntry( CMessage *const pMsg);

	virtual void DaemonInstanceEntry( CMessage *const pMsg, CApp* pApp);	/*守护实例入口函数*/
	
	static CDemoApp *GetDemoInstance();
	static CDemoApp *m_pDemoInstance;

};


#endif
