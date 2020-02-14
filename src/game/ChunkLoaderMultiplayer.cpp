#include <iostream>
#include <sstream>
#include <fstream>
#include "ChunkLoaderMultiplayer.h"
#include "Zip.h"
#include "globals.h"
#include "core.h"

void wiicraft::ChunkLoaderMultiplayer::Execute()
{
    std::shared_ptr<ChunkSection> chunkData = m_queue.Pop();

	std::ostringstream filename;
    filename << WORLD_PATH "/";
    filename << static_cast<int32_t>(chunkData->GetPosition().first);
	filename << '_';
    filename << static_cast<int32_t>(chunkData->GetPosition().second);
	filename << ".data";    

	std::ifstream fstream(filename.str(), std::ios::in | std::ios::binary);
	if (fstream)
	{
		int32_t x = 0, z = 0;
		bool bGroundUpCon = false;
		uint16_t primaryBitMap = 0, addBitMap = 0;
		int32_t compressedSize = 0;
		unsigned char* compressedData = nullptr;

		fstream.read((char*)& x, sizeof(x));
		fstream.read((char*)& z, sizeof(z));
		fstream.read((char*)& bGroundUpCon, sizeof(bGroundUpCon));
		fstream.read((char*)& primaryBitMap, sizeof(primaryBitMap));
		fstream.read((char*)& addBitMap, sizeof(addBitMap));
		fstream.read((char*)& compressedSize, sizeof(compressedSize));

        if (compressedSize <= 0)
        {
            // Chunk not completely loaded -> push back into queue
            m_queue.Push(chunkData);
            return;
        }

		compressedData = new unsigned char[compressedSize];
		ASSERT(compressedData != nullptr);
		fstream.read((char*)compressedData, compressedSize);
		fstream.close();

		uint32_t sections = 0;
		const int32_t sectionSize = 4096 + (3 * 2048);

		for (uint32_t i = 0; i < 16; ++i)
			sections += primaryBitMap >> i & 1;

		size_t size = sections * sectionSize;
		if (bGroundUpCon)
			size += 256;

        ASSERT(compressedSize > 0);

		unsigned char* cdata = new unsigned char[size];
        size_t s = core::Zip::Decompress(compressedData, compressedSize, cdata, size);
		ASSERT(s == size);

		delete[] compressedData;
		compressedData = nullptr;

        BlockType***  blocks = chunkData->GetBlocks();
		for (uint32_t i = 0; i < 16; ++i)
		{
			if (primaryBitMap & 1 << i)
			{
				for (int ix = 0; ix < 16; ++ix)
				{
					for (int iy = 0; iy < 16; ++iy)
					{
						for (int iz = 0; iz < 16; ++iz)
						{
							unsigned char block = (cdata[(iz * 16 * 16) + (iy * 16) + ix + (i * 16 * 16 * 16)]);
                            BlockType* blockPtr = &blocks[ix][iz + (i * 16)][iy];

                            switch (block) // TODO remove when all blocks are implemented!
							{
                            case 0:
                                *blockPtr = BlockType::AIR;
                                break;
                            case 1:
                                *blockPtr = BlockType::STONE;
                                break;
                            case 2:
                                *blockPtr = BlockType::GRASS;
                                break;
                            case 3:
                                *blockPtr = BlockType::DIRT;
                                break;
                            case 4:
                                *blockPtr = BlockType::COBBLESTONE;
                                break;
                            case 5:
                                *blockPtr = BlockType::PLANKS;
                                break;
                            case 6:
                                *blockPtr = BlockType::SAPLINGS;
                                break;
                            case 7:
                                *blockPtr = BlockType::BEDROCK;
                                break;
                            case 8:
                                *blockPtr = BlockType::WATER;
                                break;
                            case 9:
                                *blockPtr = BlockType::STATIONARY_WATER;
                                break;
                            case 10:
                                *blockPtr = BlockType::LAVA;
                                break;
                            case 11:
                                *blockPtr = BlockType::STATIONARY_LAVA;
                                break;
                            case 12:
                                *blockPtr = BlockType::SAND;
                                break;
                            case 14:
                                *blockPtr = BlockType::GLASS;
                                break;
                            case 17:
                                *blockPtr = BlockType::WOOD;
                                break;
                            case 18:
                                *blockPtr = BlockType::LEAF;
                                break;
							case 31:
                                *blockPtr = BlockType::AIR;
								break;
                            case 45:
                                *blockPtr = BlockType::BRICKS;
                                break;
                            case 46:
                                *blockPtr = BlockType::TNT;
                                break;
                            case 106: // Skip drawing Vines
                                *blockPtr = BlockType::AIR;
                                break;
							default:
                                *blockPtr = BlockType::DIRT;
								break;
							}
						}
					}
				}                
                chunkData->SetLoaded(true);
			}
		}
		delete[] cdata;
	}
	else
	{        
        chunkData->SetTo(wiicraft::BlockType::AIR);
        chunkData->SetLoaded(true);
	}
}
