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

#pragma once

#include "Thread.h"
#include "SafeQueue.h"

namespace core {

template<class T>
class Job : public Thread
{
public:
	Job() {}
	virtual ~Job() {}
	Job(const Job&) = delete;
	Job(Job&&) = delete;
	void operator=(const Job&) = delete;
	void operator=(Job&&) = delete;

	void Add(const T& data)
	{
		m_queue.Push(data);
		if (IsSuspended())
		{
			Resume();
		}
	}
    uint32_t GetQueueCount()
	{
		return m_queue.GetCount();
	}

protected:
	virtual void PreExecute() override
	{
		while (true)
		{
			if (IsStopped())
				break;

			if (m_queue.IsEmpty())
			{
				Suspend();
			}
			else
			{
				Execute();
			}
		}
	}

protected:
	SafeQueue<T> m_queue;
};

}
