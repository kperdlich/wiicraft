#pragma once

#include "Mutex.h"

namespace core
{

    class Lockguard
    {
    public:
        explicit Lockguard(Mutex& mutex);
        ~Lockguard();
        Lockguard(const Lockguard&) = delete;
        Lockguard(Lockguard&&) = delete;
        void operator=(const Lockguard&) = delete;
        void operator=(Lockguard&&) = delete;

        void Release();
        void Lock();

    private:
        Mutex& m_mutex;
        bool m_bReleased;
    };
} // namespace core
