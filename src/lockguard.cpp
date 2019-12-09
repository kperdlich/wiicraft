#include <gccore.h>
#include "lockguard.h"

core::lock_guard::lock_guard(Mutex& mutex) : m_mutex(mutex), m_bReleased(true)
{
	Lock();
}

core::lock_guard::~lock_guard()
{
	Release();
}

void core::lock_guard::Release()
{
	if (!m_bReleased)
	{
		LWP_MutexUnlock(m_mutex.m_mutex);
		m_bReleased = true;
	}
}

void core::lock_guard::Lock()
{
	if (m_bReleased)
	{
		LWP_MutexLock(m_mutex.m_mutex);
		m_bReleased = false;
	}
}
