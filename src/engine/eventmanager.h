#pragma once

#include <stdint.h>
#include <list>
#include <map>
#include <memory>
#include "fastdelegate.h"
#include "fastdelegatebind.h"
#include "SafeQueue.h"

namespace core {

// Event System from Game Coding Complete - 4th Edition

typedef uint64_t EventType;
typedef std::shared_ptr<class IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;
typedef SafeQueue<IEventDataPtr> ThreadSafeEventQueue;

class IEventData
{
public:
    virtual const EventType& GetEventType()  const = 0;
    virtual float GetTimeStamp() const = 0;
    virtual IEventDataPtr Copy() const = 0;
    virtual const char* GetName() const = 0;
};

class BaseEventData : public IEventData
{
public:
    explicit BaseEventData(const float timeStamp = 0.0f) :
        mTimeStamp(timeStamp) {}
    virtual ~BaseEventData() = default;
    float GetTimeStamp() const override { return mTimeStamp; }
private:
    const float mTimeStamp;
};

class IEventManager
{
public:
    static constexpr uint64_t INFINITE = 0xffffffff;

    explicit IEventManager(bool setAsGlobal);
    virtual ~IEventManager() = default;
    virtual bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;
    virtual bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;
    virtual bool TriggerEvent(const IEventDataPtr& event) = 0;
    virtual bool QueueEvent(const IEventDataPtr& event) = 0;
    virtual bool AbortEvent(const EventType& type, bool allOfType = false) = 0;
    virtual bool TickUpdate(uint64_t maxMs = INFINITE) = 0;
    virtual bool ThreadSafeQueueEvent (const IEventDataPtr& event) = 0;

    static IEventManager* Get();

private:
    static core::IEventManager* sGlobalEventManager;
};

constexpr int32_t EVENTMANAGER_NUM_QUEUE = 2;
class EventManager : public IEventManager
{
    typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<IEventDataPtr> EventQueue;

public:
    explicit EventManager(bool setAsGlobal);
    virtual ~EventManager() = default;

    virtual bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
    virtual bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);
    virtual bool TriggerEvent(const IEventDataPtr& event);
    virtual bool QueueEvent(const IEventDataPtr& event);
    virtual bool AbortEvent(const EventType& type, bool allOfType = false);
    virtual bool TickUpdate(uint64_t maxMs = IEventManager::INFINITE);
    virtual bool ThreadSafeQueueEvent (const IEventDataPtr& event);

private:
    EventListenerMap mEventListeners;
    EventQueue mQueues[EVENTMANAGER_NUM_QUEUE];
    ThreadSafeEventQueue mRealtimeEventQueue;
    int32_t mActiveQueue;
};

}
