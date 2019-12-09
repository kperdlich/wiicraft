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

#include <gccore.h>

namespace core {
class Mutex
{
public:
	Mutex(const Mutex&) = delete;
	Mutex(Mutex&&) = delete;
	void operator=(const Mutex&) = delete;
	void operator=(Mutex&&) = delete;

	Mutex()
	{
		LWP_MutexInit(&m_mutex, true);
	}

	~Mutex()
	{
		LWP_MutexDestroy(m_mutex);
	}

	friend class lock_guard;

private:
	mutex_t m_mutex;
};
}
