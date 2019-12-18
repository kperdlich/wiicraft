#pragma once

#include <memory>
#include "Job.h"
#include "chunksection.h"

namespace wiicraft {

class ChunkLoaderMultiplayer : public core::Job<std::shared_ptr<ChunkSection>>
{
protected:
	void Execute() override;
};

}
