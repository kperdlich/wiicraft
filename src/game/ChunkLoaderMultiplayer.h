#pragma once

#include "Job.h"
#include "chunksection.h"

namespace wiicraft {

class ChunkLoaderMultiplayer : public core::Job<ChunkSection*>
{
protected:
	void Execute() override;
};

}
