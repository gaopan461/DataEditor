#ifndef _AC_THREAD_GUARD_H_
#define _AC_THREAD_GUARD_H_

#include "ACDef.h"

BEGIN_NS_AC

/*
 *	»¥³âËø
 */
class ThreadGuard
{
public:
	ThreadGuard(long* m)
	{
		m_nMutex = m;
		while(::InterlockedExchange(m_nMutex,1))
		{
			//wait for unlock;
			::Sleep(0);
		}
	}
	~ThreadGuard()
	{
		::InterlockedExchange(m_nMutex,0);
	}
private:
	long* m_nMutex;
};

END_NS_AC

#endif //_AC_THREAD_GUARD_H_