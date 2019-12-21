#include "eventmanager.h"
#include "core.h"
#include "time_helper.h"

core::IEventManager* core::IEventManager::sGlobalEventManager = nullptr;

core::IEventManager::IEventManager(bool setAsGlobal)
{
    if (setAsGlobal)
    {
        ASSERT(!sGlobalEventManager);
        sGlobalEventManager = this;
    }
}

core::IEventManager *core::IEventManager::Get()
{
    return sGlobalEventManager;
}

core::EventManager::EventManager(bool setAsGlobal)
     : IEventManager(setAsGlobal),
       mActiveQueue(0)
{
}

bool core::EventManager::AddListener(const core::EventListenerDelegate& eventDelegate, const core::EventType& type)
{
    EventListenerList& eventListenerList = mEventListeners[type];
    for (const auto& existingDelegate : eventListenerList)
    {
        if (eventDelegate == existingDelegate)
        {
            // Double register a delegate
            return false;
        }
    }
    eventListenerList.push_back(eventDelegate);
    return true;
}

bool core::EventManager::RemoveListener(const core::EventListenerDelegate& eventDelegate, const core::EventType& type)
{
    auto findIt = mEventListeners.find(type);
    if (findIt != mEventListeners.end())
    {
        EventListenerList& listeners = findIt->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it)
        {
            if (eventDelegate == (*it))
            {
                listeners.erase(it);
                return true;
            }
        }
    }
    return false;
}

bool core::EventManager::TriggerEvent(const core::IEventDataPtr& event)
{
    bool process = false;
    auto findIt = mEventListeners.find(event->GetEventType());
    if (findIt != mEventListeners.end())
    {
        const EventListenerList& eventListenerList = findIt->second;
        for (const auto& it : eventListenerList)
        {
            EventListenerDelegate listener = it;
            listener(event);
            process = true;
        }
    }
    return process;
}

bool core::EventManager::QueueEvent(const core::IEventDataPtr& event)
{
   ASSERT(mActiveQueue >= 0);
   ASSERT(mActiveQueue < EVENTMANAGER_NUM_QUEUE);

   auto findIt = mEventListeners.find(event->GetEventType());
   if (findIt != mEventListeners.end())
   {
        mQueues[mActiveQueue].push_back(event);
        return true;
   }
   return false;
}

bool core::EventManager::AbortEvent(const core::EventType& type, bool allOfType)
{
    ASSERT(mActiveQueue >= 0);
    ASSERT(mActiveQueue < EVENTMANAGER_NUM_QUEUE);

    bool success = false;
    auto findIt = mEventListeners.find(type);
    if (findIt != mEventListeners.end())
    {
        EventQueue& eventQueue = mQueues[mActiveQueue];
        for (auto it = eventQueue.begin(); it != eventQueue.end();)
        {
            if ((*it)->GetEventType() == type)
            {
                it = eventQueue.erase(it);
                success = true;
                if (!allOfType)
                {
                    break;
                }
            }
            else
            {
                ++it;
            }
        }
    }
    return success;
}

bool core::EventManager::TickUpdate(uint64_t maxMs)
{
    uint64_t currentMs = core::GetMsCount();
    const uint64_t newMaxMs = maxMs == IEventManager::INFINITE ? IEventManager::INFINITE : (currentMs + maxMs);

    while (!mRealtimeEventQueue.IsEmpty())
    {
        IEventDataPtr realtimeEvent = mRealtimeEventQueue.Pop();
        QueueEvent(realtimeEvent);
        currentMs = core::GetMsCount();
        if (maxMs != IEventManager::INFINITE)
        {
            if (currentMs >= newMaxMs)
            {
                ASSERT(false); // Thread spammes event manager!
            }
        }
    }

    const int32_t queueToProcess = mActiveQueue;
    mActiveQueue = (mActiveQueue + 1) % EVENTMANAGER_NUM_QUEUE;
    mQueues[mActiveQueue].clear();

    while (!mQueues[queueToProcess].empty())
    {
        IEventDataPtr event = mQueues[queueToProcess].front();
        mQueues[queueToProcess].pop_front();

        const EventType& eventType = event->GetEventType();
        auto findIt = mEventListeners.find(eventType);
        if (findIt != mEventListeners.end())
        {
            const EventListenerList& eventListeners = findIt->second;
            for (const auto& listener : eventListeners)
            {
                listener(event);
            }
        }

        currentMs = core::GetMsCount();
        if (maxMs != IEventManager::INFINITE && currentMs >= newMaxMs)
        {
            break; // Time ran out
        }
    }

    bool queueFlushed = mQueues[queueToProcess].empty();
    if (!queueFlushed)
    {
        while (!mQueues[queueToProcess].empty())
        {
            IEventDataPtr event = mQueues[queueToProcess].back();
            mQueues[queueToProcess].pop_back();
            mQueues[mActiveQueue].push_front(event);
        }
    }

    return queueFlushed;
}

bool core::EventManager::ThreadSafeQueueEvent(const core::IEventDataPtr &event)
{
    mRealtimeEventQueue.Push(event);
    return true;
}
