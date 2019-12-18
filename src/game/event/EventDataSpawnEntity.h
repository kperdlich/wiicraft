#pragma once

#include <memory>
#include "eventmanager.h"
#include "vector3f.h"
#include "EntityManager.h"

namespace wiicraft {

class EventDataSpawnEntity : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0x5f39a9c4;
    EventDataSpawnEntity(std::shared_ptr<wiicraft::Entity> entity);
    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;

    inline std::shared_ptr<wiicraft::Entity> GetEntity();

private:
    std::shared_ptr<wiicraft::Entity> mEntity;
};

inline std::shared_ptr<Entity> EventDataSpawnEntity::GetEntity()
{
    return mEntity;
}

};

