/***
 *
 * Copyright (C) 2018 DaeFennek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
***/

#include "Thread.h"
#include "lockguard.h"
#include "core.h"

int core::Thread::Create(ThreadEntryCallback entryCallback, void* stackbase, u32 stack_size, u8 prio)
{
	int32_t ret = LWP_CreateThread(&m_threadID, entryCallback, this, stackbase, stack_size, prio);
    ASSERT(ret >= 0);
	return ret;
}

void* core::Thread::ThreadEntry(void* args)
{
	static_cast<Thread*>(args)->PreExecute();
	return nullptr;
}


int core::Thread::Start()
{
	return Create(Thread::ThreadEntry, nullptr, 0, 128);
}

bool core::Thread::IsStopped()
{
	lock_guard guard(m_mutex);
	bool val = m_bStop;
	return val;
}

void core::Thread::Stop()
{
	lock_guard guard(m_mutex);
	m_bStop = true;
	guard.Release();

	if (IsSuspended())
	{
		Resume();
	}
	LWP_JoinThread(m_threadID, nullptr);

	guard.Lock();
	m_bStop = false;
}

bool core::Thread::IsSuspended()
{
	return LWP_ThreadIsSuspended(m_threadID);
}

void core::Thread::Resume()
{
	LWP_ResumeThread(m_threadID);
}

void core::Thread::Suspend()
{
	LWP_SuspendThread(m_threadID);
}
