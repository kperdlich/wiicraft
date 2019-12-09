/***
 *
 * Copyright (C) 2017 DaeFennek
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

#include <queue>
#include <ogcsys.h>
#include <gccore.h>
#include "Mutex.h"
#include "lockguard.h"

namespace core {
template<class T>
class SafeQueue
{
public:
	SafeQueue() {}
	SafeQueue(const SafeQueue&) = delete;
	SafeQueue(SafeQueue&&) = delete;
	void operator=(const SafeQueue&) = delete;
	void operator=(SafeQueue&&) = delete;

	T Pop()
	{
		lock_guard guard(m_mutex);
		T val = m_queue.front();
		m_queue.pop();
		return val;
	}

	void Push(const T& value)
	{
		lock_guard guard(m_mutex);
		m_queue.push(value);
	}

	bool IsEmpty()
	{
		lock_guard guard(m_mutex);
		bool bIsEmpty = m_queue.empty();
		return bIsEmpty;
	}

	uint32_t GetCount()
	{
		lock_guard guard(m_mutex);
		uint32_t count = m_queue.size();
		return count;
	}

	Mutex& GetMutex()
	{
		return m_mutex;
	}

private:
	std::queue<T> m_queue;
	Mutex m_mutex;
};
}
