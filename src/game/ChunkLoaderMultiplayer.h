#pragma once

#include "ChunkSection.h"
#include "Job.h"
#include <memory>

namespace wiicraft
{

    class ChunkLoaderMultiplayer : public core::Job<std::shared_ptr<ChunkSection>>
    {
    protected:
        void Execute() override;
    };

} // namespace wiicraft
