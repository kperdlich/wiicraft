#pragma once

#include "Mutex.h"

namespace core {

class lock_guard
{
public:
	explicit lock_guard(Mutex& mutex);
	~lock_guard();
	lock_guard(const lock_guard&) = delete;
	lock_guard(lock_guard&&) = delete;
	void operator=(const lock_guard&) = delete;
	void operator=(lock_guard&&) = delete;

	void Release();
	void Lock();
private:
	Mutex& m_mutex;
	bool m_bReleased;
};
}
