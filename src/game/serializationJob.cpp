#include <sstream>
#include "fstream"
#include "serializationJob.h"
#include "core.h"
#include "eventmanager.h"
#include "EventDataAllChunksInQueueSerialized.h"
#include "globals.h"

void wiicraft::SerializationJob::Execute()
{
    const CompressedChunkData& chunkData = m_queue.Pop();

	if (chunkData.m_CompressedData[0] != 0x78 ||
		chunkData.m_CompressedData[1] != 0x9c ||
		chunkData.m_CompressedData[2] != 0xed)
	{
        //ERROR("SerializationJob: chunk %d %d wrong inflate signature", chunkData.m_X, chunkData.m_Z);
        ASSERT(false);
		delete[] chunkData.m_CompressedData;
		return;
	}

	std::ostringstream filename;
	filename << WORLD_PATH "/";
	filename << chunkData.m_X;
	filename << '_';
	filename << chunkData.m_Z;
	filename << ".data";

	std::ofstream stream(filename.str(), std::ios::out | std::ios::binary | std::ios::trunc);
	stream.write((const char*)& chunkData.m_X, sizeof(chunkData.m_X));
	stream.write((const char*)& chunkData.m_Z, sizeof(chunkData.m_Z));
	stream.write((const char*)& chunkData.m_bGroundUpCon, sizeof(chunkData.m_bGroundUpCon));
	stream.write((const char*)& chunkData.m_PrimaryBitMap, sizeof(chunkData.m_PrimaryBitMap));
	stream.write((const char*)& chunkData.m_AddBitMap, sizeof(chunkData.m_AddBitMap));
	stream.write((const char*)& chunkData.m_CompressedSize, sizeof(chunkData.m_CompressedSize));
	stream.write((const char*)chunkData.m_CompressedData, chunkData.m_CompressedSize);
	stream.close();

	delete[] chunkData.m_CompressedData;

    //LOG("SerializationJob: Serialize Chunk %d %d", chunkData.m_X, chunkData.m_Z);

    static bool isLoading = false;
    if (m_queue.GetCount() > 10)
    {
        isLoading = true;
    }
    if (isLoading && m_queue.GetCount() == 0)
    {
        isLoading = false;
        core::IEventManager::Get()->ThreadSafeQueueEvent(std::make_shared<EventDataAllChunksInQueueSerialized>());
    }
}
