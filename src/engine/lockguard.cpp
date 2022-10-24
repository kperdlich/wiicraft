#include "Lockguard.h"
#include <gccore.h>

core::Lockguard::Lockguard(Mutex& mutex)
    : m_mutex(mutex)
    , m_bReleased(true)
{
    Lock();
}

core::Lockguard::~Lockguard()
{
    Release();
}

void core::Lockguard::Release()
{
    if (!m_bReleased)
    {
        LWP_MutexUnlock(m_mutex.m_mutex);
        m_bReleased = true;
    }
}

void core::Lockguard::Lock()
{
    if (m_bReleased)
    {
        LWP_MutexLock(m_mutex.m_mutex);
        m_bReleased = false;
    }
}
